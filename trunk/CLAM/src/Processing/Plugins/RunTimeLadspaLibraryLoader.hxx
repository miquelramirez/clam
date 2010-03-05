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
	void SetupLibrary(void* handle, const std::string & pluginFullFilename) const
	{
		LADSPA_Descriptor_Function descriptorTable = 0;
		descriptorTable = (LADSPA_Descriptor_Function)GetSymbol(handle, "ladspa_descriptor");
		if (!descriptorTable)
		{
			std::cout << "[LADSPA Plugin] Warning: trying to open non ladspa plugin: " << pluginFullFilename << std::endl;
			return;
		}
		//std::cout << "[LADSPA] \topened plugin: " << pluginFullFilename << std::endl;
		CLAM::ProcessingFactory& factory = CLAM::ProcessingFactory::GetInstance();
		for (unsigned long i=0; descriptorTable(i); i++)
		{
			LADSPA_Descriptor* descriptor = (LADSPA_Descriptor*)descriptorTable(i);
			const char* id = descriptor->Label;
			factory.AddCreatorWarningRepetitions(id,
					new CLAM::LadspaWrapperCreator(pluginFullFilename,
						i,
						id));
			factory.AddAttribute(id, "category", "LADSPA");
			factory.AddAttribute(id, "description", descriptor->Name);
			factory.AddAttribute(id, "library", pluginFullFilename);

			std::ostringstream oss;
			oss << descriptor->Label << "_buffer" << i;
			std::string id2=oss.str();
			factory.AddCreatorWarningRepetitions(id2,
					new CLAM::LadspaWrapperBufferCreator(pluginFullFilename,
						i,
						id2));
			factory.AddAttribute(id2, "category", "LADSPA_BUFFER");
			factory.AddAttribute(id2, "description", descriptor->Name);
			factory.AddAttribute(id2, "library", pluginFullFilename);
			//std::cout << "[LADSPA] added \"" << plugin.factoryID << "\" to the Factory" << std::endl;
		}
		if (ReleaseLibraryHandler(handle, pluginFullFilename))
		{
			std::cout<<"[LADSPA Plugin] error unloading library handle of: " << pluginFullFilename<<std::endl;
			std::cout<<LibraryLoadError()<<std::endl;
		}
	}

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


