#include "ProcessingFactory.hxx"

namespace CLAM
{

ProcessingFactory& ProcessingFactory::GetInstance()	
{
	static ProcessingFactory theInstance;
	return theInstance;
}

} //namespace CLAM

