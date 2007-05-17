#include "DataSilentSource.hxx"
#include "Factory.hxx"

namespace CLAM
{

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

typedef Factory<Processing> ProcessingFactory;

namespace detail
{
	static ProcessingFactory::Registrator<SilentSource> regtSilentSource( "SilentSource" );
}

};

