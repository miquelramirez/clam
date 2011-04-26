#include "DummyProcessings.hxx"
#include <CLAM/ProcessingFactory.hxx>

static const char * metadata[] = {
	"key", "DummyProcessingWithInAndOutPorts",
	"category", "Spectral Transformations",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, DummyProcessingWithInAndOutPorts> registrator(metadata);

static const char * metadata2[] = {
	"key", "DummyProcessingWithInAndOutControls",
	"category", "Spectral Transformations",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, DummyProcessingWithInAndOutControls> registrator2(metadata2);
