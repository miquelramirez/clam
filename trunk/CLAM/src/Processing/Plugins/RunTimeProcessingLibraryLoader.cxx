#include <dlfcn.h>
#include <dirent.h>
#include <iostream>
#include <string>

class RunTimeProcessingLibraryLoader
{
public:
	RunTimeProcessingLibraryLoader(const std::string & directory="libs")
	{
		DIR* dir = opendir(directory.c_str());
		if (!dir)
		{
			std::cout << "[Plugins] warning: could not open CLAM plugins dir: " << directory << std::endl;
			return;
		}
		struct dirent * dirEntry;
		while ( (dirEntry = readdir(dir)) )
		{
			std::string pluginFilename(dirEntry->d_name);
			if(pluginFilename == "." || pluginFilename == "..")
				continue;
			std::string pluginFullFilename(directory + std::string("/") + pluginFilename);
			std::cout << "[Plugins] Loading CLAM plugin: " << pluginFullFilename << std::endl;
			void* handle = dlopen( pluginFullFilename.c_str(), RTLD_NOW);
			if (handle) continue; // Ok
			std::cout << "[Plugins] warning: failed to load CLAM plugin: " << pluginFullFilename << std::endl;
			
		}
	}
};

static RunTimeProcessingLibraryLoader dllLoader("/usr/lib/clam/");


