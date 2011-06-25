#ifndef ProcessingFactory_hxx
#define ProcessingFactory_hxx

#include "Processing.hxx"
#include "Factory.hxx"

namespace CLAM
{

class ProcessingFactory : public Factory<Processing>
{
	typedef std::list<std::string> Libs;
	Libs _loadedLibs;
public:
	static ProcessingFactory& GetInstance();
	bool isLibraryLoaded(const std::string & path) const
	{

		for (Libs::const_iterator lib=_loadedLibs.begin(); lib!=_loadedLibs.end(); lib++)
		{
			if (*lib==path) return true;
		}
		return false;
	}
	void setLibraryAsLoaded (const std::string & path)
	{
		_loadedLibs.push_back(path);
	}
	
};

} //namespace CLAM

#endif

