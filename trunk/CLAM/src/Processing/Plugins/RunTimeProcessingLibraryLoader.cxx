#ifdef WIN32
#include <windows.h>
#undef GetClassName
#else
#include <dlfcn.h>
#endif
#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>
#ifdef USE_LADSPA
	#include <CLAM/ProcessingFactory.hxx>
	#include <CLAM/LadspaWrapperCreator.hxx>
	#include <ladspa.h>
#endif

class RunTimeProcessingLibraryLoader
{

	static const char * pathSeparator;

	void * FullyLoadLibrary(const std::string & libraryPath)
	{
#ifdef WIN32
//		SetErrorMode( SEM_FAILCRITICALERRORS | SEM_NOALIGNMENTFAULTEXCEPT |
//			SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX );
		return LoadLibrary(libraryPath.c_str());
#else
		return dlopen( libraryPath.c_str(), RTLD_NOW);	
#endif
	}
	std::string LibraryLoadError()
	{
#ifdef WIN32
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );
		std::string message((char*)lpMsgBuf);
		LocalFree(lpMsgBuf);
		return message;
#else
		return dlerror();
#endif
	}

	void loadLibrariesFromPath(const std::string & path)
	{
		std::cerr << "[Plugins] Looking at path '" << path << "'" << std::endl;
		DIR* dir = opendir(path.c_str());
		if (!dir)
		{
			std::cerr << "[Plugins] warning: could not open CLAM plugins dir: " << path << std::endl;
			return;
		}
		while ( struct dirent * dirEntry = readdir(dir) )
		{
			std::string pluginFilename(dirEntry->d_name);
			if(pluginFilename == "." || pluginFilename == "..")
				continue;
			std::string pluginFullFilename(path + std::string("/") + pluginFilename);
			void* handle = FullyLoadLibrary( pluginFullFilename );
//			if (!handle) std::cerr << "[Plugins] Error: " << LibraryLoadError() << std::endl;
			if (handle) std::cerr << "[Plugins] Loaded: " << pluginFullFilename << std::endl;
		}
		closedir(dir);
	}
	
	void loadLibrariesFromPathLADSPA(const std::string & path)
	{
#ifdef USE_LADSPA
		std::cerr << "[LADSPA] Looking at path '" << path << "'" << std::endl;
		DIR* dir = opendir(path.c_str());
		if (!dir)
		{
			std::cerr << "[LADSPA] warning: could not open CLAM plugins dir: " << path << std::endl;
			return;
		}
		while ( struct dirent * dirEntry = readdir(dir) )
		{
			std::string pluginFilename(dirEntry->d_name);
			if(pluginFilename == "." || pluginFilename == "..")
				continue;
			LADSPA_Descriptor_Function descriptorTable = 0;
			std::string pluginFullFilename(path + std::string("/") + pluginFilename);
			void* handle = FullyLoadLibrary(pluginFullFilename);
			descriptorTable = (LADSPA_Descriptor_Function)dlsym(handle, "ladspa_descriptor");
			if (!descriptorTable)
			{
				std::cout << "[LADSPA] warning: trying to open non ladspa plugin: " << pluginFullFilename << std::endl;
				continue;
			}
			//std::cout << "[LADSPA] \topened plugin: " << pluginFullFilename << std::endl;
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
				factory.AddAttribute(oss.str(), "category", "LADSPA");
				factory.AddAttribute(oss.str(), "description", descriptor->Name);
				//std::cout << "[LADSPA] added \"" << plugin.factoryID << "\" to the Factory" << std::endl;
			}
		}
		closedir(dir);	
#endif // USE_LADSPA
	}
public:
	std::vector<std::string> splitPathVariable(const std::string & pathVariable)
	{
		std::string content=pathVariable;
		std::vector<std::string> result;
		while (!content.empty())
		{
			size_t separatorPos = content.find(pathSeparator);
			if (separatorPos == std::string::npos)
			{
				result.push_back(content);
				break;
			} 
			result.push_back(content.substr(0, separatorPos));
			content = content.substr(separatorPos+1);
		}
		return result;
	}
	RunTimeProcessingLibraryLoader()
	{
		const char * standardPaths[] = 
		{
			"/usr/local/lib/clam",
			"/usr/lib/clam",
			"/opt/lib/clam",
			0
		};
		const char * standardPathsLADSPA[] = 
		{
			"/usr/local/lib/ladspa",
			"/usr/lib/ladspa",
			0
		};
		const char * envPath = getenv("CLAM_PLUGIN_PATH");
		const char * envPathLADSPA = getenv("LADSPA_PATH");
		const char * envHome = getenv("HOME");
		std::string path = envPath ? envPath : "";
		std::string pathLADSPA = envPathLADSPA ? envPathLADSPA : "";
		if (envHome)
		{	
			path += std::string(path.empty()? "":pathSeparator) + envHome + "/.clam/plugins";
			pathLADSPA += std::string(pathLADSPA.empty()? "":pathSeparator) + envHome + "/.ladspa";
		}
		// add standardPath to the env path string (i.e. path1:path2)
		for (const char ** standardPath=standardPaths; *standardPath; standardPath++)
			path += std::string(path.empty()? "":pathSeparator) + *standardPath;
		// for each path, load libraries
		std::vector <std::string> environmentPaths = splitPathVariable(path);
		for (unsigned i=0; i<environmentPaths.size(); i++)
			loadLibrariesFromPath(environmentPaths[i]);

		//idem for ladspa
		for (const char ** standardPathLADSPA=standardPathsLADSPA; *standardPathLADSPA; standardPathLADSPA++)
			pathLADSPA += std::string(pathLADSPA.empty()? "":pathSeparator) + *standardPathLADSPA;
		std::vector <std::string> environmentPathsLADSPA = splitPathVariable(pathLADSPA);
		for (unsigned i=0; i<environmentPathsLADSPA.size(); i++)
			loadLibrariesFromPathLADSPA(environmentPathsLADSPA[i]);
	}
};


const char * RunTimeProcessingLibraryLoader::pathSeparator = 
	#ifdef WIN32
		";";
	#else
		":";
	#endif

static RunTimeProcessingLibraryLoader dllLoader;


