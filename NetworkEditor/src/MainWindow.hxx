#include "ui_MainWindow.hxx"
#include "ClamNetworkCanvas.hxx"
#include "ProcessingTree.hxx"
#include <QtGui/QVBoxLayout>
#include <QtGui/QScrollArea>
#include <QtGui/QDockWidget>
#include <QtGui/QWhatsThis>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QTimer>
#include "ui_About.hxx"
#include <CLAM/Network.hxx>
#include <CLAM/NetworkPlayer.hxx>
#include <CLAM/NaiveFlowControl.hxx>
#include <CLAM/XMLStorage.hxx>
#include <CLAM/XmlStorageErr.hxx>
#include <CLAM/CLAMVersion.hxx>
#include "NetworkEditorVersion.hxx"
#include "RichTextEditor.hxx"

// copied from Annotator:
#include "TaskRunner.hxx"

#if QT_VERSION >= 0x040400
#include <QtWebKit/QWebView>
#endif
#include <QtSvg/QSvgWidget>
#include <QtCore/QProcess>
#include <QtGui/QDesktopServices>

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

class PlaybackIndicator : public QLabel
{
Q_OBJECT
public:
	PlaybackIndicator()
		: _network(0)
	{
	}
	void setNetwork(CLAM::Network * network)
	{
		_network = network;
	}
	void updatePlayStatus()
	{
		if (not _network)
			setText(tr("<p style='color:blue'>Dummy</p>"));
		else if (_network->IsPlaying())
			setText(tr("<p style='color:green'>Playing...</p>"));
		else if (_network->IsPaused())
			setText(tr("<p style='color:orange;text-decoration:blink'>Paused</p>"));
		else
			setText(tr("<p style='color:red'>Stopped</p>"));
	}
private:
	CLAM::Network * _network;
};

class MainWindow : public QMainWindow
{
	Q_OBJECT
	Ui::MainWindow ui;
public:
	virtual ~MainWindow();

	MainWindow()
		: _networkPlayer(0)
		, _clientName("CLAM network player")
	{
		ui.setupUi(this);
		setWindowIcon(QIcon(":/icons/images/NetworkEditor-icon.png"));
#ifdef AFTER13RELEASE
		QTabWidget * centralTab = new QTabWidget(this);
		setCentralWidget(centralTab);
		centralTab->setTabPosition(QTabWidget::South);
#endif//AFTER13RELEASE
//		QScrollArea * scroll = new QScrollArea(this);
		_canvas = new ClamNetworkCanvas;
		ClamNetworkCanvas * scroll = _canvas;
//		scroll->setWidget(_canvas);

#ifdef AFTER13RELEASE
		QScrollArea * backendScroll = new QScrollArea(this);
		centralTab->addTab(scroll, "Network");
		centralTab->addTab(backendScroll, "Jack");
#else
		setCentralWidget(scroll);
#endif//AFTER13RELEASE
		_processingTreeDock = new QDockWidget(this);
		_processingTree = new NetworkGUI::ProcessingTree(_processingTreeDock);
		_processingTreeDock->setObjectName("ProcessingTree");
		_processingTreeDock->setWindowTitle(tr("Processing Toolbox"));
		_processingTreeDock->setWidget(_processingTree);
		addDockWidget(Qt::LeftDockWidgetArea, _processingTreeDock);

		// add a Description Panel for description of networks
		_descriptionPanel = new QDockWidget(this);
		_descriptionPanel->setObjectName("Description");
		_descriptionPanel->setWindowTitle(tr("Description"));
		_textDescriptionEdit = new RichTextEditor(_descriptionPanel);
		_descriptionPanel->setWidget(_textDescriptionEdit);
		addDockWidget(Qt::LeftDockWidgetArea, _descriptionPanel);

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

#ifdef USE_LADSPA
		ui.menuFaust->setEnabled(true);
		ui.action_Compile_Faust_Modules->setEnabled(true);
#endif

		_playingLabel = new PlaybackIndicator;
		statusBar()->addPermanentWidget(_playingLabel);
		_backendLabel = new QLabel;
		statusBar()->addPermanentWidget(_backendLabel);

		periodicPlayStatusUpdate(); // Should be directly called just once

		connect(ui.action_Show_processing_toolbox, SIGNAL(toggled(bool)), _processingTreeDock, SLOT(setVisible(bool)));
		connect(ui.action_Show_description_panel, SIGNAL(toggled(bool)), _descriptionPanel, SLOT(setVisible(bool)));

		connect(ui.action_Print, SIGNAL(triggered()), _canvas, SLOT(print()));
		connect(_canvas, SIGNAL(changed()), this, SLOT(updateCaption()));
		connect(_canvas, SIGNAL(browseUrlRequest(const QString&)),this,SLOT(browseUrlInternalFromProcessing(const QString &)));
		connect(_textDescriptionEdit, SIGNAL(textChanged()), this, SLOT(updateNetworkDescription()));
		updateCaption();

	}
	void updatePlayStatusIndicator()
	{
		ui.action_Play->setEnabled(not _network.IsPlaying());
		ui.action_Stop->setEnabled(not _network.IsStopped());
		ui.action_Pause->setEnabled(_network.IsPlaying());
		_playingLabel->updatePlayStatus();
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
		_network.ResetConnectionReport();

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
		_textDescriptionEdit->setText(QString::fromLocal8Bit(_network.GetDescription().c_str()));

		_playingLabel->setNetwork(&_network);
		_canvas->loadNetwork(&_network);
		_canvas->loadGeometriesFromXML();

		CLAM::Network::ConnectionState connectionState = _network.GetConnectionReport();
		if (connectionState.first)
		{
			QMessageBox::warning(this, tr("Old clamnetwork file detected"), 
				tr("<p>The clamnetwork file is older than 1.3.1, the following connections are broken.<p>" 
					"<p><b>%1</b></p>").arg(connectionState.second.c_str()));
		}

		appendRecentFile(filename);
		_networkFile = filename;
		updateCaption();
	}
	void save(const QString & filename)
	{
		std::string localFilename = filename.toLocal8Bit().constData();
		std::cout << "Saving " << localFilename << "..." << std::endl;
		_canvas->updateGeometriesOnXML();
		CLAM::XMLStorage::Dump(_network, "network", localFilename);
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
		_playingLabel->setNetwork(isDummy?0:&_network);
		_canvas->loadNetwork(isDummy?0:&_network);
		updateCaption();
	}

