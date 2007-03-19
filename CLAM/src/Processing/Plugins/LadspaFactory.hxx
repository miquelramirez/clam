#ifndef __Factory_LADSPA_hxx
#define __Factory_LADSPA_hxx

#include <map>
#include <string>
#include "LadspaWrapper.hxx"  // CLAM
#include "LadspaWrapperCreator.hxx" // CLAM

namespace CLAM {

class LadspaFactory
{
public:
	static LadspaFactory& GetInstance()
	{
		static LadspaFactory theInstance;
		return theInstance;
	}
	/*class Processing //TODO delete. it should be CLAM::Processing 
	{
	public:
		virtual ~Processing() {};
	};*/

	/*class LadspaWrapper : public Processing //TODO move to CLAM/src/Processing/Plugins (in namespace CLAM)
	{
	private:
		std::string _libraryFileName;
		unsigned _index;
	public:
		LadspaWrapper( const std::string& libraryFileName, unsigned index ) 
			: _libraryFileName( libraryFileName )
			, _index( index )
		{
		}
		const std::string& LibraryFileName() 
		{ 
			return _libraryFileName; 
		}
		unsigned Index()
		{
			return _index;
		}
	};

	class LadspaWrapperCreator  //TODO move to CLAM/src/Processing/Plugins
	{
	private:
		std::string _libraryFileName;
		unsigned _index;
	public:
		LadspaWrapperCreator( const std::string& libraryFileName, unsigned index ) 
			: _libraryFileName( libraryFileName )
			, _index( index )
		{
		}
		virtual Processing* Create()
		{
			return new CLAM::LadspaWrapper(_libraryFileName, _index);
		}
		virtual ~LadspaWrapperCreator() {};
	};*/
public:
	LadspaFactory()
	{
	}

	class NonExistingKeyError
	{
	};

	class EmptyCreatorListError
	{
	};

	Processing* CreateSafe(const std::string& key)
	{
		if( GetCreator( key ) == NULL )
		{
			throw NonExistingKeyError();
		}

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
			throw EmptyCreatorListError();
		}

		LadspaCreatorMap::const_iterator it = _creators.find( key );
		if(it == _creators.end())
			return NULL;
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
private:
	typedef std::map< std::string, LadspaWrapperCreator* > LadspaCreatorMap;
	LadspaCreatorMap _creators;
};

} //namespace

#endif // __Factory_LADSPA_hxx
