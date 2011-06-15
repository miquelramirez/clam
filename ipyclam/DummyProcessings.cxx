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

static const char * metadata6[] = {
	"key", "DummyProcessingWithCompleteConfiguration",
	"category", "Dummy Processings",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, DummyProcessingWithCompleteConfiguration> registrator6(metadata6);

static const char * metadata7[] = {
	"key", "DummyProcessingWithOptionalAttributes",
	"category", "Dummy Processings",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, DummyProcessingWithOptionalAttributes> registrator7(metadata7);

static const char * metadata8[] = {
	"key", "DummyTypeProcessing1",
	"category", "Dummy Processings",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, DummyTypeProcessing1> registrator8(metadata8);

static const char * metadata9[] = {
	"key", "DummyTypeProcessing2",
	"category", "Dummy Processings",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, DummyTypeProcessing2> registrator9(metadata9);

static const char * metadata10[] = {
	"key", "ProcessingWithPortsAndControls",
	"category", "Dummy Processings",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, ProcessingWithPortsAndControls> registrator10(metadata10);

static const char * metadata11[] = {
	"key", "PortSink",
	"category", "Dummy Processings",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, PortSink> registrator11(metadata11);

static const char * metadata12[] = {
	"key", "DummyControlSink",
	"category", "Dummy Processings",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, DummyControlSink> registrator12(metadata12);