	void setClientName(const std::string & clientName)
	{
		_clientName=clientName;
	}

	void setBackend(QString &backend)
	{
		QString backendLogo = ":/icons/images/editdelete.png"; // TODO: Change this icon
		if (_networkPlayer) delete _networkPlayer;
		_networkPlayer = 0;

#ifdef USE_JACK
		if (backend=="JACK" || backend=="Auto")
		{
			CLAM::JACKNetworkPlayer *jackPlayer = new CLAM::JACKNetworkPlayer(_clientName);
			if ( jackPlayer->IsWorking() )
			{
				backend = "JACK";
				backendLogo = ":/icons/images/jacklogo-mini.png";
				_networkPlayer = jackPlayer;
#ifdef AFTER13RELEASE
				_jackCanvas = new ClamNetworkCanvas; // TODO: This should be a JackNetworkCanvas
				backendScroll->setWidget(_jackCanvas);
#endif//AFTER13RELEASE
			}
			else
				delete jackPlayer;
		}
#endif

#ifdef USE_PORTAUDIO
		if (backend=="PortAudio" || backend=="Auto")
		{
			if (! _networkPlayer)
			{
				backend = "PortAudio";
				backendLogo = ":/icons/images/portaudiologo-mini.png";
				_networkPlayer = new CLAM::PANetworkPlayer();
			}
		}
#endif

		CLAM_ASSERT(_networkPlayer!=0, "Problem setting the backend.");
		if (_networkPlayer==0) backend = "None";
		_network.SetPlayer( _networkPlayer );

		_backendLabel->setToolTip(tr("<p>Audio Backend: %1</p>").arg(backend));
		_backendLabel->setPixmap(QPixmap(backendLogo));

		updatePlayStatusIndicator();
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
		settings.setValue("EmbedSVGDiagramsOption",_canvas->embedSVGDiagramsOption());
		settings.setValue("WhiteColorsForBoxes",ui.action_White_colors_Option->isChecked());
		event->accept();
	}

public slots:
	// Do not call this slot but by the timer, call updatePlayStatusIndicator instead
	void periodicPlayStatusUpdate()
	{
		updatePlayStatusIndicator();
		QTimer::singleShot(500, this, SLOT(periodicPlayStatusUpdate()));
	}
	void updateCaption()
	{
		setWindowTitle(tr("CLAM Network Editor - %1%2")
				.arg(_networkFile.isNull()?tr("Untitled"):_networkFile)
				.arg(_canvas->isChanged()?tr(" [modified]"):"")
				);
		updatePlayStatusIndicator();
	}

	void browseUrlInternalFromProcessing(const QString & fileName)
	{
#if QT_VERSION >= 0x040400
		QDockWidget * browser=new QDockWidget(this);
		QWebView * view=new QWebView(browser);
		view->setContextMenuPolicy(Qt::NoContextMenu);
		view->load(fileName);
		browser->setObjectName(tr("Internal Browser"));
		browser->setWidget(view);
		browser->setWindowTitle(tr("Browsing %1").arg(fileName));
		addDockWidget(Qt::BottomDockWidgetArea,browser);
#endif
	}

	void updateNetworkDescription()
	{
		QString text(_textDescriptionEdit->toHtml());
	
		if(!_canvas->isChanged())
			_canvas->markAsChanged();
		
		_network.SetDescription(text.toLocal8Bit().constData());
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
		QString helpUrl = "http://clam-project.org/wiki/Network_Editor_tutorial";
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
		_canvas->updateGeometriesOnXML();
		_network.Start();
		_network.GetAndClearGeometries();
		updatePlayStatusIndicator();
	}
	void on_action_Stop_triggered()
	{
		_network.Stop();
		updatePlayStatusIndicator();
	}
	void on_action_Pause_triggered()
	{
		_network.Pause();
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
	void on_action_Compile_Faust_Modules_triggered();
	void endCompilationFaust(bool done);
	void closeCompilationWidget();
	void on_action_Reload_Faust_Modules_triggered();

	void on_action_Quit_triggered()
	{
		close();
	}

	void on_action_CompileAsLadspaPlugin_triggered();

private:
	ClamNetworkCanvas * _canvas;
	NetworkCanvas * _jackCanvas;
	QDialog * _aboutDialog;
	CLAM::Network _network;
	CLAM::NetworkPlayer * _networkPlayer;
	std::string _clientName;
	QString _networkFile;
	QLabel * _backendLabel;
	PlaybackIndicator * _playingLabel;
	QStringList _recentFiles;
	QDockWidget * _descriptionPanel;
	RichTextEditor * _textDescriptionEdit; 
	
	QDockWidget * _processingTreeDock;
	NetworkGUI::ProcessingTree * _processingTree;
};

