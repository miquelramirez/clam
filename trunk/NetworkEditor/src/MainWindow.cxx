#include "Python.h"
#include <QtCore/QTemporaryFile>
#include "ui_LadspaMetadataEditor.hxx"
#include "LadspaPluginCompilationTask.hxx"

#include <fstream>

#include <CLAM/EmbeddedFile.hxx>
CLAM_EMBEDDED_FILE(migrationScript,"migrationScript")
CLAM_EMBEDDED_FILE(clamrefactor,"../CLAM/scripts/clamrefactor.py")



MainWindow::~MainWindow()
{
}


static std::string pythonString(PyObject * object)
{
	PyObject * pythonString = PyObject_Str(object);
	std::string result = PyString_AsString(pythonString);
	Py_DECREF(pythonString);
	return result;
}
static std::string pythonClassName(PyObject * object)
{
	PyObject * pystring = PyObject_GetAttrString(object,"__name__");
	std::string result = PyString_AsString(pystring);
	Py_DECREF(pystring);
	return result;
}

class NetworkUpgrader::Impl
{
public:
	std::string _result;
	std::string _errorMessage;
	const char * run(const std::string & networkContents);
	const char * takePythonError();
};

NetworkUpgrader::NetworkUpgrader()
	: _impl(new Impl)
{
}
NetworkUpgrader::~NetworkUpgrader()
{
	delete _impl;
}

const char * NetworkUpgrader::run(const std::string & filename)
{
	return _impl->run(filename);
}

const char * NetworkUpgrader::errorMessage() const
{
	return _impl->_errorMessage.c_str();
}


const char * NetworkUpgrader::Impl::takePythonError()
{
	PyObject *pType=0;
	PyObject *pValue=0;
	PyObject *pTrace=0;
	PyErr_Fetch(&pType, &pValue, &pTrace);
	CLAM_ASSERT(pType,"pType es zero");
	CLAM_ASSERT(pValue,"pValue es zero");
	_errorMessage = 
		pythonClassName(pType) + ": " +
		pythonString(pValue);
//		(pTrace? pythonString(pTrace):"");

	std::cout << _errorMessage << std::endl;
	
	if (pTrace) Py_DECREF(pTrace);
	Py_DECREF(pValue);
	if (pType) Py_DECREF(pType);
	Py_Finalize();
	return 0;
}

const char * NetworkUpgrader::Impl::run(const std::string & filename)
{
	Py_Initialize();
	PyObject * pModule = PyImport_ImportModule("__main__");
	if (not pModule)
		return takePythonError();

	PyObject * pDict = PyModule_GetDict(pModule);
	// Hack to avoid __main__ exectuion
	PyDict_SetItemString(pDict, "__name__", Py_BuildValue("s","boo"));

	int error = PyRun_SimpleString(clamrefactor);
	if (error)
	{
		_errorMessage = "Error loading clamrefactor python script";
		return 0;
	}

	PyObject * pClassClamNetwork = PyDict_GetItemString(pDict, "ClamNetwork");
	if (!PyCallable_Check(pClassClamNetwork))
		return takePythonError();

	PyObject * pClamNetwork = Py_BuildValue( "(s)", filename.c_str() );
	PyObject * pInstanceClamNetwork = PyObject_CallObject(pClassClamNetwork, pClamNetwork); 
	Py_DECREF(pClamNetwork);
	if (pInstanceClamNetwork == NULL)
		return takePythonError();

	PyObject * pValue = PyObject_CallMethod(
		pInstanceClamNetwork, "runScript", "(s)", migrationScript );
	if (not pValue)
		return takePythonError();

	PyObject * pModule2 = PyImport_ImportModule("StringIO");
	if (pModule2 == NULL)
		return takePythonError();

	PyObject * pDict2 = PyModule_GetDict(pModule2);
	PyObject * pClassStringIO = PyDict_GetItemString(pDict2, "StringIO");
	PyObject * pStringIO = Py_BuildValue( "(s)", "" );
	PyObject * pInstanceStringIO = PyObject_CallObject(pClassStringIO, pStringIO);
	if (!PyCallable_Check(pClassStringIO))
		return takePythonError(); 
	PyObject_CallMethod(pInstanceClamNetwork, "dump", "(O)", pInstanceStringIO);
	pValue = PyObject_CallMethod(pInstanceStringIO, "getvalue", NULL);

	char * cstring;
	if (pValue == NULL)
		return takePythonError();

	PyArg_Parse(pValue, "s", &cstring);
	Py_DECREF(pModule);
	Py_DECREF(pModule2);
	Py_DECREF(pStringIO);
	Py_DECREF(pInstanceClamNetwork);
	Py_DECREF(pInstanceStringIO);
	Py_DECREF(pValue);
	Py_Finalize();
	_result = cstring;
	return _result.c_str();
}


