#ifndef ProcessingFactory_hxx
#define ProcessingFactory_hxx

#include "Processing.hxx"
#include "Factory.hxx"

namespace CLAM
{

class ProcessingFactory : public Factory<Processing>
{
public:
	typedef RegistryKey Key;
	typedef std::string Value;
	typedef struct sValue {
		std::string name;
		Value value;
	} Attribute;
	typedef std::list<Key> Keys;
	typedef std::list<Value> Values;
	typedef std::list<Attribute> Attributes;
	typedef std::map<Key, Attributes> ProcessingAttributes;
	ProcessingAttributes mProcessingAttributes;
	static ProcessingFactory& GetInstance();
	void AddAttribute(const std::string& key, const std::string& attribute, const std::string& value);
	Keys GetListOfKeys(const std::string& attribute, const std::string& value);
	Values GetValuesFor(const std::string& attribute); 
	Attributes GetValuesOf(const std::string& key); 
	Values GetValuesFrom(const std::string& key, const std::string& attribute);
	void ProcessingClear();
	std::size_t ProcessingCount();
};

} //namespace CLAM
#endif

