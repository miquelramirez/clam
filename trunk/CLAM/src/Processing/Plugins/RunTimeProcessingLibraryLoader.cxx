#include "RunTimeLibraryLoader.hxx"

class RunTimeProcessingLibraryLoader : public RunTimeLibraryLoader
{

protected:

	const bool needReleaseHandlerOnReload() const { return true; } 
	const char ** standardPaths() const
	{ 
		static const char * result[] = 
		{
			"/usr/local/lib/clam",
			"/usr/lib/clam",
			"/opt/lib/clam",
			"c:/Program Files/CLAM/NetworkEditor/lib/clam",
			0
		};
		return result;
	}

	const char * homePath() const { return  "/.clam/plugins"; }
	const char * pathEnvironmentVar() const { return  "CLAM_PLUGIN_PATH"; }
	const char * libraryType() const { return  "CLAM"; }
};

class AutomaticRunTimeProcessingLibraryLoader
{
public:
	AutomaticRunTimeProcessingLibraryLoader()
	{
		RunTimeProcessingLibraryLoader loader;
		loader.Load();
	}
};

static AutomaticRunTimeProcessingLibraryLoader dllLoader;

