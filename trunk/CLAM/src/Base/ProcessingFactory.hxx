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
	typedef std::list<Key> Keys;
	typedef std::list<std::string> Values; //pau: TODO poposal: std::list<Pair> Pairs; where Pair is a (attr,value) struct
	typedef std::map<Key, Values> ProcessingAttributes;
	ProcessingAttributes mProcessingAttributes; //pau: TODO don't like the name. how about mMetadata?
	static ProcessingFactory& GetInstance();
	void AddAttribute(const std::string& key, const std::string& attribute, const std::string& value);
	Keys GetListOfKeys(const std::string& attribute, const std::string& value);
	Values GetValuesFor(const std::string& key); //TODO name: UniqueValues? DifferentValues? SetOfValues. pau: I like GetSetOfValues()
	void ProcessingClear(); //pau: TODO remove it till need it
	std::size_t ProcessingCount(); //pau: idem
};

} //namespace CLAM
#endif

