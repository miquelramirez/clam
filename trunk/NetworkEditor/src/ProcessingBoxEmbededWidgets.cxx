#include "ProcessingBox.hxx"
#include "NetworkCanvas.hxx"

//TODO move this group to a factory
#include "Vumeter.hxx"
#include "Oscilloscope.hxx"
#include "BufferOscilloscope.hxx"
#include "SpectrumView.hxx"
#include "PeakView.hxx"
#include "PolarChromaPeaks.hxx"
#include "Tonnetz.hxx"
#include "KeySpace.hxx"
#include "KeySpaceMonitor.hxx"
#include "Spectrogram.hxx"
#include "SpectrogramMonitor.hxx"
#include "ChordRanking.hxx"
#include "Tunner.hxx"
#include "LPModelView.hxx"
#include "MelSpectrumView.hxx"
#include "MelCepstrumView.hxx"
#include "VectorView.hxx"
#include "ControlPrinterWidget.hxx"
#include "ControlSenderWidget.hxx"
#include "ControlSurfaceWidget.hxx"

#include <typeinfo>
#include <CLAM/ProcessingDataPlugin.hxx>


QWidget * embededWidgetFor(CLAM::Processing * processing, QWidget * canvas)
{
	if (!processing) return 0;
	std::string className = processing->GetClassName();

	if (className=="OutControlSender")
		return new ControlSenderWidget(processing);

	if (className=="ControlSurface")
		return new ControlSurfaceWidget(canvas, processing);

	if (className=="ControlPrinter" || className=="ControlTraceWriter")
		return new ControlPrinterWidget(processing);

	if (className=="Vumeter")
		return new Vumeter( canvas, dynamic_cast<VumeterMonitor*>(processing) );

	if (className=="Oscilloscope")
		return new Oscilloscope( canvas, dynamic_cast<OscilloscopeMonitor*>(processing) );

	if (className=="BufferOscilloscope")
		return new BufferOscilloscope( canvas, dynamic_cast<BufferOscilloscopeMonitor*>(processing) );

	if (className=="SpectrumView")
		return new SpectrumView(canvas, dynamic_cast<SpectrumViewMonitor*>(processing) );

	if (className=="PeakView")
	{
		PeakView * widget = new PeakView(canvas);
		widget->setDataSource( *dynamic_cast<PeakViewMonitor*>(processing) );
		return widget;
	}
	if (className=="PolarChromaPeaks")
	{
		PolarChromaPeaks * widget = new PolarChromaPeaks(canvas);
		widget->setDataSource( *dynamic_cast<PolarChromaPeaksMonitor*>(processing) );
		return widget;
	}
	if (className=="Tonnetz")
	{
		CLAM::VM::Tonnetz * widget = new CLAM::VM::Tonnetz(canvas);
		widget->setDataSource( *dynamic_cast<TonnetzMonitor*>(processing) );
		return widget;
	}
	if (className=="KeySpace")
	{
		CLAM::VM::KeySpace * widget = new CLAM::VM::KeySpace(canvas);
		widget->setDataSource( *dynamic_cast<KeySpaceMonitor*>(processing) );
		return widget;
	}
	if (className=="Spectrogram")
	{
		CLAM::VM::Spectrogram * widget = new CLAM::VM::Spectrogram(canvas);
		widget->setDataSource( *dynamic_cast<SpectrogramMonitor*>(processing) );
		return widget;
	}
	if (className=="ChordRanking")
	{
		CLAM::VM::ChordRanking * widget = new CLAM::VM::ChordRanking(canvas);
		widget->setDataSource( *dynamic_cast<ChordRankingMonitor*>(processing) );
		return widget;
	}
	if (className=="Tunner")
	{
		Tunner * widget = new Tunner(canvas);
		widget->setDataSource( *dynamic_cast<TunnerMonitor*>(processing) );
		return widget;
	}
	if (className=="LPModelView")
		return new CLAM::VM::LPModelView(canvas, dynamic_cast<LPModelViewMonitor*>(processing));

	if (className=="MelSpectrumView")
		return new CLAM::VM::MelSpectrumView(canvas, dynamic_cast<MelSpectrumViewMonitor*>(processing));

	if (className=="MelCepstrumView")
		return new CLAM::VM::MelCepstrumView(canvas, dynamic_cast<MelCepstrumViewMonitor*>(processing));

	if (className=="VectorView")
		return new CLAM::VM::VectorView(canvas, dynamic_cast<VectorViewMonitor*>(processing));

	return 0;
}


