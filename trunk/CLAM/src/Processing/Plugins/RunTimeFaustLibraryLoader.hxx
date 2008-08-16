#ifndef RunTimeFaustLibraryLoader_hxx
#define RunTimeFaustLibraryLoader_hxx

#ifdef WIN32
#include <windows.h>
#undef GetClassName
#else
#include <dlfcn.h>
#endif
#include <dirent.h>
#include "RunTimeLibraryLoader.hxx"

#include "LadspaWrapperCreator.hxx"
#include <ladspa.h>


class RunTimeFaustLibraryLoader : public RunTimeLibraryLoader
{

public:
	virtual void Load() const
	{
		std::string examplesDir = CompletePathFor("examples/ladspadir");
		LoadLibrariesFromPath(examplesDir);
		RunTimeLibraryLoader::Load(); // needed??
	}
	void LoadPlugin(const std::string & pluginFullPath) const
	{
		LoadLibrariesFromPath(pluginFullPath);
	}

	// overload as workaround of Load() overload and path issues
	virtual const std::list<std::string> GetUsedLibraries()
	{
		const std::list<std::string> loadedLibraries=LoadedLibraries();
		LoadedLibraries().clear();
		return loadedLibraries;
	}
protected:

	virtual const bool needReleaseHandlerOnReload() const {return false; }
	void SetupLibrary(void* handle, const std::string & pluginFullFilename) const
	{
		LADSPA_Descriptor_Function descriptorTable = 0;
		descriptorTable = (LADSPA_Descriptor_Function)dlsym(handle, "ladspa_descriptor");
		if (!descriptorTable)
		{
//			std::cout << "[FAUST-LADSPA Plugin] Warning: trying to open non ladspa plugin: " << pluginFullFilename << std::endl;
			return;
		}

		LoadedLibraries().push_back(pluginFullFilename);
		CLAM::ProcessingFactory& factory = CLAM::ProcessingFactory::GetInstance();
		for (unsigned long i=0; descriptorTable(i); i++)
		{
			LADSPA_Descriptor* descriptor = (LADSPA_Descriptor*)descriptorTable(i);
			std::ostringstream oss;
			oss << descriptor->Label << i;
			factory.AddCreatorWarningRepetitions(oss.str(), 
					new CLAM::LadspaWrapperCreator(pluginFullFilename, 
						i, 
						oss.str()));
			factory.AddAttribute(oss.str(), "category", "FAUST");
			factory.AddAttribute(oss.str(), "description", descriptor->Name);
			factory.AddAttribute(oss.str(), "library", pluginFullFilename);

			std::string pluginName=descriptor->Label;
			const std::string diagramMainSufix=".dsp-svg/process.svg";
			std::string svgFileDir = CompletePathFor( "examples/" + pluginName + diagramMainSufix);
			if (svgFileDir != "")
			{
				factory.AddAttribute(oss.str(), "faust_diagram", svgFileDir);
			}
			if (!factory.AttributeExists(oss.str(), "embedded_svg"))
				factory.AddAttribute(oss.str(), "embedded_svg", ":icons/images/faustlogo.svg");
			if (!factory.AttributeExists(oss.str(), "icon"))
				factory.AddAttribute(oss.str(), "icon", "faustlogo.svg");
			std::string sourcePath=CompletePathFor( "examples/"+pluginName+".dsp");
			if (sourcePath != "")
				factory.AddAttribute(oss.str(), "faust_source_file", sourcePath);
		}
		if (ReleaseLibraryHandler(handle, pluginFullFilename))
		{
			std::cout<<"[FAUST-LADSPA] error unloading library handle of: " << pluginFullFilename<<std::endl;
			std::cout<<LibraryLoadError()<<std::endl;
		}
	}

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


