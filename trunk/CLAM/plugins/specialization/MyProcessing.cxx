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
	class myProcessingRegistratorMetadata
	{
	public:
		myProcessingRegistratorMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("MyProcessing", "category", "CLAM");
			factory.AddAttribute("MyProcessing", "description", "MyProcessing: just an example");
		}
	};
	static myProcessingRegistratorMetadata dummy;
