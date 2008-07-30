#include "uic_MainWindow.hxx"
#include "uic_LadspaMetadataEditor.hxx"
#include "NetworkCanvas.hxx"
#include "ProcessingTree.hxx"
#include <QtGui/QVBoxLayout>
#include <QtGui/QScrollArea>
#include <QtGui/QDockWidget>
#include <QtGui/QWhatsThis>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include "uic_About.hxx"
#include <CLAM/Network.hxx>
#include <CLAM/NetworkPlayer.hxx>
#include <CLAM/NaiveFlowControl.hxx>
#include <CLAM/BlockingNetworkPlayer.hxx>
#include <CLAM/XMLStorage.hxx>
#include <CLAM/XmlStorageErr.hxx>
#include <CLAM/CLAMVersion.hxx>
#include "NetworkEditorVersion.hxx"

// copied from Annotator:
#include "TaskRunner.hxx"

#include <QtSvg/QSvgWidget>
#include <QtCore/QProcess>
#if QT_VERSION >= 0x040200
	#include <QtGui/QDesktopServices>
#endif

#ifdef USE_JACK
#include <CLAM/JACKNetworkPlayer.hxx>
#endif
#ifdef USE_PORTAUDIO
#include <CLAM/PANetworkPlayer.hxx>
#endif
#ifdef USE_LADSPA
#	include <CLAM/RunTimeFaustLibraryLoader.hxx> 
#	include <QtCore/QDir>
#endif

#ifndef DATA_EXAMPLES_PATH
#define DATA_EXAMPLES_PATH "example-data"
#endif

class MainWindow : public QMainWindow
{
	Q_OBJECT
	Ui::MainWindow ui;
public:
	virtual ~MainWindow();

