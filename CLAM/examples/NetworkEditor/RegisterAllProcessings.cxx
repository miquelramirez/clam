
#include "Factory.hxx"
#include "Oscillator.hxx"
#include "AudioMixer.hxx"
#include "AudioMultiplier.hxx"
#include "AudioFileOut.hxx"
#include "AudioFileIn.hxx"

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

static ProcessingFactory::Registrator<CLAM::Oscillator> regtOscil( "Oscillator" );
//static ProcessingFactory::Registrator<CLAM::AudioMixer<2>> regtMixer( "Mixer 2" );
static ProcessingFactory::Registrator<CLAM::AudioMultiplier> regtMulti( "Multiplier" );
static ProcessingFactory::Registrator<CLAM::AudioFileIn> regtFileIn( "Audio File In" );
static ProcessingFactory::Registrator<CLAM::AudioFileOut> regtFileOut( "Audio File Out" );



