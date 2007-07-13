#include "ProcessingFactory.hxx"
#include <string>
#include <set>

//#include <iostream> //TODO only when debugging
namespace CLAM
{

ProcessingFactory& ProcessingFactory::GetInstance()	
{
	static ProcessingFactory theInstance;
//	std::cout << "ProcessingFactory Singleton accessed: "<< (void *) &theInstance << std::endl;
	return theInstance;
}

ProcessingFactory::Keys ProcessingFactory::GetListOfKeys()
{
	return GetListOfKeys("","");
}
ProcessingFactory::Keys ProcessingFactory::GetListOfKeys(const std::string& attribute, const std::string& value)
{
	Keys result;
	ProcessingAttributes::const_iterator it;
	for(it = mProcessingAttributes.begin(); it != mProcessingAttributes.end(); it++)
	{
		if( (attribute == "") )
		{
			result.push_back(it->first);
			continue;
		}
		Attributes attributes = it->second;
		Attributes::const_iterator itAtt;
		for(itAtt = attributes.begin(); itAtt != attributes.end(); itAtt++)
		{
			if( ((*itAtt).name == attribute) && ((*itAtt).value == value) )
			{
				result.push_back(it->first);
			}
		}
	}
	return result;
}
ProcessingFactory::Attributes ProcessingFactory::GetValuesOf(const std::string& key)
{
	Attributes attributes;
	ProcessingAttributes::const_iterator it = mProcessingAttributes.find(key);
	if(it!=mProcessingAttributes.end())
	{
		attributes = it->second;
	}
	return attributes;
}

ProcessingFactory::Values ProcessingFactory::GetValuesFor(const std::string& attribute)
{
	std::set<Value> AttributeSet;
	std::set<Value>::const_iterator itSet;
	Values values;
	ProcessingAttributes::const_iterator it;
	for(it = mProcessingAttributes.begin(); it != mProcessingAttributes.end(); it++)
	{
		Attributes attributes = it->second;
		Attributes::const_iterator itAtt;
		for(itAtt = attributes.begin(); itAtt != attributes.end(); itAtt++)
		{
			if((*itAtt).name == attribute)
			{
				itSet = AttributeSet.find((*itAtt).value);
				if(itSet == AttributeSet.end())
				{
					AttributeSet.insert((*itAtt).value);
				}
			}
		}
	}
	// keep using the ProcessingFactory::Values
	for(itSet = AttributeSet.begin(); itSet != AttributeSet.end(); itSet++)
	{
		values.push_back(*itSet);
	}
	return values;
}

ProcessingFactory::Values ProcessingFactory::GetValuesFrom(const std::string& key, const std::string& attribute)
{
	Values values;
	ProcessingAttributes::const_iterator it = mProcessingAttributes.find(key);
	if(it != mProcessingAttributes.end())
	{
		Attributes::const_iterator itAtt;
		for(itAtt = it->second.begin(); itAtt != it->second.end(); itAtt++)
		{
			if((*itAtt).name == attribute)
			{
				values.push_back((*itAtt).value);
			}
		}
	}
	return values;
}
void ProcessingFactory::AddAttribute(const std::string& key, const std::string& attribute, const std::string& value)
{
	Attributes mValue;
	ProcessingAttributes::const_iterator it = mProcessingAttributes.find(key);
	Attribute att;
	att.name = attribute;
	att.value = value;
	// TODO check if the factory has the key
	if(!ExistsKey(key))
	{
		std::cout << "[ProcessingFactory] tryind to add metadata to a non-existing key \"" << key << "\"" << std::endl; 
		return;
	}
	if(it == mProcessingAttributes.end()) // first check if it does not exist
	{
		mValue.push_back(att);
		mProcessingAttributes.insert( ProcessingAttributes::value_type( key, mValue ) );
	} else { // TODO if exists, add a new attribute
		mValue = it->second;
		mValue.push_back(att);
		mProcessingAttributes[key] = mValue;
	}
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

