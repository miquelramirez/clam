
#include "Factory.hxx"
#include "Oscillator.hxx"
//#include "AudioMixer.hxx"
#include "AudioMultiplier.hxx"
#include "AudioAdder.hxx"
#include "AudioFileOut.hxx"
#include "AudioFileIn.hxx"
#include "AutoPanner.hxx"
#include "FFT_rfftw.hxx"
#include "AudioOut.hxx"
#include "Controller.hxx"

// VC6 needs to see the whole types
#include "Audio.hxx"
#include "Spectrum.hxx"


/**
 * In this cxx are defined the objects that will be in the factory by default. Including it 
 * in a project will let create objects of these types without include all the processings
 * headers. If a project needs some processings not included, another .cxx can be created
 * to register more processings (with the condition of avoid duplicated keys).
 * 
 * The way processings are registered in factory could be changed in a future.
 * Warning: This file has a lot of dependencies.
 */
typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

static ProcessingFactory::Registrator<CLAM::Oscillator> regtOscillator( "Oscillator" );
static ProcessingFactory::Registrator<CLAM::AudioMultiplier> regtAudioMultiplier( "AudioMultiplier" );
static ProcessingFactory::Registrator<CLAM::AudioAdder> regtAudioAdder( "AudioAdder" );
static ProcessingFactory::Registrator<CLAM::FFT_rfftw> regtFFT_rfftw( "FFT_rfftw" );
static ProcessingFactory::Registrator<CLAM::AutoPanner> regtAutoPanner( "AutoPanner" );
static ProcessingFactory::Registrator<CLAM::AudioOut> regtAudioOut( "AudioOut" );
static ProcessingFactory::Registrator<CLAM::Controller> regtController( "Controller" );
static ProcessingFactory::Registrator<CLAM::AudioFileIn> regtAudioFileIn( "AudioFileIn" );
static ProcessingFactory::Registrator<CLAM::AudioFileOut> regtAudioFileOut( "AudioFileOut" );
