#include "Factory.hxx"
#include "Qt_ProcessingPresentation.hxx"
#include "Qt_OutControlSenderPresentation.hxx"


typedef CLAM::Factory<NetworkGUI::Qt_ProcessingPresentation> Qt_ProcessingPresentationFactory;

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtSimpleOscillator( "SimpleOscillator" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtOscillator( "Oscillator" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtADSR( "ADSR" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtAutoPanner( "AutoPanner" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtAudioFileIn( "AudioFileIn" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtAudioFileOut( "AudioFileOut" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtAudioOut( "AudioOut" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtAudioAdder( "AudioAdder" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtAudioMixer( "AudioMixer" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtAudioMultiplier( "AudioMultiplier" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtFFT_rfftw( "FFT_rfftw" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtIFFT_rfftw( "IFFT_rfftw" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtLadspaLoader( "LadspaLoader" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtMonoAudioFileReader( "MonoAudioFileReader" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtMonoAudioFileWriter( "MonoAudioFileWriter" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtMultiChannelAudioFileReader( "MultiChannelAudioFileReader" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtMultiChannelAudioFileWriter( "MultiChannelAudioFileWriter" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtFlagControl( "FlagControl" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtRandom( "Random" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtOneOverF( "OneOverF" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation > 
	regtSquareWave( "SquareWave" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_OutControlSenderPresentation > 
	regtOutControlSender( "OutControlSender" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation> 
	regtSMSAnalysisCore( "SMSAnalysisCore" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation> 
	regtSMSSynthesis( "SMSSynthesis" );

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtFundamental2Control( "Fundamental2Control" );


// MIDI
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtMIDIKeyboard( "MIDIKeyboard" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtMIDInControl( "MIDIInControl" );
//static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
//        regtMIDIDispatcher( "MIDIDispatcher" );

// Plot Processings

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
	regtAudioPlotProcessing( "AudioPlotProcessing" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
	regtSpectrumPlotProcessing( "SpectrumPlotProcessing" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtPeaksPlotProcessing( "PeaksPlotProcessing" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtFundPlotProcessing( "FundPlotProcessing" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtSinTracksPlotProcessing( "SinTracksPlotProcessing" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtFundTrackPlotProcessing( "FundTrackPlotProcessing" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtSpecgramPlotProcessing( "SpecgramPlotProcessing" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtAudioBuffPlotProcessing( "AudioBuffPlotProcessing" );

// SMS Processings
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtSMSFreqShift( "SMSFreqShift" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtSMSPitchShift( "SMSPitchShift" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtSMSResidualGain( "SMSResidualGain" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtSMSSinusoidalGain( "SMSSinusoidalGain" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtSMSSineFilter( "SMSSineFilter" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtSMSOddEvenHarmonicRatio( "SMSOddEvenHarmonicRatio" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtSMSSpectralShapeShift( "SMSSpectralShapeShift" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtSMSPitchDiscretization( "SMSPitchDiscretization" );
static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ProcessingPresentation>
        regtSMSGenderChange( "SMSGenderChange" );
		
