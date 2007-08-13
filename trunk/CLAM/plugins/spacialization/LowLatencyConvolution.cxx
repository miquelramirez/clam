#include "LowLatencyConvolution.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace Hidden
{
	static const char* metadata[] = {
		"key", "LowLatencyConvolution",
		"category", "CLAM",
		"description", "LowLatencyConvolution",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, LowLatencyConvolution>
	reg = metadata;
}

}
