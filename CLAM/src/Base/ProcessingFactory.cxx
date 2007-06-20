#include "ProcessingFactory.hxx"

#include <iostream> //TODO remove
namespace CLAM
{

ProcessingFactory& ProcessingFactory::GetInstance()	
{
	static ProcessingFactory theInstance;
	std::cout << "ProcessingFactory Singleton accessed: "<< (void *) &theInstance << std::endl;
	return theInstance;
}

} //namespace CLAM

