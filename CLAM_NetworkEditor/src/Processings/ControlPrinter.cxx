#include "ControlPrinter.hxx"
#include "Factory.hxx"

namespace CLAM
{

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
static ProcessingFactory::Registrator<CLAM::ControlPrinter> regtControlPrinter( "ControlPrinter" );

}
