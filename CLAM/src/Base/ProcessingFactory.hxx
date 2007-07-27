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
	Keys GetKeys(const std::string& attribute, const std::string& value);
	Keys GetKeys();
	Values GetSetOfValues(const std::string& attribute); 
	Pairs GetPairsFromKey(const std::string& key); 
	Values GetValuesFromAttribute(const std::string& key, const std::string& attribute);

private:
	Metadata _metadata;


public:
	static ProcessingFactory& GetInstance();
};

} //namespace CLAM
#endif

