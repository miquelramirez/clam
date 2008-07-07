#include "MyProcessing.hxx"
#include <CLAM/ProcessingFactory.hxx>

static const char * metadata[] =
{
	"key", "MyProcessing",
	"category", "Examples",
	0
};
static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MyProcessing> registrator(metadata);

