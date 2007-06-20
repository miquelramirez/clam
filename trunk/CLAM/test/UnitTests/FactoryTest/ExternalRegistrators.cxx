#include "Factory.hxx" // CLAM
#include "DummyProducts.hxx"
#include "FactoryOnOtherTranslationUnits.hxx"

namespace CLAMTest
{


FactoryOfAs* gAddressOfFactoryUsedByRegistrator;

static CLAM::FactoryRegistrator<FactoryOfAs, B> DummyRegt( "B" );

}

