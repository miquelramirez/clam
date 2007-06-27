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
	typedef std::list<std::string> Values;
	static ProcessingFactory& GetInstance();
	Keys GetListOfKeys(const std::string& attribute, const std::string& value);
	Values GetValuesFor(const std::string&); //TODO Unique? Different values?
};

} //namespace CLAM
#endif

