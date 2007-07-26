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

ProcessingFactory::Keys ProcessingFactory::GetKeys()
{
	return GetKeys("","");
}
ProcessingFactory::Keys ProcessingFactory::GetKeys(const std::string& attribute, const std::string& value)
{
	Keys result;
	Metadata::const_iterator it;
	for(it = _metadata.begin(); it != _metadata.end(); it++)
	{
		if( (attribute == "") )
		{
			result.push_back(it->first);
			continue;
		}
		Pairs attributes = it->second;
		Pairs::const_iterator itAtt;
		for(itAtt = attributes.begin(); itAtt != attributes.end(); itAtt++)
		{
			if( ((*itAtt).attribute == attribute) && ((*itAtt).value == value) )
			{
				result.push_back(it->first);
			}
		}
	}
	return result;
}
ProcessingFactory::Pairs ProcessingFactory::GetPairsFromKey(const std::string& key)
{
	Pairs attributes;
	Metadata::const_iterator it = _metadata.find(key);
	if(it!=_metadata.end())
	{
		attributes = it->second;
	}
	return attributes;
}
/// Example GetSetOfValues("category") could return ["modulators","generators","reverbs"] without repeated items.
ProcessingFactory::Values ProcessingFactory::GetSetOfValues(const std::string& attribute)
{
	std::set<Value> AttributeSet;
	std::set<Value>::const_iterator itSet;
	Values values;
	Metadata::const_iterator it;
	for(it = _metadata.begin(); it != _metadata.end(); it++)
	{
		Pairs attributes = it->second;
		Pairs::const_iterator itAtt;
		for(itAtt = attributes.begin(); itAtt != attributes.end(); itAtt++)
		{
			if((*itAtt).attribute == attribute)
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

ProcessingFactory::Values ProcessingFactory::GetValuesFromAttribute(const std::string& key, const std::string& attribute)
{
	Values values;
	Metadata::const_iterator it = _metadata.find(key);
	if(it != _metadata.end())
	{
		Pairs::const_iterator itAtt;
		for(itAtt = it->second.begin(); itAtt != it->second.end(); itAtt++)
		{
			if((*itAtt).attribute == attribute)
			{
				values.push_back((*itAtt).value);
			}
		}
	}
	return values;
}
void ProcessingFactory::AddAttribute(const std::string& key, const std::string& attribute, const std::string& value)
{
	Pairs mValue;
	Metadata::const_iterator it = _metadata.find(key);
	Pair pair;
	pair.attribute = attribute;
	pair.value = value;
	if(!ExistsKey(key))
	{
		std::cout << "[ProcessingFactory] tryind to add metadata to a non-existing key \"" << key << "\"" << std::endl; 
//		return;  //pau: debugging: add metadata anyway. maybe factory registrator is about to be instantiated.
	}
	if(it == _metadata.end()) // first check if it does not exist
	{
		mValue.push_back(pair);
		_metadata.insert( Metadata::value_type( key, mValue ) );
	} else { // TODO if exists, add a new attribute
		mValue = it->second;
		mValue.push_back(pair);
		_metadata[key] = mValue;
	}
}

} //namespace CLAM

