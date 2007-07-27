#ifndef ProcessingFactory_hxx
#define ProcessingFactory_hxx

#include "Processing.hxx"
#include "Factory.hxx"

namespace CLAM
{

class ProcessingFactory : public Factory<Processing>
{
public:
	typedef std::string Attribute;
	typedef std::string Value;
	struct Pair 
	{
		Attribute attribute;
		Value value;
	};
	typedef RegistryKey Key;
	typedef std::list<Key> Keys;
	typedef std::list<std::string> Values;
	typedef std::list<Pair> Pairs; 
	typedef std::map<Key, Pairs> Metadata;
//TODO use Attribute and Value
	void AddAttribute(const std::string& key, const std::string& attribute, const std::string& value);
	/// Get all keys that have attribute==value in its metadata.
	Keys GetKeys(const std::string& attribute, const std::string& value);
	/// Get all keys in the factory
	Keys GetKeys();
	/// Get the set of all values present for a given metadata attribute.
	Values GetSetOfValues(const std::string& attribute); 
	/// Return all the metadata available for a product key
	Pairs GetPairsFromKey(const std::string& key); 
	/// Return the list of values for a metadata attribute for a product key.
	Values GetValuesFromAttribute(const std::string& key, const std::string& attribute);
	/// Return the value for a metadata attribute of product key.
	Value GetValueFromAttribute(const std::string& key, const std::string& attribute)
	{
		return GetValuesFromAttribute(key,attribute).front();
	}

private:
	Metadata _metadata;


public:
	static ProcessingFactory& GetInstance();
};

} //namespace CLAM
#endif

