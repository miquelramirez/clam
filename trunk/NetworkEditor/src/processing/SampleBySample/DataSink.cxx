#include "DataSink.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace detail
{
	static FactoryRegistrator<ProcessingFactory, DataSink> regDataSink("DataSink");
}

DataSink::DataSink():
mInput("Input", this)
{

}

bool DataSink::Do()
{
	mInput.Consume();
	return true;
}


};

