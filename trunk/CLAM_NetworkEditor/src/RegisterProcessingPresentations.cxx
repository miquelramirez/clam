

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
	regtAudioMultiplier( "AudioMultiplier" );
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

