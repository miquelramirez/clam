#include "Factory.hxx" // CLAM
#include "DummyProducts.hxx"
#include "FactoryOnOtherTranslationUnits.hxx"

namespace CLAMTest
{


typedef CLAM::Factory< A > FactoryOfAs;

	FactoryOfAs* gAddressOfFactoryUsedByRegistrator;


  static     FactoryOfAs::Registrator<B> DummyRegt( "B" );

}

