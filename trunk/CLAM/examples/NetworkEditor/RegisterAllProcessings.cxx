
#include "Factory.hxx"
#include "Oscillator.hxx"
#include "AudioMixer.hxx"
#include "AudioMultiplier.hxx"
#include "AudioFileOut.hxx"
#include "AudioFileIn.hxx"

/**
 * This .cxx is a provisional way of having some processings registered to the 
 * ProcessingFactory. The registrations must happen in a decentralized way:
 * that is in each concrete processing implementation file. 
 *
 */
typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
typedef CLAM::AudioMixer< 2 > MixerTwoInPorts;

static ProcessingFactory::Registrator<CLAM::Oscillator> regtOscil( "Oscillator" );
static ProcessingFactory::Registrator<MixerTwoInPorts> regtMixer( "Mixer 2" );
static ProcessingFactory::Registrator<CLAM::AudioMultiplier> regtMulti( "Multiplier" );



