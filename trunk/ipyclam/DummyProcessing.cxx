#include "DummyProcessing.hxx"
#include <CLAM/ProcessingFactory.hxx>

static const char * metadata[] = {
	"key", "DummyProcessing",
	"category", "Spectral Transformations",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, DummyProcessing> registrator(metadata);