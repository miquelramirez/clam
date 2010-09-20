#include <QtCore/QTemporaryFile>
#include "ui_LadspaMetadataEditor.hxx"
#include "LadspaPluginCompilationTask.hxx"
#include "Python.h"
#include <fstream>

MainWindow::~MainWindow()
{
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


//CLAM_EMBEDEDFILE(migrationScript, "migrationScript");

const char * MainWindow::updatedNetworkVersion(const std::string & filename)
{
	Py_Initialize();
	PyObject * pModule = PyImport_ImportModule("clamrefactor");
	if (pModule == NULL)
	{
		PyErr_Print();
		return 0;
	}
	PyObject * pDict = PyModule_GetDict(pModule);
	// Build the name of a callable class 
	PyObject * pClassClamNetwork = PyDict_GetItemString(pDict, "ClamNetwork");
	// Create an instance of the class
	if (! PyCallable_Check(pClassClamNetwork))
	{
		//TODO Manage error
		return 0;
	}
	PyObject * pClamNetwork = Py_BuildValue( "(s)", filename.c_str() );
	PyObject * pInstanceClamNetwork = PyObject_CallObject(pClassClamNetwork, pClamNetwork); 
	Py_DECREF(pClamNetwork);

	if (pInstanceClamNetwork == NULL)
	{
		PyErr_Print();
		return 0;
	}
	std::ifstream ifs( "migrationScript" );
	std::string command;
	PyObject *pValue;
	while( getline(ifs, command) )
	{
		pValue = PyObject_CallMethod( pInstanceClamNetwork, "runCommand", "(s)", command.c_str() );
	}

	PyObject *pModule2, *pDict2, *pClassStringIO, *pStringIO, *pInstanceStringIO;
	pModule2 = PyImport_ImportModule("StringIO");
	if (pModule2 == NULL)
	{
		PyErr_Print();
		return 0;
	}
	pDict2 = PyModule_GetDict(pModule2);
	pClassStringIO = PyDict_GetItemString(pDict2, "StringIO");
	if (PyCallable_Check(pClassStringIO))
	{
		pStringIO = Py_BuildValue( "(s)", "" );
		pInstanceStringIO = PyObject_CallObject(pClassStringIO, pStringIO); 
	}

	PyObject_CallMethod(pInstanceClamNetwork, "dump", "(O)", pInstanceStringIO);
	pValue = PyObject_CallMethod(pInstanceStringIO, "getvalue", NULL);

	char *cstring;
	if (pValue == NULL)
	{
		PyErr_Print();
		return 0;
	}

	PyArg_Parse(pValue, "s", &cstring);
	Py_DECREF(pModule);
	Py_DECREF(pModule2);
	Py_DECREF(pStringIO);
	Py_DECREF(pInstanceClamNetwork);
	Py_DECREF(pInstanceStringIO);
	Py_DECREF(pValue);
	Py_Finalize();

	return cstring;
}
