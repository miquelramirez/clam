#include "ProcessingFactory.hxx"
#include <string>

//#include <iostream> //TODO only when debugging
namespace CLAM
{

ProcessingFactory& ProcessingFactory::GetInstance()	
{
	static ProcessingFactory theInstance;
//	std::cout << "ProcessingFactory Singleton accessed: "<< (void *) &theInstance << std::endl;
	return theInstance;
}

ProcessingFactory::Keys ProcessingFactory::GetListOfKeys(const std::string& attribute, const std::string& value)
{
	Keys result;
	result.push_back("lala");
	return result;
}
ProcessingFactory::Values ProcessingFactory::GetValuesFor(const std::string&)
{
	Values result;
	return result;
}
} //namespace CLAM

