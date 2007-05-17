#include "DataSink.hxx"
#include "Factory.hxx"

namespace CLAM
{

DataSink::DataSink():
mInput("Input", this)
{

}

bool DataSink::Do()
{
	mInput.Consume();
	return true;
}

typedef Factory<Processing> ProcessingFactory;

namespace detail
{
	static ProcessingFactory::Registrator<DataSink> regtDataSink( "DataSink" );
}

};

