
#include "Factory.hxx"
#include "ProcessingConfigPresentation.hxx"

// concrete configs
#include "OscillatorConfigPresentation.hxx"
#include "ConfigPresentationTmpl.hxx"
#include "Controller.hxx"
#include "AudioFile.hxx"
#include "AutoPanner.hxx"
#include "AudioIO.hxx"
//#include "AudioMixer.hxx"
#include "BinaryAudioOp.hxx"
#include "Oscillator.hxx"
#include "FFTConfig.hxx"

typedef CLAM::Factory<NetworkGUI::ProcessingConfigPresentation> ProcessingConfigPresentationFactory;


typedef NetworkGUI::ConfigPresentationTmpl<CLAM::OscillatorConfig> OscillatorConfigPresentationTmpl;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::ControllerConfig> ControllerConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AutoPannerConfig> AutoPannerConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AudioFileConfig> AudioFileConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AudioIOConfig> AudioIOConfigPresentation;
//typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AudioMixerConfig> AudioMixerConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::BinaryAudioOpConfig> BinaryAudioOpConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::FFTConfig> FFTConfigPresentation;

static ProcessingConfigPresentationFactory::Registrator<OscillatorConfigPresentationTmpl> regtOscillatorCfg( "OscillatorConfig" );
static ProcessingConfigPresentationFactory::Registrator<ControllerConfigPresentation> regtControllerCfg( "ControllerConfig" );
static ProcessingConfigPresentationFactory::Registrator<AutoPannerConfigPresentation> regtAutoPannerCfg( "AutoPannerConfig" );
static ProcessingConfigPresentationFactory::Registrator<AudioFileConfigPresentation> regtAudioFileInCfg( "AudioFileConfig" );
static ProcessingConfigPresentationFactory::Registrator<AudioIOConfigPresentation> regtAudioOutCfg( "AudioIOConfig" );
//static ProcessingConfigPresentationFactory::Registrator<AudioMixerConfigPresentation> regAudioMixerCfg( "AudioMixerConfig" );
static ProcessingConfigPresentationFactory::Registrator<BinaryAudioOpConfigPresentation> regtBinaryAudioOpCfg( "BinaryAudioOpConfig" );
static ProcessingConfigPresentationFactory::Registrator<FFTConfigPresentation> regtFFTCfg( "FFTConfig" );


// manual config presentation
//static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::OscillatorConfigPresentation> regOscillatorCfg( "OscillatorConfig" );
