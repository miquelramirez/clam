#ifndef RunTimeFaustLibraryLoader_hxx
#define RunTimeFaustLibraryLoader_hxx

#include <dirent.h>
#include "RunTimeLibraryLoader.hxx"
#include "LadspaWrapperCreator.hxx"
#include <ladspa.h>


class RunTimeFaustLibraryLoader : public RunTimeLibraryLoader
{

public:
	virtual void Load() 
	{
		std::string examplesDir = CompletePathFor("examples/ladspadir");
		LoadLibrariesFromPath(examplesDir);
		RunTimeLibraryLoader::Load(); 
	}
	void LoadPlugin(const std::string & pluginFullPath)
	{
		LoadLibrariesFromPath(pluginFullPath);
	}

	// overload as workaround of Load() overload and path issues
	virtual std::list<std::string> GetUsedLibraries()
	{
		std::list<std::string> loadedLibraries=LoadedLibraries();
		LoadedLibraries().clear();
		return loadedLibraries;
	}
protected:

	virtual const bool needReleaseHandlerOnReload() const {return false; }
	void SetupLibrary(void* handle, const std::string & pluginFullFilename) const;

	const char ** standardPaths() const
	{ 
		static const char * result[] = 
		{
/*			"/usr/share/doc/faust",
			"/usr/local/share/doc/faust",*/
			0
		};
		return result;
	}

	const char * homePath() const { return  "/.faust"; }
	const char * pathEnvironmentVar() const { return  "CLAM_FAUST_PATH"; }
	const char * libraryType() const { return  "LADSPA"; }
private:
	static std::list<std::string> & LoadedLibraries()
	{
		static std::list<std::string> sLoadedLibraries;
		return sLoadedLibraries;
	}
};

#endif // RunTimeFaustLibraryLoader_hxx


