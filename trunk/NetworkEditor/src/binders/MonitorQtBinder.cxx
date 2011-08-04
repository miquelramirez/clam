#include "MonitorQtBinder.hxx"

#include "Oscilloscope.hxx"
#include "OscilloscopeMonitor.hxx"
#include "Vumeter.hxx"
#include "VumeterMonitor.hxx"
#include "SpectrumView.hxx"
#include "SpectrumViewMonitor.hxx"
#include "PeakView.hxx"
#include "PeakViewMonitor.hxx"
#include "Tonnetz.hxx"
#include "TonnetzMonitor.hxx"
#include "KeySpace.hxx"
#include "KeySpaceMonitor.hxx"
#include "Spectrogram.hxx"
#include "SpectrogramMonitor.hxx"
#include "PolarChromaPeaks.hxx"
#include "PolarChromaPeaksMonitor.hxx"
#include "ChordRanking.hxx"
#include "ChordRankingMonitor.hxx"
#include "LPModelView.hxx"
#include "LPModelViewMonitor.hxx"
#include "MelSpectrumView.hxx"
#include "MelSpectrumViewMonitor.hxx"
#include "MelCepstrumView.hxx"
#include "MelCepstrumViewMonitor.hxx"
#include "SegmentationView.hxx"
#include "SegmentationViewMonitor.hxx"


static CLAM::MonitorQtBinder<Oscilloscope,OscilloscopeMonitor> oscilloscopeBinder
	("Oscilloscope");
static CLAM::MonitorQtBinder<Vumeter,VumeterMonitor> vumeterBinder
	("Vumeter");
static CLAM::MonitorQtBinder<SpectrumView,SpectrumViewMonitor> spectrumViewBinder
	("SpectrumView");
static CLAM::MonitorQtBinder<PeakView,PeakViewMonitor> peakMonitorBinder
	("PeakView");
static CLAM::MonitorQtBinder<CLAM::VM::Tonnetz,TonnetzMonitor> tonnetzBinder
	("CLAM::VM::Tonnetz");
static CLAM::MonitorQtBinder<CLAM::VM::KeySpace,KeySpaceMonitor> keyspaceBinder
	("CLAM::VM::KeySpace");
static CLAM::MonitorQtBinder<CLAM::VM::Spectrogram,SpectrogramMonitor> spectrogramBinder
	("CLAM::VM::Spectrogram");
static CLAM::MonitorQtBinder<PolarChromaPeaks,PolarChromaPeaksMonitor> polarChromaPeaksBinder
	("PolarChromaPeaks");
static CLAM::MonitorQtBinder<CLAM::VM::ChordRanking,ChordRankingMonitor> chordRankingBinder
	("CLAM::VM::ChordRanking");
static CLAM::MonitorQtBinder<CLAM::VM::LPModelView,LPModelViewMonitor> lpModelBinder
	("CLAM::VM::LPModelView");
static CLAM::MonitorQtBinder<CLAM::VM::MelCepstrumView,MelCepstrumViewMonitor> melCepstrumBinder
	("CLAM::VM::MelCepstrumView");
static CLAM::MonitorQtBinder<CLAM::VM::MelSpectrumView,MelSpectrumViewMonitor> melSpectrumBinder
	("CLAM::VM::MelSpectrumView");
static CLAM::MonitorQtBinder<SegmentationView,SegmentationViewMonitor> segmentationBinder
	("SegmentationView");