	MainWindow()
		: _networkPlayer(0),
		_saveUsingOldPosFiles(false)
	{
		ui.setupUi(this);
		setWindowIcon(QIcon(":/icons/images/NetworkEditor-icon.png"));

		QTabWidget * centralTab = new QTabWidget(this);
		setCentralWidget(centralTab);
		centralTab->setTabPosition(QTabWidget::South);

		QScrollArea * scroll = new QScrollArea(this);
		_canvas = new ClamNetworkCanvas;
		scroll->setWidget(_canvas);

		QScrollArea * backendScroll = new QScrollArea(this);

		centralTab->addTab(scroll, "Network");
		centralTab->addTab(backendScroll, "Jack");
		
		_processingTreeDock = new QDockWidget(this);
		_processingTree = new NetworkGUI::ProcessingTree(_processingTreeDock);
		_processingTreeDock->setObjectName("ProcessingTree");
		_processingTreeDock->setWindowTitle(tr("Processing Toolbox"));
		_processingTreeDock->setWidget(_processingTree);
		addDockWidget(Qt::LeftDockWidgetArea, _processingTreeDock);

		_aboutDialog = new QDialog(this);
		Ui::About aboutUi;
		aboutUi.setupUi(_aboutDialog);

		aboutUi.versionInfo->setText(tr(
			"<p><b>Network Editor version %1</b></p>"
			"<p>Using CLAM version %2</p>"
			)
			.arg(NetworkEditor::GetFullVersion())
			.arg(CLAM::GetFullVersion())
			);

		QSettings settings;
		_recentFiles=settings.value("RecentFiles").toStringList();
		updateRecentMenu();
		restoreState(settings.value("DockWindowsState").toByteArray());
		bool embedSvgDiagramsOption=settings.value("EmbedSVGDiagramsOption").toBool();
		bool whiteColorsForBoxes=settings.value("WhiteColorsForBoxes").toBool();
		ui.action_White_colors_Option->setChecked(whiteColorsForBoxes);
		_canvas->setEmbedSVGDiagramsOption(embedSvgDiagramsOption);
		ui.action_Embed_SVG_Diagrams_Option->setChecked(embedSvgDiagramsOption);

		_network.AddFlowControl( new CLAM::NaiveFlowControl );
		QString backend = "None";
		QString backendLogo = ":/icons/images/editdelete.png"; // TODO: Change this icon
		if (_networkPlayer) delete _networkPlayer;
		_networkPlayer = 0;

#ifdef USE_LADSPA
		ui.menuFaust->setEnabled(true);
		ui.action_Compile_Faust_Modules->setEnabled(true);
#endif

#ifdef USE_JACK
		CLAM::JACKNetworkPlayer * jackPlayer = new CLAM::JACKNetworkPlayer();
		backend = "JACK";
		backendLogo = ":/icons/images/jacklogo-mini.png";
		if ( jackPlayer->IsWorking())
		{
			_networkPlayer = jackPlayer;
			_jackCanvas = new ClamNetworkCanvas; // TODO: This should be a JackNetworkCanvas
			backendScroll->setWidget(_jackCanvas);
		}
		else
			delete jackPlayer;
#endif
#ifdef USE_PORTAUDIO
		if (! _networkPlayer)
		{
			backend = "PortAudio";
			backendLogo = ":/icons/images/portaudiologo-mini.png";
			_networkPlayer = new CLAM::PANetworkPlayer();
		}
#endif
		if (! _networkPlayer)
		{
			backend = "ALSA";
			backendLogo = ":/icons/images/alsalogo-mini.png";
			_networkPlayer = new CLAM::BlockingNetworkPlayer();
		}
		_network.SetPlayer( _networkPlayer );

		_playingLabel = new QLabel;
		statusBar()->addPermanentWidget(_playingLabel);
		_backendLabel = new QLabel;
		statusBar()->addPermanentWidget(_backendLabel);
		_backendLabel->setToolTip(tr("<p>Audio Backend: %1</p>").arg(backend));
		_backendLabel->setPixmap(QPixmap(backendLogo));
		updatePlayStatusIndicator();

		connect(ui.action_Show_processing_toolbox, SIGNAL(toggled(bool)), _processingTreeDock, SLOT(setVisible(bool)));
		connect(_processingTreeDock, SIGNAL(visibilityChanged(bool)), ui.action_Show_processing_toolbox, SLOT(setChecked(bool)));
		connect(ui.action_Print, SIGNAL(triggered()), _canvas, SLOT(print()));
		connect(_canvas, SIGNAL(changed()), this, SLOT(updateCaption()));
		connect(_canvas, SIGNAL(openFileWithExternalApplicationRequest()), this, SLOT(openFileWithExternalApplicationFromProcessing()));
		updateCaption();

	}
	void updatePlayStatusIndicator()
	{
		if (_canvas->networkIsDummy() )
			_playingLabel->setText(tr("<p style='color:blue'>Dummy</p>"));
		else if (! _network.IsStopped())
			_playingLabel->setText(tr("<p style='color:green'>Playing...</p>"));
		else
			_playingLabel->setText(tr("<p style='color:red'>Stopped</p>"));
	}
	void updateRecentMenu()
	{
		ui.menuOpen_recent->clear();
		QMenu * toolBarOpenMenu = new QMenu(this);
		ui.action_OpenToolbar->setMenu(toolBarOpenMenu);
		int i=0;
		for (QStringList::iterator it = _recentFiles.begin(); it!=_recentFiles.end(); it++)
		{
			QString text = QString("&%1 %2").arg(++i).arg(*it);
			QAction * recentFileAction = new QAction(text,this);
			recentFileAction->setData(*it);
			ui.menuOpen_recent->addAction(recentFileAction);
			toolBarOpenMenu->addAction(recentFileAction);
			connect(recentFileAction, SIGNAL(triggered()), this, SLOT(openRecentTriggered()));
		}
	}
	void appendRecentFile(const QString & recentFile)
	{
		_recentFiles.removeAll(recentFile);
		_recentFiles.push_front(recentFile);
		while (_recentFiles.size()> 8)
			_recentFiles.pop_back();
		updateRecentMenu();
	}

	QString networkFilter() {return tr(
		"CLAM Network files (*.clamnetwork)"
//		";;Dummy Network files (*.dummynetwork)"
		); }

