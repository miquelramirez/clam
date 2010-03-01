#ifndef LadspaPluginCompilationTask_hxx
#define LadspaPluginCompilationTask_hxx
#include "MainWindow.hxx"
#include <QtCore/QTemporaryFile>
#include "ui_LadspaMetadataEditor.hxx"


//////////////////////
// LADSPA Generator
// TODO: Use a TaskRunner
// TODO: Auto install the plugin into ~/.ladspa
// TODO: Windows?

static const char * ladspaCxxTemplate =
	"#include <CLAM/LadspaNetworkExporter.hxx>\n"
	"#include <CLAM/LadspaLibrary.hxx>\n"
	"#include <CLAM/EmbeddedFile.hxx>\n"
	"static CLAM::LadspaLibrary library;\n"
	"CLAM_EMBEDDED_FILE(embededNetwork,\"%1\")\n"
	"extern \"C\" const LADSPA_Descriptor * ladspa_descriptor(unsigned long index)\n"
	"{\n"
	"	static CLAM::LadspaNetworkExporter n2(library, embededNetwork, %2,\n"
	"			\"%3\", \"%4\",\n"
	"			\"%5\", \"%6\");\n"
	"	return library.pluginAt(index);\n"
	"}\n";

static const char * ladspaSconsTemplate =
	"import os, glob, sys\n"
	"libraryName='%1'\n"
	"print 'Building', libraryName\n"
	"options = Options('options.cache', ARGUMENTS)\n"
	"options.Add(PathOption('clam_prefix', 'The prefix where CLAM was installed', ''))\n"
	"options.Add(PathOption('prefix', 'Installation prefix (normally /usr, by default this is clam_prefix)', '', validator=PathOption.PathAccept))\n"
	"options.Add(BoolOption('crossmingw', 'Using MinGW crosscompiler mode', 'no') )\n"
	"\n"
	"toolChain = 'default'\n"
	"if sys.platform == 'win32': toolChain = 'mingw'\n"
	"env = Environment(ENV=os.environ, tools=[toolChain], options=options)\n"
	"#options.Save('options.cache', env) # do not save options in NetworkEditor generated plugins\n"
	"Help(options.GenerateHelpText(env))\n"
	"env.SConsignFile() # Single signature file\n"
	"\n"
	"CLAMInstallDir = env['clam_prefix']\n"
	"InstallDir = env['prefix'] or env['clam_prefix']\n"
	"clam_sconstoolspath = os.path.join(CLAMInstallDir,'share','clam','sconstools')\n"
	"if env['crossmingw'] :\n"
	"	env.Tool('crossmingw', toolpath=[clam_sconstoolspath])\n"
	"env.Tool('clam', toolpath=[clam_sconstoolspath])\n"
	"env.EnableClamModules([\n"
	"	'clam_core',\n"
	"	'clam_audioio',\n"
	"	'clam_processing',\n"
	"	] , CLAMInstallDir)\n"
	"\n"
	"sources = ['%2']\n"
	"extraPaths = [\n"
	"	CLAMInstallDir+'/include',\n"
	"	CLAMInstallDir+'/include/CLAM', # KLUDGE to keep old style includes\n"
	"]\n"
	"env.Append(CPPPATH=extraPaths)\n"
	"\n"
	"env.Append( CCFLAGS=['-g','-O3','-Wall'] )\n"
	"libraries = [\n"
	"	env.SharedLibrary(target=libraryName, source = sources, SHLIBPREFIX=''),\n"
	"	]\n"
	"install = env.Install(os.path.join(InstallDir,'lib','ladspa'), libraries)\n"
	"env.Alias('install', install)\n"
	"env.Default(libraries)\n"
	"\n";

class LadspaPluginCompilationTask : public QObject
{
public:
	LadspaPluginCompilationTask(QWidget * parent, CLAM::Network & network)
		: QObject(parent)
		, _network(network)
		, networkFile("ladspa-XXXXXXXX.clamnetwork")
		, cxxFile(    "ladspa-XXXXXXXX.cxx")
		, sconsFile(  "ladspa-XXXXXXXX.scons")
	{
	}
	~LadspaPluginCompilationTask() {}
	void keepFiles()
	{
		cxxFile.setAutoRemove(false);
		sconsFile.setAutoRemove(false);
		networkFile.setAutoRemove(false);
	}
	void run(
		unsigned id, 
		const QString & label, 
		const QString & name, 
		const QString & maker, 
		const QString & copyright,
		const QString & libraryName,
		const QString & clamPrefix
		)
	{
		if (not networkFile.open()) return; // TODO: Error
		std::ostringstream memoryBuffer;
		CLAM::XMLStorage::Dump(_network, "network", memoryBuffer);
		networkFile.write(memoryBuffer.str().c_str());
		networkFile.flush();

		if (not cxxFile.open()) return; // TODO: Error
		QString pluginCode = QString(ladspaCxxTemplate)
			.arg(networkFile.fileName()).arg(id)
			.arg(label).arg(name).arg(maker).arg(copyright);
		std::cout << pluginCode.toStdString() << std::endl;
		cxxFile.write(pluginCode.toUtf8());
		cxxFile.flush();

		if (not sconsFile.open()) return; // TODO: Error
		sconsFile.write(QString(ladspaSconsTemplate)
			.arg(libraryName)
			.arg(cxxFile.fileName())
			.toLocal8Bit());
		sconsFile.flush();
		QString command = QString("scons -f %1 clam_prefix=%2 prefix=%3")
			.arg(sconsFile.fileName())
			.arg(clamPrefix)
			.arg("/usr/local/")
			;
		std::cout << command.toStdString() << std::endl;
		QProcess::execute(command);
	}	
private:
	CLAM::Network & _network;
	QTemporaryFile networkFile;
	QTemporaryFile cxxFile;
	QTemporaryFile sconsFile;
};


#endif//LadspaPluginCompilationTask_hxx
