#include "ProcessingBox.hxx"
#include "ClamNetworkCanvas.hxx"

//TODO move this group to a factory
#include "Vumeter.hxx"
#include "VumeterMonitor.hxx"
#include "Oscilloscope.hxx"
#include "OscilloscopeMonitor.hxx"
#include "BufferOscilloscope.hxx"
#include "BufferOscilloscopeMonitor.hxx"
#include "SpectrumView.hxx"
#include "SpectrumViewMonitor.hxx"
#include "PeakView.hxx"
#include "PeakViewMonitor.hxx"
#include "PolarChromaPeaks.hxx"
#include "PolarChromaPeaksMonitor.hxx"
#include "Tonnetz.hxx"
#include "TonnetzMonitor.hxx"
#include "KeySpace.hxx"
#include "KeySpaceMonitor.hxx"
#include "Spectrogram.hxx"
#include "SpectrogramMonitor.hxx"
#include "ChordRanking.hxx"
#include "ChordRankingMonitor.hxx"
#include "Tunner.hxx"
#include "TunnerMonitor.hxx"
#include "LPModelView.hxx"
#include "LPModelViewMonitor.hxx"
#include "MelSpectrumView.hxx"
#include "MelSpectrumViewMonitor.hxx"
#include "MelCepstrumView.hxx"
#include "MelCepstrumViewMonitor.hxx"
#include "VectorView.hxx"
#include "VectorViewMonitor.hxx"
#include "HistogramView.hxx"
#include "HistogramViewMonitor.hxx"
#include "SegmentationView.hxx"
#include "SegmentationViewMonitor.hxx"
#include "MIDIPianoWidget.hxx"
#include "ControlPrinterWidget.hxx"
#include "ControlSenderWidget.hxx"
#include "ControlSurfaceWidget.hxx"
#include "BoolControlDisplay.hxx"
#include "BoolControlSenderWidget.hxx"
#include "ProgressControlWidget.hxx"


#include <typeinfo>
#include <CLAM/ProcessingDataPlugin.hxx>

#include <QtSvg/QSvgWidget>
#include <QtSvg/QSvgRenderer>
#include <QtCore/QFileInfo> // added to check if embbeded file exists as external without console error message


namespace CLAM
{
class EmbededWidgetFactory
{
private:
	typedef std::string Key;
	typedef std::map<Key, EmbededWidgetFactory*> Creators;
	static Creators & creators()
	{
		static Creators creators;
		return creators;
	}
protected:
	EmbededWidgetFactory(const Key & processingTypeName)
	{
		creators().insert(std::make_pair(processingTypeName, this));
	}
	virtual ~EmbededWidgetFactory() {} // TODO: to the cxx
	virtual QWidget * concreteCreate(CLAM::Processing * processing, QWidget * parent) = 0;
public:
	static QWidget * create(CLAM::Processing * processing, QWidget * parent)
	{
		Key type = processing->GetClassName();
		Creators::iterator it = creators().find(type);
		if (it==creators().end()) return 0;
		return it->second->concreteCreate(processing, parent);
	}
};

template <typename WidgetType>
class EmbededWidgetCreator : public EmbededWidgetFactory
{
public:
	EmbededWidgetCreator(const Key & type)
		: EmbededWidgetFactory(type)
	{}
	virtual QWidget * concreteCreate(CLAM::Processing * processing, QWidget * parent)
	{
		return new WidgetType(processing, parent);
	}
};

template <typename WidgetType, typename MonitorType>
class EmbededMonitorCreator : public EmbededWidgetFactory
{
public:
	EmbededMonitorCreator(const Key & type)
		: EmbededWidgetFactory(type)
	{}
	virtual QWidget * concreteCreate(
		QWidget * parent, 
		CLAM::Processing * processing)
	{
		MonitorType * monitor = dynamic_cast<MonitorType*>(processing);
		if (not monitor) return 0;
		WidgetType * widget = new WidgetType(parent);
		widget->setDataSource(monitor);
	}
};

namespace {
	static EmbededWidgetCreator <ControlSurfaceWidget> reg("ControlSurface");
}

}