	bool askUserSaveChanges()
	{
		bool goOn = true;
		bool abort = false;
		if (! _canvas->isChanged()) return goOn;
		int reply = QMessageBox::question(this, tr("Unsaved changes"),
				tr("The network has been modified. Do you want to save it?"),
			   	tr("Save"), tr("Discard"), tr("Abort"));
		if (reply == 2) return abort;
		if (reply == 1 ) return goOn;
		
		on_action_Save_triggered();
		return _canvas->isChanged()? abort : goOn;;
	}
	void load(const QString & filename)
	{
		std::string localFilename = filename.toLocal8Bit().constData();
		std::cout << "Loading " << localFilename << "..." << std::endl;
		clear();
		try
		{
			CLAM::XMLStorage::Restore(_network, localFilename);
		}
		catch(CLAM::XmlStorageErr &e)
		{
			QMessageBox::critical(this, tr("Error loading the network"), 
					tr("<p>An occurred while loading the network file.<p>"
						"<p><b>%1</b></p>").arg(e.what()));
			clear();
			return;
		}
		_canvas->loadNetwork(&_network);
		_canvas->loadPositions(filename+".pos"); //TODO: remove after embeded positions are released
		_canvas->loadGeometriesFromXML();
		appendRecentFile(filename);
		_networkFile = filename;
		updateCaption();
		// TODO: Update canvas
	}
	void save(const QString & filename)
	{
		std::string localFilename = filename.toLocal8Bit().constData();
		std::cout << "Saving " << localFilename << "..." << std::endl;
		if (!_saveUsingOldPosFiles) _canvas->updateGeometriesOnXML();
		CLAM::XMLStorage::Dump(_network, "network", localFilename);
		if (_saveUsingOldPosFiles) _canvas->savePositions(filename+".pos");
		_canvas->clearChanges();
		_networkFile = filename;
		appendRecentFile(filename);
		updateCaption();
	}
	void clear(bool isDummy=false)
	{
		_network.Stop();
		_networkFile = QString();
		_network.Clear();
		_canvas->loadNetwork(isDummy?0:&_network);
		updateCaption();
	}

