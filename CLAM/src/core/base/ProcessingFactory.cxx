#include "ProcessingFactory.hxx"
#include <string>
#include <set>

namespace CLAM
{

ProcessingFactory& ProcessingFactory::GetInstance()	
{
	static ProcessingFactory theInstance;
	return theInstance;
}

} //namespace CLAM

