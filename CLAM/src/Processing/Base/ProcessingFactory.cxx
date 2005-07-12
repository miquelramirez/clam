#include "ProcessingFactory.hxx"

namespace CLAM
{

Factory<Processing>& ProcessingFactory::GetInstance()
{
	static ProcessingFactory theInstance;
	return theInstance;
}
	
	
ProcessingFactory::~ProcessingFactory()
{
}

}
