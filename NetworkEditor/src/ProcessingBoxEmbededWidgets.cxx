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


namespace { static CLAM::EmbededMonitorCreator <PeakView, PeakViewMonitor> regPeakView("PeakView"); } 
namespace { static CLAM::EmbededMonitorCreator <PolarChromaPeaks, PolarChromaPeaksMonitor> regPolarChromaPeaks("PolarChromaPeaks"); }
namespace { static CLAM::EmbededMonitorCreator <CLAM::VM::Tonnetz, TonnetzMonitor> regTonnetz("Tonnetz"); }
namespace { static CLAM::EmbededMonitorCreator <CLAM::VM::KeySpace, KeySpaceMonitor> regKeySpace("KeySpace"); }
namespace { static CLAM::EmbededMonitorCreator <CLAM::VM::Spectrogram, SpectrogramMonitor> regSpectrogram("Spectrogram"); }
namespace { static CLAM::EmbededMonitorCreator <CLAM::VM::ChordRanking,ChordRankingMonitor> regChordRanking("ChordRanking"); }
namespace { static CLAM::EmbededMonitorCreator <Tunner,TunnerMonitor> regTunner("Tunner"); }
namespace { static CLAM::EmbededMonitorCreator2 <Vumeter,VumeterMonitor> regVumeter("Vumeter"); }
namespace { static CLAM::EmbededMonitorCreator2 <Oscilloscope,OscilloscopeMonitor> regOscilloscop("Oscilloscope"); }
namespace { static CLAM::EmbededMonitorCreator2 <BufferOscilloscope,BufferOscilloscopeMonitor> regBufferOscilloscope("BufferOscilloscope"); }
namespace { static CLAM::EmbededMonitorCreator2 <SpectrumView,SpectrumViewMonitor> regSpectrumView("SpectrumView"); }
namespace { static CLAM::EmbededMonitorCreator2 <CLAM::VM::LPModelView,LPModelViewMonitor> regLPModelView("LPModelView"); }
namespace { static CLAM::EmbededMonitorCreator2 <CLAM::VM::MelSpectrumView,MelSpectrumViewMonitor> regMelSpectrumView("MelSpectrumView"); }
namespace { static CLAM::EmbededMonitorCreator2 <CLAM::VM::MelCepstrumView,MelCepstrumViewMonitor> regMelCepstrumView("MelCepstrumView"); }
namespace { static CLAM::EmbededMonitorCreator2 <CLAM::VM::VectorView,VectorViewMonitor> regVectorView("VectorView"); }
namespace { static CLAM::EmbededMonitorCreator2 <CLAM::VM::HistogramView,HistogramViewMonitor> regHistogramView("HistogramView"); }
namespace { static CLAM::EmbededMonitorCreator2 <SegmentationView,SegmentationViewMonitor> regSegmentationView("SegmentationView"); }


QWidget * ClamNetworkCanvas::embededWidgetFor(void * model)
{
	if (!model) return 0;

	CLAM::Processing * processing = (CLAM::Processing*) model;
	QWidget * myWidget = CLAM::EmbededWidgetCreatorBase::create(processing, this);
	if (myWidget) return myWidget;

	std::string className = processing->GetClassName();

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


