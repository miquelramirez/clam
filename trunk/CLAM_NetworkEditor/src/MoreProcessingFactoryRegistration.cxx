
#include "Factory.hxx"
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

#include "AudioPlotProcessing.hxx"


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

// processings in draft directory
static ProcessingFactory::Registrator<CLAM::FlagControl> regtFlagControl( "FlagControl" );
static ProcessingFactory::Registrator<CLAM::Random> regtRandom( "Random" );
static ProcessingFactory::Registrator<CLAM::OneOverF> regtOneOverF( "OneOverF" );
static ProcessingFactory::Registrator<CLAM::SquareWave> regtSquareWave( "SquareWave" );

#ifndef WIN32
static ProcessingFactory::Registrator<CLAM::LadspaLoader> regtLadspaLoader( "LadspaLoader" );
#endif

static ProcessingFactory::Registrator<CLAM::MonoAudioFileReader> regtMonoAudioFileReader( "MonoAudioFileReader" );
static ProcessingFactory::Registrator<CLAM::MonoAudioFileWriter> regtMonoAudioFileWriter( "MonoAudioFileWriter" );
static ProcessingFactory::Registrator<CLAM::MultiChannelAudioFileReader> regtMultiChannelAudioFileReader( "MultiChannelAudioFileReader" );
static ProcessingFactory::Registrator<CLAM::MultiChannelAudioFileWriter> regtMultiChannelAudioFileWriter( "MultiChannelAudioFileWriter" );

static ProcessingFactory::Registrator<CLAM::OutControlSender> regtOutControlSender( "OutControlSender" );

static ProcessingFactory::Registrator<CLAM::AudioPlotProcessing> regtAudioPlotProcessing("AudioPlotProcessing");

