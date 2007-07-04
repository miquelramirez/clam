#include "ConstantSpectrum.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{
namespace Hidden
{

static FactoryRegistrator<ProcessingFactory, ConstantSpectrum>
	regConstantSpectrum("ConstantSpectrum");
}
}

