#include "MyProcessing.hxx"
#include <CLAM/ProcessingFactory.hxx>

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MyProcessing>
	myProcessingRegistrator("MyProcessing");
/*	myRegistrator("MyProcessing") = 
	{
		"key", "MyProcessing", 
		"category", "CLAM",
		0
	}
*/
class A
{
public:
	A()
	{
		CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
		factory.AddAttribute("MyProcessing", "category", "CLAM");
		factory.AddAttribute("MyProcessing", "description", "My super brand new processing");
	}
};
static A a;
