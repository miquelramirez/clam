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

public:
	virtual void Load() const
	{
		std::string examplesDir = CompletePathFor("examples/ladspadir");
		LoadLibrariesFromPath(examplesDir);
		examplesDir = CompletePathFor("ladspadir");
		LoadLibrariesFromPath(examplesDir);
		//std::cout<<"[FAUST DEBUG] \tload of RunTimeFaustLibraryLoader"<<std::endl;
		RunTimeLibraryLoader::Load(); // needed??
	}
	void LoadPlugin(const std::string & pluginFullPath) const
	{
		LoadLibrariesFromPath(pluginFullPath);
	}

	const std::map<std::string, std::string> GetCompilePluginCommands(const std::string & pluginSourceFullName) const
	{
		typedef std::map<std::string,std::string> CommandsMap;
		CommandsMap commands;
		const std::string pluginSourcePath=pluginSourceFullName.substr(0,pluginSourceFullName.rfind("/")+1);// separate path from plugin filename
		std::cout << "sourcepath:"<<pluginSourcePath<<std::endl;
		std::string baseFileName=pluginSourceFullName.substr( pluginSourceFullName.rfind("/")+1); //separate base filename
		baseFileName=baseFileName.substr(0,baseFileName.rfind("."));	// throw away extension

		const std::string ladspaLibraryFullPath=faustLibIncludeFile();
		if (ladspaLibraryFullPath == "") return commands; // if doesn't found libraries, return with empty list of commands
		const std::string faustCommandFullPath = faustBinCommand();
		if (faustCommandFullPath == "") return commands; // if doesn't found libraries, return with empty list of commands

		std::string command;
		std::string defaultPath;

		// Generate Faust LADSPA plugin .cpp from .dsp:
		defaultPath=ladspaLibraryFullPath.substr(0,ladspaLibraryFullPath.rfind("/"));	// compile from library path (to include possible libraries)
		command = faustCommandFullPath + " -a " + ladspaLibraryFullPath + " " +pluginSourceFullName  + " -o " + pluginSourcePath +  baseFileName + ".cpp";
		commands.insert(CommandsMap::value_type(command,defaultPath));

		// compile Faust LADSPA plugin from .cpp:
		command = "g++ -fPIC -shared -O3 -Dmydsp=" + baseFileName + " "+pluginSourcePath + baseFileName + ".cpp -o " + pluginSourcePath +"ladspadir/"+  baseFileName + ".so";
		commands.insert(CommandsMap::value_type(command,defaultPath));

		// compile Faust SVG diagram from .dsp
		command = faustCommandFullPath + " -svg " + pluginSourceFullName + " -o /dev/null";
		commands.insert(CommandsMap::value_type(command,defaultPath));
		return commands;
	}

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

			CLAM::ProcessingFactory::Creator * creator=new CLAM::LadspaWrapperCreator(pluginFullFilename, i, oss.str());
			if (!factory.KeyExists(oss.str()))
			{
				factory.AddCreatorWarningRepetitions(oss.str(), creator);
				std::cout << "[FAUST-LADSPA] \t created: " << oss.str() << std::endl;
			}
			else
			{
				factory.ReplaceCreator(oss.str(), creator);
				std::cout << "[FAUST-LADSPA] \t replaced: " << oss.str() << std::endl;
			}
			factory.AddAttribute(oss.str(), "category", "FAUST");
			factory.AddAttribute(oss.str(), "description", descriptor->Name);

			std::string pluginName=descriptor->Label;
			std::string svgFileDir = CompletePathFor( "examples/" + pluginName + ".dsp-svg/process.svg");
			if (svgFileDir != "")
			{
				factory.AddAttribute(oss.str(), "faust_diagram", svgFileDir);
				std::cout << "[FAUST-LADSPA] \tusing faust diagram: " << svgFileDir << std::endl;
			}
			if (!factory.AttributeExists(oss.str(), "embedded_svg"))
				factory.AddAttribute(oss.str(), "embedded_svg", ":icons/images/faustlogo.svg");
			if (!factory.AttributeExists(oss.str(), "icon"))
				factory.AddAttribute(oss.str(), "icon", "faustlogo.svg");

			std::string sourceFileName=CompletePathFor( "examples/"+pluginName+".dsp");
			if (sourceFileName != "")
				factory.AddAttribute(oss.str(), "faust_source_file", sourceFileName);
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
	const char * pathEnvironmentVar() const { return  "FAUST_PATH"; }
	const char * libraryType() const { return  "LADSPA"; }

private:
	const std::string faustLibIncludeFile() const	{ return CompletePathFor("architecture/ladspa.cpp"); } // get needed libraries path
	const std::string faustBinCommand() const	{ return CompletePathFor("compiler/faust"); }		// get faust binary path
};

#endif // RunTimeFaustLibraryLoader_hxx


