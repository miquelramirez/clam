#ifndef RunTimeLibraryLoader_hxx
#define RunTimeLibraryLoader_hxx

#include <string>
#include <vector>
#include <map>
#include <list>


class RunTimeLibraryLoader
{
public:

	virtual ~RunTimeLibraryLoader() {}
	void Load() const;
	void ReLoad();

	const std::string CompletePathFor(const std::string & subpathAndName) const; // if subpathAndName exists on environment paths, returns full path

	// static methods for dynamic libraries handles
	static void * FullyLoadLibrary(const std::string & libraryPath);
	static void * LazyLoadLibrary(const std::string & libraryPath);
	static bool ReleaseLibraryHandler(void * handle, const std::string pluginFullFilename="");
	static const std::string LibraryLoadError();
	static const std::string FileOfSymbol (void * symbolAddress);
	static void * GetSymbol(void * libraryHandler, const std::string & symbolName);

protected:
	void LoadLibrariesFromPath(const std::string & path) const;
	std::vector<std::string> SplitPathVariable(const std::string & pathVariable) const;
	void * GetLibraryHandler(const std::string & libraryPath) const;
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
	virtual const bool needReleaseHandlerOnReload() const =0;
	virtual void SetupLibrary( void* handle, const std::string & pluginFullFilename ) const {}
	const std::string getPathFromFullFileName(const std::string & fullFileNameConst) const
	{
		std::string fullFileName = fullFileNameConst;
		return fullFileName.substr(0,fullFileName.rfind("/"));
	}
	bool IsOnPath(const std::string & path) const;
	virtual std::list<std::string> GetUsedLibraries();

private:
	const std::string GetPaths() const;
};

#endif //RunTimeLibraryLoader_hxx