	void closeEvent(QCloseEvent *event)
	{
		if (not askUserSaveChanges())
		{
			event->ignore();
			return;
		}
		QSettings settings;
		settings.setValue("RecentFiles",_recentFiles);
		settings.setValue("DockWindowsState", saveState());
		settings.setValue("EmbedSVGDiagramsOption",_canvas->getEmbedSVGDiagramsOption());
		settings.setValue("WhiteColorsForBoxes",ui.action_White_colors_Option->isChecked());
		event->accept();
	}

private:
	bool runQueuedCommands(TaskRunner::CommandsAndEnvironmentsList & commandsQList, const char * slotFinishedQueueName=NULL, const char * slotWidgetClosedName=NULL, bool stopOnError=true)
	{
		TaskRunner * runner = new TaskRunner();
		runner->setWindowTitle(tr("Faust compilation"));
		if (slotFinishedQueueName)
			connect(runner, SIGNAL(taskDone(bool)), this, slotFinishedQueueName);
		if (slotWidgetClosedName)
			connect(runner, SIGNAL(widgetDestructed()),this,slotWidgetClosedName);
		addDockWidget( Qt::BottomDockWidgetArea, runner);
		// Wait the window to be redrawn after the reconfiguration
		// before loading the cpu with the extractor
		qApp->processEvents();
		bool ok = runner->run(commandsQList,stopOnError);
		if (!ok)
			delete runner;
		return ok;
	}
public slots:
	void updateCaption()
	{
		setWindowTitle(tr("CLAM Network Editor - %1%2")
				.arg(_networkFile.isNull()?tr("Untitled"):_networkFile)
				.arg(_canvas->isChanged()?tr(" [modified]"):"")
				);
		updatePlayStatusIndicator();
	}
	void openFileWithExternalApplicationFromProcessing()
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile(_canvas->getFileNameToOpenWithExternalApplication()));
	}
	void endCompilationFaust(bool done)
	{
		ui.action_Compile_Faust_Modules->setEnabled(true);
		statusBar()->clearMessage();
		if (done)
			on_action_Reload_Faust_Modules_triggered();
	}

	void closeCompilationWidget()
	{
		ui.action_Compile_Faust_Modules->setEnabled(true);
	}

	void on_action_Embed_SVG_Diagrams_Option_changed()
	{
		QAction *action = qobject_cast<QAction *>(sender());
		if (!action) return;
		_canvas->setEmbedSVGDiagramsOption(action->isChecked());
	}
	void on_action_White_colors_Option_changed()
	{
		QAction *action = qobject_cast<QAction *>(sender());	
		// Change colors scheme
		if (action->isChecked())
			_canvas->setWhiteColorsForBoxes();		
		else
			_canvas->setGreenColorsForBoxes();
	}
	void on_action_Whats_this_triggered()
	{
		QWhatsThis::enterWhatsThisMode();
	}

	void on_action_Online_tutorial_triggered()
	{
		QString helpUrl = "http://iua-share.upf.es/wikis/clam/index.php/Network_Editor_tutorial";
		QDesktopServices::openUrl(helpUrl);
	}
	void on_action_About_triggered()
	{
		_aboutDialog->show();
	}
	void on_action_New_triggered()
	{
		if (!askUserSaveChanges()) return;
		clear();
	}
	void on_action_New_dummy_triggered()
	{
		if (!askUserSaveChanges()) return;
		clear(true);
	}
	void on_action_Open_triggered()
	{
		if (!askUserSaveChanges()) return;
		QString file = QFileDialog::getOpenFileName(this, "Choose a network file to open", "", networkFilter());
		if (file==QString::null) return;
		load(file);
	}
	void on_action_Open_example_triggered()
	{
		if (!askUserSaveChanges()) return;
		QString examplesPath;
#ifdef __APPLE__
		QDir dir(QApplication::applicationDirPath()+"/../Resources/example-data/");
		examplesPath =QString(dir.absolutePath());
#else
		examplesPath = DATA_EXAMPLES_PATH;
#endif
		QString file = QFileDialog::getOpenFileName(this, "Choose a network file to open", examplesPath, networkFilter());
		if (file==QString::null) return;
		load(file);
	}
	void on_action_OpenToolbar_triggered()
	{
		on_action_Open_triggered();
	}
	void openRecentTriggered()
	{
		QAction *action = qobject_cast<QAction *>(sender());
		if (!action) return;
		QString file = action->data().toString();
		if (file==QString::null) return;
		if (!askUserSaveChanges()) return;
		load(file);
	}
	void on_action_Save_triggered()
	{
		if (_networkFile.isNull()) on_action_Save_as_triggered();
		else save(_networkFile);
	}
	void on_action_Save_as_triggered()
	{
		QFileDialog fileDialog(this);
		fileDialog.setAcceptMode(QFileDialog::AcceptSave);
		fileDialog.setFileMode(QFileDialog::AnyFile);
//		fileDialog.setCaption("");
		fileDialog.selectFile(_networkFile);
		fileDialog.setFilter(networkFilter());
		fileDialog.setDefaultSuffix("clamnetwork");
		if (not fileDialog.exec()) return;
		
		QStringList files = fileDialog.selectedFiles();
		if (files.isEmpty()) return;
		if (files[0].endsWith("/.clamnetwork")) return;
		save(files[0]);
	}
	void on_action_Play_triggered()
	{
		if (_canvas->networkIsDummy() )
		{
			QMessageBox::critical(this, tr("Unable to play the network"), 
				tr("<p><b>Dummy networks are not playable.</b></p>"
				"<p>Dummy networks are used to draw arbitrary networks without"
				" real processings under the boxes, so you cannot play them.</p>"
				"<p>To have a playable network, create a new network or load an existing one.</p>"));
			return;
		}
		if (  _network.IsEmpty() )
		{
			QMessageBox::critical(this, tr("Unable to play the network"), 
					tr("<p><b>A network without processings is not playable.</b></p>"));
			return;
		}
		if (_network.HasMisconfiguredProcessings())
		{
			QMessageBox::critical(this, tr("Unable to play the network"), 
					tr("<p><b>Not all the processings are properly configured.</b></p>"
					));
			return;
		}
		if (_network.HasUnconnectedInPorts() )
		{
			QMessageBox::critical(this, tr("Unable to play the network"), 
					tr(
					"<p><b>Some inports in the network are not connected.</b></p>"
					"<p>To play the network you should connect the following inports.</p>"
					"<pre>%1</pre"
					).arg(_network.GetUnconnectedInPorts().c_str()));
			return;
		}
		if (! _networkPlayer->IsCallbackBased() && ! _network.HasSyncSource() )
		{
			QMessageBox::critical(this, tr("Unable to play the network"), 
				tr("<p>The network needs an AudioIn or AudioOut in order to be playable.</p>"));
			return;
		}
		_network.Start();
		updatePlayStatusIndicator();
	}
	void on_action_Stop_triggered()
	{
		_network.Stop();
		updatePlayStatusIndicator();
	}
	void on_action_Zoom_in_triggered()
	{
		_canvas->zoom(+1);
	}
	void on_action_Zoom_out_triggered()
	{
		_canvas->zoom(-1);
	}
	void on_action_Default_zoom_triggered()
	{
		_canvas->resetZoom();
	}
	void on_action_Edit_interface_triggered()
	{
		QMessageBox::warning(this, tr("Feature not implemented"),
			tr(
				"<p>Current NetworkEditor version does not implement launching the Qt designer from this buttom.</p>\n"
				"<p>Run the Qt designer and build an interface with the same filename than"
				" the network but changing the '.clamnetwork' extension to '.ui'</p>\n"
			));
	}
	void on_action_Run_prototyper_triggered()
	{
		QMessageBox::warning(this, tr("Feature not implemented"),
			tr(
				"<p>Current NetworkEditor version does not implement launching the Prototyper from this buttom.</p>\n"
				"<p>Run the Prototyper and open the same network you are editing</p>\n"
			));
	}
	void on_action_Compile_Faust_Modules_triggered()
	{
#if USE_LADSPA
		RunTimeFaustLibraryLoader faustLoader;
		typedef std::map<std::string,std::string> CommandsMap;
		std::cout << "[FAUST] \tcompiling" << std::endl;
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
		TaskRunner::CommandsAndEnvironmentsList commandsQList;
		TaskRunner::CommandAndEnvironment command;
		// define compilation using make:
		command.command="make";
		command.arguments=(QStringList() << QString("ladspa"));
		command.workingDir=faustDir.c_str();
		commandsQList.append(command);
		command.arguments=(QStringList() << QString("svg"));
		commandsQList.append(command);

		statusBar()->showMessage("Compiling faust modules...");
		const char * slotFinishedQueue = SLOT(endCompilationFaust(bool));
		const char * slotCloseWidget = SLOT(closeCompilationWidget());
		bool ok = runQueuedCommands(commandsQList,slotFinishedQueue,slotCloseWidget);
		if(!ok)
		{
			QMessageBox::critical(this, tr("Compiling Faust modules"),
				tr("<p><b>Error: Compilation failed.</b></p>\n"));
			statusBar()->clearMessage();
		}
#endif
	}
	void on_action_Reload_Faust_Modules_triggered()
	{
#if USE_LADSPA
		RunTimeFaustLibraryLoader faustLibraryLoader;
		faustLibraryLoader.Load();
		// delete the previous instance of processingtree
		delete(_processingTree);
		// and generate a new one
		_processingTree = new NetworkGUI::ProcessingTree(_processingTreeDock);
		_processingTreeDock->setWidget(_processingTree);
		addDockWidget(Qt::LeftDockWidgetArea, _processingTreeDock);
		// generate svg for faust code
		/*QString svgFilename;
		svgFilename += faustDir.c_str();
		on_action_Launch_Browser_triggered(svgFilename);

	#ifdef SVGWIDGET	
		QDockWidget * svgDockWidget = new QDockWidget(this);
		QSvgWidget * svgWidget = new QSvgWidget(svgFilename, svgDockWidget);
		svgDockWidget->setWidget(svgWidget);
		addDockWidget(Qt::RightDockWidgetArea, svgDockWidget);
	#endif*/
#endif
	}

	void on_action_Quit_triggered()
	{
		close();
	}

	void on_action_CompileAsLadspaPlugin_triggered()
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
		QString pluginTemplateCode = 
			"#include <CLAM/LadspaNetworkExporter.hxx>\n"
			"#include <CLAM/LadspaLibrary.hxx>\n"
			"static CLAM::LadspaLibrary library;\n"
			"CLAM_EXTERNAL_FILE_DATA(embededNetwork,\"myplugin.clamnetwork\")\n"
			"extern \"C\" const LADSPA_Descriptor * ladspa_descriptor(unsigned long index)\n"
			"{\n"
			"	static CLAM::LadspaNetworkExporter n2(library, embededNetwork, %1,\n"
			"			\"%2\", \"%3\",\n"
			"			\"%4\", \"%5\");\n"
			"	return library.pluginAt(index);\n"
			"}\n";
		QString pluginCode = pluginTemplateCode.arg(id).arg(label).arg(name).arg(maker).arg(copyright);
		std::cout << pluginCode.toStdString() << std::endl;
	}

private:
	ClamNetworkCanvas * _canvas;
	NetworkCanvas * _jackCanvas;
	QDialog * _aboutDialog;
	CLAM::Network _network;
	CLAM::NetworkPlayer * _networkPlayer;
	QString _networkFile;
	QLabel * _backendLabel;
	QLabel * _playingLabel;
	QStringList _recentFiles;
	
	//XML boxes geometries testing
	const bool _saveUsingOldPosFiles;

	QDockWidget * _processingTreeDock;
	NetworkGUI::ProcessingTree * _processingTree;
};

