#include "LowLatencyConvolution.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace Hidden
{

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, LowLatencyConvolution>
	regLowLatencyConvolution("LowLatencyConvolution");



}

}
