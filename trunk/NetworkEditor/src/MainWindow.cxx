#include "MainWindow.hxx"
#include <QtCore/QTemporaryFile>


MainWindow::~MainWindow()
{
}

static const char * ladspaCxxTemplate =
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
	"	env.SharedLibrary(target=libraryName, source = sources),\n"
	"	]\n"
	"install = env.Install(os.path.join(InstallDir,'lib','ladspa'), libraries)\n"
	"env.Alias('install', install)\n"
	"env.Default(libraries)\n"
	"\n";

void MainWindow::on_action_CompileAsLadspaPlugin_triggered()
{
	QSettings settings;
	Ui::LadspaMetadataEditor ui;
	QDialog ladspaMetadataDialog(this);
	ui.setupUi(&ladspaMetadataDialog);
	ui.ladspaMaker->setText(settings.value("Ladspa/Maker").toString());
	ui.clamPrefix->setText(settings.value("ClamPrefix").toString());
	bool accepted = ladspaMetadataDialog.exec();
	if (!accepted) return;
	unsigned id = ui.ladspaUniqueId->value();
	QString label = ui.ladspaLabel->text();
	QString name = ui.ladspaName->text();
	QString maker = ui.ladspaMaker->text();
	QString copyright = ui.ladspaCopyright->currentText();
	QString libraryName = ui.ladspaLibrary->text();
	settings.setValue("Ladspa/Maker", maker);
	QString clamPrefix = ui.clamPrefix->text();
	settings.setValue("ClamPrefix", clamPrefix);

	QTemporaryFile networkFile("ladspa-XXXXXXXX.clamnetwork");
	if (not networkFile.open()) return; // TODO: Error
	std::ostringstream memoryBuffer;
	CLAM::XMLStorage::Dump(_network, "network", memoryBuffer);
	networkFile.write(memoryBuffer.str().c_str());
	networkFile.flush();

	QTemporaryFile cxxFile("ladspa-XXXXXXXX.cxx");
	if (not cxxFile.open()) return; // TODO: Error
	QString pluginCode = QString(ladspaCxxTemplate)
		.arg(networkFile.fileName()).arg(id)
		.arg(label).arg(name).arg(maker).arg(copyright);
	std::cout << pluginCode.toStdString() << std::endl;
	cxxFile.write(pluginCode.toUtf8());
	cxxFile.flush();

	QTemporaryFile sconsFile("ladspa-XXXXXXXX.scons");
	if (not sconsFile.open()) return; // TODO: Error
	sconsFile.write(QString(ladspaSconsTemplate)
		.arg(libraryName)
		.arg(cxxFile.fileName())
		.toLocal8Bit());
	sconsFile.flush();

	QString command = QString("scons -f '%1' clam_prefix=%2 prefix=%3")
        .arg(sconsFile.fileName())
        .arg(clamPrefix)
        .arg("/usr/local/")
		;
	std::cout << command.toStdString() << std::endl;
	system(command.toStdString().c_str());
#if 0 // To debug, keep the files
	cxxFile.setAutoRemove(false);
	sconsFile.setAutoRemove(false);
	networkFile.setAutoRemove(false);
#endif

}


