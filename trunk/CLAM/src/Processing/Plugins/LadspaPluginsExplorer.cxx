#include "LadspaPluginsExplorer.hxx"
#include <iostream> //TODO move
#include <ladspa.h> //TODO move to extracted cxx
#include <dlfcn.h> //TODO move as well
#include <dirent.h> //TODO move
#include <sys/types.h>
#include <sstream>
#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/LadspaWrapperCreator.hxx>

namespace CLAM
{

void LadspaPluginsExplorer::ExploreStandardPaths()
{
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
		ExplorePath(dir);
	}
}

void LadspaPluginsExplorer::ExplorePath(const std::string & dir)
{
	//std::cout << "[LADSPA] Reading dir: " << dir << std::endl;
	DIR* ladspaDir = opendir(dir.c_str());
	if (!ladspaDir)
	{
		std::cout << "[LADSPA] warning: could not open ladspa dir: " << dir << std::endl;
		closedir(ladspaDir);
		return;
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
		ProcessingFactory& factory = ProcessingFactory::GetInstance();
		for (unsigned long i=0; descriptorTable(i); i++)
		{
			LADSPA_Descriptor* descriptor = (LADSPA_Descriptor*)descriptorTable(i);
			std::ostringstream oss;
			oss << descriptor->Label << i;
			factory.AddCreatorWarningRepetitions(oss.str(), 
					new LadspaWrapperCreator(pluginFullFilename, 
						i, 
						oss.str()));
			factory.AddAttribute(oss.str(), "category", "LADSPA");
			factory.AddAttribute(oss.str(), "description", descriptor->Name);
			//std::cout << "[LADSPA] added \"" << plugin.factoryID << "\" to the Factory" << std::endl;
		}
	}
	closedir(ladspaDir);
}

}
