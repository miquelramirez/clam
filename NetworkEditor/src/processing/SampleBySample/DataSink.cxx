#include "DataSink.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace detail
{
	static const char * metadata[] = {
		"key", "DataSink",
		"category", "Sample by Sample (experimental)",
		"description", "DataSink",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, DataSink> regDataSink("DataSink");
	static FactoryRegistrator<ProcessingFactory, DataSink> reg = metadata;
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

