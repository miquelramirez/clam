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
	std::string ladspaPath;
	if(!path)
	{
		std::cout << "[LADSPA] LADSPA_PATH empty! Using defaults." << std::endl;
		ladspaPath = "/usr/lib/ladspa:/usr/local/lib/ladspa:";
		ladspaPath += getenv("HOME"); // TODO: Could be NULL in theory
		ladspaPath += "/.ladspa";
	} 
	else 
	{
		ladspaPath = path;
	}
	std::cout << "[LADSPA] Parsed LADSPA library path: " << ladspaPath << std::endl;
	while (!ladspaPath.empty())
	{
		//std::cout << "[LADSPA] ladspaPath = " << ladspaPath << " size = " << ladspaPath.size() << std::endl;
		std::string dir = ladspaPath.substr(0, ladspaPath.find(":"));
		if (ladspaPath.find(":") != std::string::npos)
		{
			ladspaPath = ladspaPath.substr(ladspaPath.find(":")+1);
		} 
		else 
		{
			ladspaPath = "";
		}

		std::cout << "[LADSPA] Reading dir: " << dir << std::endl;
		DIR* ladspaDir = opendir(dir.c_str());
		if (!ladspaDir)
		{
			std::cout << "[LADSPA] warning: could not open ladspa dir: " << ladspaPath << std::endl;
			continue;
		}
		struct dirent * dirEntry;
		while ( (dirEntry = readdir(ladspaDir)) )
		{
			std::string pluginFilename(dirEntry->d_name);
			if(pluginFilename == "." || pluginFilename == "..")
				continue;
			LADSPA_Descriptor_Function descriptorTable = 0;
			std::string pluginFullFilename(dir + std::string("/") + pluginFilename);
			void* handle = dlopen( pluginFullFilename.c_str(), RTLD_LAZY);
			descriptorTable = (LADSPA_Descriptor_Function)dlsym(handle, "ladspa_descriptor");
			if (!descriptorTable)
			{
				std::cout << "[LADSPA] warning: trying to open non ladspa plugin: " << pluginFullFilename << std::endl;
				continue;
			}
			//std::cout << "[LADSPA] \topened plugin: " << pluginFullFilename << std::endl;
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
	}

	return result;
}

}

