#include "DataSilentSource.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace detail
{
	static FactoryRegistrator<ProcessingFactory, SilentSource> regSilentSource("SilentSource");
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

