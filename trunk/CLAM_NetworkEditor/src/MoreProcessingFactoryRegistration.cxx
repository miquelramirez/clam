
#include "Factory.hxx"
#include "Processing.hxx"

// processings in draft directory
#include "Random.hxx"
#include "FlagControl.hxx"
#include "OneOverF.hxx"
//#include "LadspaLoader.hxx"

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

// processings in draft directory
static ProcessingFactory::Registrator<CLAM::FlagControl> regtFlagControl( "FlagControl" );
static ProcessingFactory::Registrator<CLAM::Random> regtRandom( "Random" );
static ProcessingFactory::Registrator<CLAM::OneOverF> regtOneOverF( "OneOverF" );
//static ProcessingFactory::Registrator<CLAM::LadspaLoader> regtLadspaLoader( "LadspaLoader" );