void MainWindow::on_action_CompileAsLadspaPlugin_triggered()
{
	QSettings settings;
	Ui::LadspaMetadataEditor ui;
	QDialog ladspaMetadataDialog(this);
	ui.setupUi(&ladspaMetadataDialog);
	ui.fieldHelp->setText("");
	ui.ladspaMaker->setText(settings.value("Ladspa/Maker").toString());
	ui.clamPrefix->setLocation(settings.value("ClamPrefix").toString());
	ui.clamPrefix->setDirMode();
	ui.clamPrefix->propagateWhatsThisHack();
	bool accepted = ladspaMetadataDialog.exec();
	if (!accepted) return;
	settings.setValue("Ladspa/Maker", ui.ladspaMaker->text());
	settings.setValue("ClamPrefix", ui.clamPrefix->location());

	LadspaPluginCompilationTask ladspaCompilation(this, _network);
	if (ui.keepIntermediateFiles->isChecked())
		ladspaCompilation.keepFiles();
	ladspaCompilation.run(
		ui.ladspaUniqueId->value(), 
		ui.ladspaLabel->text(), 
		ui.ladspaName->text(), 
		ui.ladspaMaker->text(), 
		ui.ladspaCopyright->currentText(),
		ui.ladspaLibrary->text(),
		ui.clamPrefix->location()
		);
}

////////////////////////////////////////////////////
// Faust Compilation

void MainWindow::on_action_Compile_Faust_Modules_triggered()
{
#if USE_LADSPA
	std::cout << "[FAUST] \tcompiling" << std::endl;
	RunTimeFaustLibraryLoader faustLoader;
	std::string faustDir=faustLoader.CompletePathFor("examples"); // get path for examples dir
	QDir examplesPath=QDir(faustDir.c_str());
	if (not examplesPath.exists("ladspadir") and not examplesPath.mkdir("ladspadir")) // if directory for plugins compilation doesn't exist try to create it
	{
		QMessageBox::warning(this, tr("Faust compilation failed"),
			tr(
				"<p>Can't create ladspadir on '%1'!</p>\n"
				"<p>Compilation failed.</p>\n"
			).arg(examplesPath.path()) );
		return;
	}
	// disable compilation action while compiling
	ui.action_Compile_Faust_Modules->setEnabled(false);

	statusBar()->showMessage(tr("Compiling faust modules..."));
	TaskRunner * runner = new TaskRunner("FaustCompilationWidget");
	runner->setWindowTitle(tr("Faust compilation"));
	// define compilation using make:
	runner->enqueueSubtask("make", QStringList() << "ladspa", faustDir.c_str());
	runner->enqueueSubtask("make", QStringList() << "svg", faustDir.c_str());
	connect(runner, SIGNAL(taskDone(bool)), this, SLOT(endCompilationFaust(bool)));
	connect(runner, SIGNAL(widgetDestructed()), this,SLOT(closeCompilationWidget()));
	addDockWidget( Qt::BottomDockWidgetArea, runner);
	// Wait the window to be redrawn after the reconfiguration
	// before loading the cpu with the extractor
	qApp->processEvents();
	if (not runner->runQueued())
	{
		QMessageBox::critical(this, tr("Compiling Faust modules"),
			tr("<p><b>Error: Compilation failed.</b></p>\n"));
		statusBar()->clearMessage();
		delete runner;
	}
#endif
}
void MainWindow::endCompilationFaust(bool done)
{
	ui.action_Compile_Faust_Modules->setEnabled(true);
	statusBar()->clearMessage();
	if (done)
		on_action_Reload_Faust_Modules_triggered();
}
void MainWindow::closeCompilationWidget()
{
	ui.action_Compile_Faust_Modules->setEnabled(true);
}

void MainWindow::on_action_Reload_Faust_Modules_triggered()
{
#if USE_LADSPA
	RunTimeFaustLibraryLoader faustLibraryLoader;
	faustLibraryLoader.ReLoad();
	if (_processingTree)
		_processingTree->RePopulateTree();
#endif
}
