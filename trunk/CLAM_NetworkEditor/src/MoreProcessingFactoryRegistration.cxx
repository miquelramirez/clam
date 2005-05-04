#include "ProcessingFactory.hxx"
#include "Processing.hxx"
#include "MultiChannelAudioFileReader.hxx"
#include "MultiChannelAudioFileWriter.hxx"

#ifndef WIN32
#include "LadspaLoader.hxx"
#endif

// processings in local directory
#include "Random.hxx"
#include "FlagControl.hxx"
#include "OneOverF.hxx"
#include "SquareWave.hxx"
#include "OutControlSender.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
#include "OSCSender.hxx"

#include "AudioPlotProcessing.hxx"
#include "SpectrumPlotProcessing.hxx"
#include "PeaksPlotProcessing.hxx"
#include "FundPlotProcessing.hxx"
#include "SinTracksPlotProcessing.hxx"
#include "FundTrackPlotProcessing.hxx"
#include "SpecgramPlotProcessing.hxx"
#include "AudioBuffPlotProcessing.hxx"

typedef CLAM::ProcessingFactory ProcessingFactory;

// processings in draft directory
static ProcessingFactory::Registrator<CLAM::FlagControl> regtFlagControl( "FlagControl" );
static ProcessingFactory::Registrator<CLAM::Random> regtRandom( "Random" );
static ProcessingFactory::Registrator<CLAM::OneOverF> regtOneOverF( "OneOverF" );
static ProcessingFactory::Registrator<CLAM::SquareWave> regtSquareWave( "SquareWave" );
static ProcessingFactory::Registrator<CLAM::OSCSender> regtOSCSender( "OSCSender" );
#ifndef WIN32
static ProcessingFactory::Registrator<CLAM::LadspaLoader> regtLadspaLoader( "LadspaLoader" );
#endif

static ProcessingFactory::Registrator<CLAM::MonoAudioFileReader> regtMonoAudioFileReader( "MonoAudioFileReader" );
static ProcessingFactory::Registrator<CLAM::MonoAudioFileWriter> regtMonoAudioFileWriter( "MonoAudioFileWriter" );
static ProcessingFactory::Registrator<CLAM::MultiChannelAudioFileReader> regtMultiChannelAudioFileReader( "MultiChannelAudioFileReader" );
static ProcessingFactory::Registrator<CLAM::MultiChannelAudioFileWriter> regtMultiChannelAudioFileWriter( "MultiChannelAudioFileWriter" );

static ProcessingFactory::Registrator<CLAM::OutControlSender> regtOutControlSender( "OutControlSender" );

static ProcessingFactory::Registrator<CLAM::AudioPlotProcessing> regtAudioPlotProcessing("AudioPlotProcessing");
static ProcessingFactory::Registrator<CLAM::SpectrumPlotProcessing> regtSpectrumPlotProcessing("SpectrumPlotProcessing");
static ProcessingFactory::Registrator<CLAM::PeaksPlotProcessing> regtPeaksPlotProcessing("PeaksPlotProcessing");
static ProcessingFactory::Registrator<CLAM::FundPlotProcessing> regtFundPlotProcessing("FundPlotProcessing");
static ProcessingFactory::Registrator<CLAM::SinTracksPlotProcessing> regtSinTracksPlotProcessing("SinTracksPlotProcessing");
static ProcessingFactory::Registrator<CLAM::FundTrackPlotProcessing> regtFundTrackPlotProcessing("FundTrackPlotProcessing");
static ProcessingFactory::Registrator<CLAM::SpecgramPlotProcessing> regtSpecgramPlotProcessing("SpecgramPlotProcessing");
static ProcessingFactory::Registrator<CLAM::AudioBuffPlotProcessing> regtAudioBuffPlotProcessing("AudioBuffPlotProcessing");


