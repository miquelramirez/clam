#ifndef RunTimeLibraryLoader_hxx
#define RunTimeLibraryLoader_hxx

#include <string>
#include <vector>

class RunTimeLibraryLoader
{
public:
	virtual ~RunTimeLibraryLoader() {}
	void Load() const;

protected:
	void LoadLibrariesFromPath(const std::string & path) const;


	void * FullyLoadLibrary(const std::string & libraryPath) const;
	std::string LibraryLoadError() const;
	std::vector<std::string> SplitPathVariable(const std::string & pathVariable) const;
	const char * pathSeparator() const
	{
		return 
			#ifdef WIN32
				";";
			#else
				":";
			#endif
	}
	// to implement by subclasses (ex. Ladspa, CLAM processings, etc)
	virtual const char ** standardPaths() const = 0;
	virtual const char * homePath() const = 0;
	virtual const char * pathEnvironmentVar() const = 0;
	virtual const char * libraryType() const = 0;
	virtual void SetupLibrary( void* handle, const std::string& pluginFullFilename ) const {}
};

#endif //RunTimeLibraryLoader_hxx

