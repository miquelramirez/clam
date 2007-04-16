#ifndef __Factory_LADSPA_hxx
#define __Factory_LADSPA_hxx

#include <map>
#include <string>
#include "ErrFactory.hxx"
#include "LadspaWrapper.hxx"
#include "LadspaWrapperCreator.hxx"

namespace CLAM {

class LadspaFactory
{
public:
	static LadspaFactory& GetInstance()
	{
		static LadspaFactory theInstance;
		return theInstance;
	}
	LadspaFactory()
	{
	}
	Processing* CreateSafe(const std::string& key)
	{
		return GetCreator( key )->Create();
	}
	void AddCreator( const std::string& key, LadspaWrapperCreator* creator )
	{
		_creators[key] = creator;
	}
	LadspaWrapperCreator* GetCreator( const std::string& key )
	{
		if(_creators.begin() == _creators.end())
		{
			throw ErrFactory("Trying to create a product from an empty factory.");
		}

		LadspaCreatorMap::const_iterator it = _creators.find( key );
		if(it == _creators.end())
			throw ErrFactory("Non existing Key in the factory");
		return it->second;
	}
	std::string RegisteredKeys()
	{
		std::string result;
		LadspaCreatorMap::const_iterator it;
		for ( it = _creators.begin(); it != _creators.end(); it++ )
		{
			result += it->first +" ";
		}
		return result;
	}
	void Clear()
	{
		_creators.clear();
	}
private:
	typedef std::map< std::string, LadspaWrapperCreator* > LadspaCreatorMap;
	LadspaCreatorMap _creators;
};

} //namespace

#endif // __Factory_LADSPA_hxx

