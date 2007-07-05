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
	ProcessingAttributes::const_iterator it;
	for(it = mProcessingAttributes.begin(); it != mProcessingAttributes.end(); it++)
	{
		Values pAttributes = it->second;
		Values::const_iterator itAtt;
		for(itAtt = pAttributes.begin(); itAtt != pAttributes.end(); itAtt++)
		{
			std::string _attribute, _value;
			if((*itAtt).find(":") != std::string::npos) // check for integrity of string
			{
				_attribute = 	(*itAtt).substr(0, (*itAtt).find(":"));
				_value = (*itAtt).substr((*itAtt).find(":")+1);
				if( (_attribute == attribute) && (_value == value))
				{
					result.push_back(it->first);
				}
			}
		}
	}
	return result;
}
ProcessingFactory::Values ProcessingFactory::GetValuesFor(const std::string& key)
{
	Values values;
	ProcessingAttributes::const_iterator it = mProcessingAttributes.find(key);
	if(it!=mProcessingAttributes.end())
	{
		values = it->second;
	}
	return values;
}
void ProcessingFactory::AddAttribute(const std::string& key, const std::string& attribute, const std::string& value)
{
	Values mValue;
	ProcessingAttributes::const_iterator it = mProcessingAttributes.find(key);
	// TODO check if the factory has the key
	if(it == mProcessingAttributes.end()) // first check if it does not exist
	{
		mValue.push_back(std::string(attribute + ":" + value));
		mProcessingAttributes.insert( ProcessingAttributes::value_type( key, mValue ) );
	} // if exists, add a new attribute
}

void ProcessingFactory::ProcessingClear()
{
	mProcessingAttributes.clear();
}

std::size_t ProcessingFactory::ProcessingCount()
{
	return mProcessingAttributes.size();
}
} //namespace CLAM

