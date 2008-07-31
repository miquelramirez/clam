#include "MainWindow.hxx"
#include <QtCore/QTemporaryFile>


MainWindow::~MainWindow()
{
}

void MainWindow::on_action_CompileAsLadspaPlugin_triggered()
{
	Ui::LadspaMetadataEditor ui;
	QDialog ladspaMetadataDialog(this);
	ui.setupUi(&ladspaMetadataDialog);
	bool accepted = ladspaMetadataDialog.exec();
	if (!accepted) return;
	unsigned id = ui.ladspaUniqueId->value();
	// TODO: Take the proper encoding here
	QString label = ui.ladspaLabel->text();
	QString name = ui.ladspaName->text();
	QString maker = ui.ladspaMaker->text();
	QString copyright = ui.ladspaCopyright->currentText();

	std::ostringstream memoryBuffer;
	CLAM::XMLStorage::Dump(_network, "network", memoryBuffer);
	QTemporaryFile networkFile("ladspa-XXXXXXXX.clamnetwork");
	if (not networkFile.open()) return; // TODO: Error
	networkFile.write(memoryBuffer.str().c_str());

	QString pluginTemplateCode = 
		"#include <CLAM/LadspaNetworkExporter.hxx>\n"
		"#include <CLAM/LadspaLibrary.hxx>\n"
		"static CLAM::LadspaLibrary library;\n"
		"CLAM_EXTERNAL_FILE_DATA(embededNetwork,\"%1\")\n"
		"extern \"C\" const LADSPA_Descriptor * ladspa_descriptor(unsigned long index)\n"
		"{\n"
		"	static CLAM::LadspaNetworkExporter n2(library, embededNetwork, %2,\n"
		"			\"%3\", \"%4\",\n"
		"			\"%5\", \"%6\");\n"
		"	return library.pluginAt(index);\n"
		"}\n";
	QString pluginCode = pluginTemplateCode
		.arg(networkFile.fileName()).arg(id)
		.arg(label).arg(name).arg(maker).arg(copyright);
	std::cout << pluginCode.toStdString() << std::endl;

	QTemporaryFile cxxFile("ladspa-XXXXXXXX.cxx");
	if (not cxxFile.open()) return; // TODO: Error
	cxxFile.write(pluginCode.toAscii());

	QTemporaryFile sconsFile("ladspa-XXXXXXXX.scons");
	if (not sconsFile.open()) return; // TODO: Error
	sconsFile.write(QString(
"import os, glob, sys\n"
"libraryName='%1'\n"
"options = Options('options.cache', ARGUMENTS)\n"
"options.Add(PathOption('clam_prefix', 'The prefix where CLAM was installed', ''))\n"
"options.Add(PathOption('prefix', 'Installation prefix (normally /usr, by default this is clam_prefix)', "", validator=PathOption.PathAccept))\n"
"options.Add(PathOption('sandbox_path', 'Path where third party libraries were installed (in windows)', "", validator=PathOption.PathAccept))\n"
"options.Add(BoolOption('crossmingw', 'Using MinGW crosscompiler mode', 'no') )\n"
"\n"
"toolChain = 'default'\n"
"if sys.platform == 'win32': toolChain = 'mingw'\n"
"env = Environment(ENV=os.environ, tools=[toolChain], options=options)\n"
"options.Save('options.cache', env)\n"
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
"sources = glob.glob('*.cxx')\n"
"extraPaths = [\n"
"	CLAMInstallDir+'/include',\n"
"	CLAMInstallDir+'/include/CLAM', # KLUDGE to keep old style includes\n"
"]\n"
"env.Append(CPPPATH=extraPaths)\n"
"\n"
"env.Append( CCFLAGS=['-g','-O3','-Wall'] )\n"
"libraries = [\n"
"	env.SharedLibrary(target=libraryName, source = sources),\n"
"	]\n"
"install = env.Install(os.path.join(InstallDir,'lib','ladspa'), libraries)\n"
"env.Alias('install', install)\n"
"env.Default(libraries)\n"
"\n"
).toAscii());
	system(QString("scons -f %1")
		.arg(sconsFile.fileName()).toStdString().c_str()
	);
}


