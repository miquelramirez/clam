#include "Factory.hxx"
#include "DummyProducts.hxx"

namespace CLAMTest
{

typedef CLAM::Factory< A > FactoryOfAs;

static FactoryOfAs::Registrator<B> DummyRegt("B");

}
