#include "DummyProcessings.hxx"
#include <CLAM/ProcessingFactory.hxx>


static const char * metadata[] = {
	"key", "DummyProcessingWithInAndOutPorts",
	"category", "Dummy Processings",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, DummyProcessingWithInAndOutPorts> registrator(metadata);

static const char * metadata2[] = {
	"key", "DummyProcessingWithInAndOutControls",
	"category", "Dummy Processings",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, DummyProcessingWithInAndOutControls> registrator2(metadata2);

static const char * metadata3[] = {
	"key", "DummyProcessingWithMultiplePortsAndControls",
	"category", "Dummy Processings",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, DummyProcessingWithMultiplePortsAndControls> registrator3(metadata3);

static const char * metadata4[] = {
	"key", "DummyProcessingWithStringConfiguration",
	"category", "Dummy Processings",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, DummyProcessingWithStringConfiguration> registrator4(metadata4);

static const char * metadata5[] = {
	"key", "DummyProcessingWithIntConfiguration",
	"category", "Dummy Processings",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, DummyProcessingWithIntConfiguration> registrator5(metadata5);