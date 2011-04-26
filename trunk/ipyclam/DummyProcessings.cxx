#include "DummyProcessings.hxx"
#include <CLAM/ProcessingFactory.hxx>

static const char * metadata[] = {
	"key", "DummyProcessingWithInAndOutPorts",
	"category", "Spectral Transformations",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, DummyProcessingWithInAndOutPorts> registrator(metadata);