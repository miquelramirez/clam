#include "ProcessingBox.hxx"
#include "ClamNetworkCanvas.hxx"

#include "EmbededWidgets.hxx"

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


#include <typeinfo>
#include <CLAM/ProcessingDataPlugin.hxx>

#include <QtSvg/QSvgWidget>
#include <QtSvg/QSvgRenderer>
#include <QtCore/QFileInfo> // added to check if embbeded file exists as external without console error message


namespace a {
	static CLAM::EmbededMonitorCreator <PeakView, PeakViewMonitor> reg("PeakView");
} namespace b{
	static CLAM::EmbededMonitorCreator <PolarChromaPeaks, PolarChromaPeaksMonitor> reg("PolarChromaPeaks");
} namespace c{
	static CLAM::EmbededMonitorCreator <CLAM::VM::Tonnetz, TonnetzMonitor> reg("Tonnetz");
} namespace d{
	static CLAM::EmbededMonitorCreator <CLAM::VM::KeySpace, KeySpaceMonitor> reg("KeySpace");
} namespace e{
	static CLAM::EmbededMonitorCreator <CLAM::VM::Spectrogram, SpectrogramMonitor> reg("Spectrogram");
} namespace f{
	static CLAM::EmbededMonitorCreator <CLAM::VM::ChordRanking,ChordRankingMonitor> reg("ChordRanking");
} namespace g{
	static CLAM::EmbededMonitorCreator <Tunner,TunnerMonitor> reg("Tunner");
}

QWidget * ClamNetworkCanvas::embededWidgetFor(void * model)
{
	if (!model) return 0;

	CLAM::Processing * processing = (CLAM::Processing*) model;
	QWidget * myWidget = CLAM::EmbededWidgetCreatorBase::create(processing, this);
	if (myWidget) return myWidget;

	std::string className = processing->GetClassName();

	if (className=="Vumeter")
		return new Vumeter( this, dynamic_cast<VumeterMonitor*>(processing) );

	if (className=="Oscilloscope")
		return new Oscilloscope( 0, dynamic_cast<OscilloscopeMonitor*>(processing) );

	if (className=="BufferOscilloscope")
		return new BufferOscilloscope( 0, dynamic_cast<BufferOscilloscopeMonitor*>(processing) );

	if (className=="SpectrumView")
		return new SpectrumView(this, dynamic_cast<SpectrumViewMonitor*>(processing) );

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


