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
			std::cerr << "[Plugins] Loading: " << pluginFullFilename << std::endl;
			void* handle = FullyLoadLibrary( pluginFullFilename );
			if (!handle) std::cerr << "[Plugins] Error: " << LibraryLoadError() << std::endl;
		}
		closedir(dir);
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
		const char * envPath = getenv("CLAM_PLUGIN_PATH");
		const char * envHome = getenv("HOME");
		std::string path = envPath ? envPath : "";
		if (envHome) 
			path += std::string(path.empty()? "":pathSeparator) + envHome + "/.clam/plugins";
		for (const char ** standardPath=standardPaths; *standardPath; standardPath++)
			path += std::string(path.empty()? "":pathSeparator) + *standardPath;

		std::vector <std::string> environmentPaths = splitPathVariable(path);
		for (unsigned i=0; i<environmentPaths.size(); i++)
			loadLibrariesFromPath(environmentPaths[i]);
	}
};

const char * RunTimeProcessingLibraryLoader::pathSeparator = 
	#ifdef WIN32
		";";
	#else
		":";
	#endif

static RunTimeProcessingLibraryLoader dllLoader;


