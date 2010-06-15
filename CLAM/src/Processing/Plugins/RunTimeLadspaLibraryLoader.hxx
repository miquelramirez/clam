#ifndef RunTimeLadspaLibraryLoader_hxx
#define RunTimeLadspaLibraryLoader_hxx

#include <dirent.h>
//#include "RunTimeLibraryLoader.hxx"

#include "ProcessingFactory.hxx" 
#include "LadspaWrapperCreator.hxx"
#include <ladspa.h>


class RunTimeLadspaLibraryLoader : public RunTimeLibraryLoader
{
protected:

	virtual const bool needReleaseHandlerOnReload() const { return false;}

	void SetupLibrary(void* handle, const std::string & pluginFullFilename) const;

	const char ** standardPaths() const
	{ 
		static const char * result[] = 
		{
			"/usr/local/lib/ladspa",
			"/usr/lib/ladspa",
			0
		};
		return result;
	}

	const char * homePath() const { return  "/.ladspa"; }
	const char * pathEnvironmentVar() const { return  "LADSPA_PATH"; }
	const char * libraryType() const { return  "LADSPA"; }
};


#endif // RunTimeLadspaLibraryLoader_hxx


