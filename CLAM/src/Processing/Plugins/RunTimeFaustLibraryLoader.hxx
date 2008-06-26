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

#include "ProcessingFactory.hxx"
#include "LadspaWrapperCreator.hxx"
#include <ladspa.h>


class RunTimeFaustLibraryLoader : public RunTimeLibraryLoader
{

protected:

	void SetupLibrary(void* handle, const std::string & pluginFullFilename) const
	{
		LADSPA_Descriptor_Function descriptorTable = 0;
		descriptorTable = (LADSPA_Descriptor_Function)dlsym(handle, "ladspa_descriptor");
		if (!descriptorTable)
		{
			std::cout << "[FAUST-LADSPA Plugin] Warning: trying to open non ladspa plugin: " << pluginFullFilename << std::endl;
			return;
		}
		std::cout << "[FAUST-LADSPA] \topened plugin: " << pluginFullFilename << std::endl;

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
			std::string pluginName=descriptor->Name;
			factory.AddAttribute(oss.str(), "category", "FAUST");
			factory.AddAttribute(oss.str(), "description", pluginName);
			std::string svgFileDir = CompletePathFor("examples/" + pluginName + ".dsp-svg/process.svg");
			if (svgFileDir != "")
			{
				factory.AddAttribute(oss.str(), "svg_diagram", svgFileDir);
				std::cout << "[FAUST-LADSPA] \tusing SVG diagram: " << svgFileDir << std::endl;
			}
			else
				factory.AddAttribute(oss.str(), "svg_diagram", ":icons/images/faustlogo.svg");

			if (!factory.AttributeExists(oss.str(), "icon"))
				factory.AddAttribute(oss.str(), "icon", "faustlogo.svg");
		}
	}

	const char ** standardPaths() const
	{ 
		static const char * result[] = 
		{
			"/usr/share/doc/faust",
			"/usr/local/share/doc/faust",
			0
		};
		return result;
	}

	const char * homePath() const { return  "/.faust"; }
	const char * pathEnvironmentVar() const { return  "FAUST_PATH"; }
	const char * libraryType() const { return  "LADSPA"; }
};

#endif // RunTimeFaustLibraryLoader_hxx