QWidget * ClamNetworkCanvas::embededWidgetFor(void * model)
{
	if (!model) return 0;
	CLAM::Processing * processing = (CLAM::Processing*) model;
	QWidget * myWidget = CLAM::EmbededWidgetFactory::create(processing, this);
	if (myWidget) return myWidget;

	std::string className = processing->GetClassName();

	if (className=="OutControlSender")
		return new ControlSenderWidget(processing);

	if (className=="ControlPrinter" || className=="ControlTraceWriter" || className=="ControlPrinterTyped" )
		return new ControlPrinterWidget(processing);

	if (className=="BoolControlPrinter")
		return new BoolControlDisplay(processing);

	if (className=="BoolControlSender")
		return new BoolControlSenderWidget(processing);

	if (className=="ControlPiano")
		return new CLAM::MIDIPianoWidget(processing);

	if (className=="ProgressControl")
		return new ProgressControlWidget(this, processing);

	if (className=="Vumeter")
		return new Vumeter( this, dynamic_cast<VumeterMonitor*>(processing) );

	if (className=="Oscilloscope")
		return new Oscilloscope( 0, dynamic_cast<OscilloscopeMonitor*>(processing) );

	if (className=="BufferOscilloscope")
		return new BufferOscilloscope( 0, dynamic_cast<BufferOscilloscopeMonitor*>(processing) );

	if (className=="SpectrumView")
		return new SpectrumView(this, dynamic_cast<SpectrumViewMonitor*>(processing) );

	if (className=="PeakView")
	{
		PeakView * widget = new PeakView(this);
		widget->setDataSource( *dynamic_cast<PeakViewMonitor*>(processing) );
		return widget;
	}
	if (className=="PolarChromaPeaks")
	{
		PolarChromaPeaks * widget = new PolarChromaPeaks(this);
		widget->setDataSource( *dynamic_cast<PolarChromaPeaksMonitor*>(processing) );
		return widget;
	}
	if (className=="Tonnetz")
	{
		CLAM::VM::Tonnetz * widget = new CLAM::VM::Tonnetz(this);
		widget->setDataSource( *dynamic_cast<TonnetzMonitor*>(processing) );
		return widget;
	}
	if (className=="KeySpace")
	{
		CLAM::VM::KeySpace * widget = new CLAM::VM::KeySpace(this);
		widget->setDataSource( *dynamic_cast<KeySpaceMonitor*>(processing) );
		return widget;
	}
	if (className=="Spectrogram")
	{
		CLAM::VM::Spectrogram * widget = new CLAM::VM::Spectrogram(this);
		widget->setDataSource( *dynamic_cast<SpectrogramMonitor*>(processing) );
		return widget;
	}
	if (className=="ChordRanking")
	{
		CLAM::VM::ChordRanking * widget = new CLAM::VM::ChordRanking(this);
		widget->setDataSource( *dynamic_cast<ChordRankingMonitor*>(processing) );
		return widget;
	}
	if (className=="Tunner")
	{
		Tunner * widget = new Tunner(this);
		widget->setDataSource( *dynamic_cast<TunnerMonitor*>(processing) );
		return widget;
	}
	if (className=="LPModelView")
		return new CLAM::VM::LPModelView(this, dynamic_cast<LPModelViewMonitor*>(processing));

	if (className=="MelSpectrumView")
		return new CLAM::VM::MelSpectrumView(this, dynamic_cast<MelSpectrumViewMonitor*>(processing));

	if (className=="MelCepstrumView")
		return new CLAM::VM::MelCepstrumView(this, dynamic_cast<MelCepstrumViewMonitor*>(processing));

	if (className=="VectorView")
		return new CLAM::VM::VectorView(this, dynamic_cast<VectorViewMonitor*>(processing));

	if (className=="HistogramView")
		return new CLAM::VM::HistogramView(this, dynamic_cast<HistogramViewMonitor*>(processing));

	if (className=="SegmentationView")
		return new SegmentationView(this, dynamic_cast<SegmentationViewMonitor*>(processing));

	// SVG embedding. TODO: replace embedded_svg for embedded_logo (requires load in QLabel? on QWidget, instead using QSvgWidget)
	CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
	bool existsDiagram=factory.AttributeExists(className,"faust_diagram");
	bool existsLogo=factory.AttributeExists(className,"embedded_svg");
	if (existsDiagram or existsLogo)
	{
		std::string embeddedSvg;
		if (existsLogo)
		{
			embeddedSvg=factory.GetValueFromAttribute(className,"embedded_svg");
		}
		// if embedding option is selected, diagram overwrites logo
		if (existsDiagram and _embedSVGDiagramsOption)
		{
			 embeddedSvg=factory.GetValueFromAttribute(className,"faust_diagram");
		}

		QSvgWidget * widget = new QSvgWidget(this);
		QSvgRenderer *renderer = widget->renderer();
		QString embeddedSvgQString = embeddedSvg.c_str();
		if ( QFileInfo(embeddedSvgQString).exists() and renderer->load(embeddedSvgQString) ) 
				return widget;
		if ( renderer->load(tr(":/icons/images/%1").arg(embeddedSvg.c_str())) )
				return widget;
		delete widget;
	}
	return 0;
}


