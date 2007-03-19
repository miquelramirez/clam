#include "LadspaPluginsExplorer.hxx"
#include <iostream> //TODO move
#include <ladspa.h> //TODO move to extracted cxx
#include <dlfcn.h> //TODO move as well
#include <dirent.h> //TODO move
#include <sys/types.h>
#include <sstream>

namespace CLAM
{

LadspaPlugins LadspaPluginsExplorer::GetList()
{
	LadspaPlugins result;
	char* path = getenv("LADSPA_PATH");
	std::string ladspaPath = path ? path : "";
	if(ladspaPath == "")
	{
	        ladspaPath = "/usr/lib/ladspa";
	}
	DIR* ladspaDir = opendir(ladspaPath.c_str());
	if (!ladspaDir)
	{
		std::cout << "EXPLORER warning: could not open ladspa dir: " << ladspaPath << std::endl;
		return result; //TODO something different when we'll be exploring several paths
	}
	struct dirent * dirEntry;
	while( (dirEntry = readdir(ladspaDir)) )
	{
		std::string pluginFilename(dirEntry->d_name);
		if(pluginFilename == "." || pluginFilename == "..")
			continue;
		LADSPA_Descriptor_Function descriptorTable = 0;
		std::string pluginFullFilename(ladspaPath + std::string("/") + pluginFilename);
		void* handle = dlopen( pluginFullFilename.c_str(), RTLD_NOW);
		descriptorTable = (LADSPA_Descriptor_Function)dlsym(handle, "ladspa_descriptor");
		if (!descriptorTable)
		{
			std::cout << "EXPLORER warning: trying to open non ladspa plugin: " << pluginFullFilename << std::endl;
			continue;
		}
		//std::cout << "EXPLORER opened plugin: " << pluginFullFilename << std::endl;
		for (unsigned long i=0; descriptorTable(i); i++)
		{
			LadspaPlugin plugin;
			LADSPA_Descriptor* descriptor = (LADSPA_Descriptor*)descriptorTable(i);
			plugin.index = i;
			plugin.description = descriptor->Name;
			plugin.libraryFileName = pluginFullFilename;
			plugin.label = descriptor->Label;
			std::ostringstream oss;
			oss << plugin.label << i;
			plugin.factoryID = oss.str();
			result.push_back(plugin);
		}
	}
	closedir(ladspaDir);

	return result;
}

}
