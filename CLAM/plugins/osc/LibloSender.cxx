#include "LibloSink.hxx"
#include <CLAM/ProcessingFactory.hxx>

static const char * metadata[] = {
	"key", "LibloSink",
	"category", "[plugin] Open Sound Control",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, CLAM::LibloSink> registrator(metadata);

