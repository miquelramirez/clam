
#include "InitProcessing.hxx"
#include <iostream>

#include <CLAM/AudioSource.hxx>
#include <CLAM/AudioSink.hxx>
#include <CLAM/ControlSource.hxx>
#include <CLAM/ControlSink.hxx>

#include <CLAM/OscillatingSpectralNotch.hxx>
#include <CLAM/TonalAnalysis.hxx>
#include <CLAM/SimpleOscillator.hxx>
#include <CLAM/Oscillator.hxx>
#include <CLAM/ADSR.hxx>
#include <CLAM/WaveGenerator.hxx>
#include <CLAM/AudioMultiplier.hxx>
#include <CLAM/AudioAdder.hxx>
#include <CLAM/AudioMixer.hxx>
#include <CLAM/SpectralPeakArrayAdder.hxx>
#include <CLAM/SpectrumAdder.hxx>
#include <CLAM/SpectrumAdder2.hxx>
#include <CLAM/AutoPanner.hxx>
#include <CLAM/FlagControl.hxx>
#include <CLAM/FFT_rfftw.hxx>
#include <CLAM/SMSAnalysisCore.hxx>
#include <CLAM/IFFT_rfftw.hxx>
#include <CLAM/SMSSynthesis.hxx>
#include <CLAM/SMSFreqShift.hxx>
#include <CLAM/SMSPitchShift.hxx>
#include <CLAM/SMSResidualGain.hxx>
#include <CLAM/SMSSinusoidalGain.hxx>
#include <CLAM/SMSSineFilter.hxx>
#include <CLAM/SMSOddEvenHarmonicRatio.hxx>
#include <CLAM/SMSSpectralShapeShift.hxx>
#include <CLAM/SMSPitchDiscretization.hxx>
#include <CLAM/SMSGenderChange.hxx>
#include <CLAM/OutControlSender.hxx>
#include <CLAM/Fundamental2Control.hxx>
#include <CLAM/ControlPrinter.hxx>
#include <CLAM/ControlScaler.hxx>
#include <CLAM/ControlMapper.hxx>
#include <CLAM/SMSHarmonizer.hxx>
#include <CLAM/SMSMorph.hxx>
#include <CLAM/SMSTimeStretch.hxx>
#include <CLAM/SMSTransformationChainIO.hxx>
//#include <CLAM/Controller.hxx>


//#include <CLAM/AudioPlotProcessing.hxx>
//#include <CLAM/SpectrumPlotProcessing.hxx>
//#include <CLAM/PeaksPlotProcessing.hxx>
//#include <CLAM/FundPlotProcessing.hxx>
//#include <CLAM/SinTracksPlotProcessing.hxx>
//#include <CLAM/FundTrackPlotProcessing.hxx>
//#include <CLAM/SpecgramPlotProcessing.hxx>
//#include <CLAM/AudioBuffPlotProcessing.hxx>
//#include <CLAM/PortMonitor.hxx>

namespace CLAM
{
	void ProcessingModule::init()
	{
		std::cout << "Initiating processing module...";
		CLAM::OscillatingSpectralNotch();
		CLAM::AudioSink();
		CLAM::AudioSource();
		CLAM::ControlSink();
		CLAM::ControlSource();
		CLAM::TonalAnalysis(false); 
		CLAM::FFT_rfftw();
		CLAM::SMSAnalysisCore();
		CLAM::AudioAdder();
		CLAM::AudioMixer();
		CLAM::AudioMultiplier();
		CLAM::AutoPanner();
		CLAM::ControlMapper();
		CLAM::ControlPrinter();
		CLAM::ControlScaler();
		// crashes
		// CLAM::Controller controller;
		CLAM::FlagControl();
		CLAM::Fundamental2Control();
		// linux only 
		// CLAM::OSCSender oscSender;
		CLAM::OutControlSender();
		CLAM::ADSR();
		CLAM::Oscillator();
		CLAM::SimpleOscillator();
		CLAM::WaveGenerator();

		// linux only
		// CLAM::LadspaLoader ladspaLoader;
		
		CLAM::IFFT_rfftw();
		CLAM::SMSSynthesis();
		CLAM::SMSFreqShift();
		CLAM::SMSGenderChange();
		CLAM::SMSHarmonizer();
		CLAM::SMSMorph();
		CLAM::SMSOddEvenHarmonicRatio();
		CLAM::SMSPitchDiscretization();
		CLAM::SMSPitchShift();
		CLAM::SMSResidualGain();
		CLAM::SMSSineFilter();
		CLAM::SMSSinusoidalGain();
		CLAM::SMSSpectralShapeShift();
		CLAM::SMSTimeStretch();
		CLAM::SMSTransformationChainIO();
		CLAM::SpectralPeakArrayAdder();
		CLAM::SpectrumAdder();
		CLAM::SpectrumAdder2();
		//CLAM::AudioOut audioOut;
		//CLAM::MonoAudioFileReader monoReader;
		//CLAM::MonoAudioFileWriter monoWriter;
		//CLAM::MultiChannelAudioFileReader multiReader;
		//CLAM::MultiChannelAudioFileWriter multiWriter;
		//CLAM::AudioPlotProcessing audioPlotProcessing;
		//CLAM::SpectrumPlotProcessing spectrumPlotProcessing;
		//CLAM::PeaksPlotProcessing peaksPlotProcessing;
		//CLAM::FundPlotProcessing fundPlotProcessing;
		//CLAM::SinTracksPlotProcessing sinTracksPlotProcessing;
		//CLAM::FundTrackPlotProcessing fundTrackPlotProcessing;
		//CLAM::SpecgramPlotProcessing specgramPlotProcessing;
		//CLAM::AudioBuffPlotProcessing audioBuffPlotProcessing;
		//CLAM::AudioPortMonitor audioPortMonitor;
		//CLAM::SpectrumPortMonitor spectrumPortMonitor;
		//CLAM::PeaksPortMonitor peaksPortMonitor;
		//CLAM::FundamentalPortMonitor fundamentalPortMonitor;
		//CLAM::AudioBuffPortMonitor audioBuffPortMonitor;
		//CLAM::SpecgramPortMonitor specgramPortMonitor;
		//CLAM::FundTrackPortMonitor fundTrackPortMonitor;
		//CLAM::SinTracksPortMonitor sinTracksPortMonitor;
		std::cout << "done!" << std::endl;
	}
}

