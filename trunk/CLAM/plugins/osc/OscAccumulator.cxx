#include "OscAccumulator.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace {
	
static const char * metadata[] = {
	"key", "OscAccumulator",
	"category", "[plugin] Open Sound Control",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, CLAM::OscAccumulator> registrator(metadata);

}