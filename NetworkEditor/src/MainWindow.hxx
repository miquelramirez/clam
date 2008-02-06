#include "uic_MainWindow.hxx"
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


#define SVGWIDGET //this macro is for debugging
#ifdef SVGWIDGET
	#include <QtSvg/QSvgWidget>
#endif
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
		: _networkPlayer(0)
	{
		ui.setupUi(this);
		setWindowIcon(QIcon(":/icons/images/NetworkEditor-icon.png"));

		QScrollArea * scroll = new QScrollArea(this);
		_canvas = new NetworkCanvas;
		setCentralWidget(scroll);
		scroll->setWidget(_canvas);
		
		dock = new QDockWidget(this);
		processingTree = new NetworkGUI::ProcessingTree(dock);
		dock->setWindowTitle("Processing Toolbox");
		dock->setWidget(processingTree);
		addDockWidget(Qt::LeftDockWidgetArea, dock);

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

		_network.AddFlowControl( new CLAM::NaiveFlowControl );
		QString backend = "None";
		QString backendLogo = ":/icons/images/editdelete.png"; // TODO: Change this icon
		if (_networkPlayer) delete _networkPlayer;
		_networkPlayer = 0;
#ifdef USE_JACK
		CLAM::JACKNetworkPlayer * jackPlayer = new CLAM::JACKNetworkPlayer();
		backend = "JACK";
		backendLogo = ":/icons/images/jacklogo-mini.png";
		if ( jackPlayer->IsWorking())
			_networkPlayer = jackPlayer;
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

		connect(ui.action_Show_processing_toolbox, SIGNAL(toggled(bool)), dock, SLOT(setVisible(bool)));
		connect(ui.action_Print, SIGNAL(triggered()), _canvas, SLOT(print()));
		connect(_canvas, SIGNAL(changed()), this, SLOT(updateCaption()));
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
		std::cout << "Loading " << filename.toStdString() << "..." << std::endl;
		clear();
		try
		{
			CLAM::XMLStorage::Restore(_network, filename.toStdString());
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
		_canvas->loadPositions(filename+".pos");
		appendRecentFile(filename);
		_networkFile = filename;
		updateCaption();
		// TODO: Update canvas
	}
	void save(const QString & filename)
	{
		std::cout << "Saving " << filename.toStdString() << "..." << std::endl;
		CLAM::XMLStorage::Dump(_network, "network", filename.toStdString());
		_canvas->savePositions(filename+".pos");
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
		if (askUserSaveChanges()) event->accept();
		else event->ignore();
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
	void on_action_Whats_this_triggered()
	{
		QWhatsThis::enterWhatsThisMode();
	}
	void on_action_Online_tutorial_triggered()
	{
		QString helpUrl = "http://iua-share.upf.es/wikis/clam/index.php/Network_Editor_tutorial";
		#if QT_VERSION >= 0x040200
		QDesktopServices::openUrl(helpUrl);
		#else
		QProcess::startDetached( "x-www-browser", QStringList() << helpUrl); // TODO: Remove this 4.1 unix only version
		#endif
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
		fileDialog.exec();
		
        QStringList files = fileDialog.selectedFiles();
        if (files.isEmpty()) return;
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
		// TODO: Activate this once it works
		if ( false && _network.HasUnconnectedInPorts() )
		{
			QMessageBox::critical(this, tr("Unable to play the network"), 
					tr("<p><b>The network has some in ports which are not connected.</b></p>"
					"<p>All in ports must be feeded in order to play the network</p>"
					));
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
	void on_action_Compile_Faust_triggered()
	{
		QMessageBox::warning(this, tr("Faust loading (icomplete feature)"),
			tr(
				"<p>This action reloads all Faust modules compiled as LADSPA found in FAUST_PATH environment var.</p>\n"
				"<p>More Faust related features like processing box embedded diagrams will come soon.</p>\n"
			));
		// clear the current map of ladspa's
#if USE_LADSPA	
		RunTimeFaustLibraryLoader faustLoader;
		faustLoader.Load();
		// delete the previous instance of processingtree
		delete(processingTree);
		// and generate a new one
		processingTree = new NetworkGUI::ProcessingTree(dock);
		dock->setWidget(processingTree);
		addDockWidget(Qt::LeftDockWidgetArea, dock);

#endif
		// compile all faust files
		// TODO: this is debugging code:
#if 0
		std::cout << "[FAUST] compiling" << std::endl;
		std::string faustDir="~/src/faust/examples";
		std::string compileFaustsCmd = "cd "+faustDir+
			" && make clean && make ladspa && make svg";
		QProcess process;
		process.start(compileFaustsCmd.c_str());
		std::cout << "executing: "<< compileFaustsCmd << std::endl;
		process.waitForFinished();
		// generate svg for faust code
		QString svgFilename;
		svgFilename += faustDir.c_str();
		svgFilename += "/freeverb.dsp-svg/process.svg";
		#if QT_VERSION >= 0x040200
		std::cout << "opening "<<svgFilename.toStdString() << std::endl;
		QDesktopServices::openUrl(svgFilename);
		#else
		QProcess::startDetached( "x-www-browser", QStringList() << svgFilename); // TODO: Remove this 4.1 unix only version
		#endif
	#ifdef SVGWIDGET	
		QDockWidget * svgDockWidget = new QDockWidget(this);
		QSvgWidget * svgWidget = new QSvgWidget(svgFilename, svgDockWidget);
		svgDockWidget->setWidget(svgWidget);
		addDockWidget(Qt::RightDockWidgetArea, svgDockWidget);
	#endif
#endif
	}
	void on_action_Quit_triggered()
	{
		close();
	}
private:
	NetworkCanvas * _canvas;
	QDialog * _aboutDialog;
	CLAM::Network _network;
	CLAM::NetworkPlayer * _networkPlayer;
	QString _networkFile;
	QLabel * _backendLabel;
	QLabel * _playingLabel;
	QStringList _recentFiles;

	// faust testing
	QDockWidget * dock;
	NetworkGUI::ProcessingTree * processingTree;
};


