#include "DataSilentSource.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{

namespace detail
{
	static const char * metadata[] = {
		"key", "SilentSource",
		"category", "Sample by Sample (experimental)",
		"description", "SilentSource",
		0
	};
	static FactoryRegistrator<ProcessingFactory, SilentSource> reg = metadata;
}

SilentSource::SilentSource():
mOutput("Output", this)
{

}

bool SilentSource::Do()
{
	mOutput.GetData()=0.;
	mOutput.Produce();
	return true;
}


};

