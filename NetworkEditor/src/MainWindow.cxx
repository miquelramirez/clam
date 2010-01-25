#include <QtCore/QTemporaryFile>
#include "ui_LadspaMetadataEditor.hxx"
#include "LadspaPluginCompilationTask.hxx"

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


