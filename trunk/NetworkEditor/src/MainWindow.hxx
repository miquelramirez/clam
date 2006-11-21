#include "uic_MainWindow.hxx"
#include "NetworkCanvas.hxx"
#include "ProcessingTree.hxx"
#include <QtGui/QDesktopServices>
#include <QtGui/QVBoxLayout>
#include <QtGui/QScrollArea>
#include <QtGui/QDockWidget>
#include <QtGui/QWhatsThis>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtCore/QProcess>
#include "uic_About.hxx"
#include <CLAM/Network.hxx>
#include <CLAM/NetworkPlayer.hxx>
#include <CLAM/PushFlowControl.hxx>
#include <CLAM/BlockingNetworkPlayer.hxx>
#include <CLAM/XMLStorage.hxx>
#include <CLAM/XmlStorageErr.hxx>
#include <CLAM/CLAMVersion.hxx>
#include "NetworkEditorVersion.hxx"

#ifdef USE_JACK
#include <CLAM/JACKNetworkPlayer.hxx>
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
		
		QDockWidget * dock = new QDockWidget(this);
		NetworkGUI::ProcessingTree * processingTree = new NetworkGUI::ProcessingTree(dock);
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

		int frameSize = 2048;
		_network.AddFlowControl( new CLAM::PushFlowControl( frameSize ));
		QString backend = "None";
		QString backendLogo = ":/icons/images/editdelete.png"; // TODO: Change this icon
		if (_networkPlayer) delete _networkPlayer;
		_networkPlayer = 0;
#ifdef USE_JACK
		CLAM::JACKNetworkPlayer * jackPlayer = new CLAM::JACKNetworkPlayer();
		backend = "JACK";
		backendLogo = ":/icons/images/jacklogo-mini.png";
		if ( jackPlayer->IsConnectedToServer())
			_networkPlayer = jackPlayer;
		else
			delete jackPlayer;
#endif
		if (! _networkPlayer)
		{
			backend = "ALSA";
			backendLogo = ":/icons/images/alsalogo-mini.png";
			_networkPlayer = new CLAM::BlockingNetworkPlayer();
		}
		_networkPlayer->SetNetwork(_network);

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
		else if (! _networkPlayer->IsStopped())
			_playingLabel->setText(tr("<p style='color:green'>Playing...</p>"));
		else
			_playingLabel->setText(tr("<p style='color:red'>Stopped</p>"));
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
		updateCaption();
	}
	void clear(bool isDummy=false)
	{
		_networkPlayer->Stop();
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
		QDesktopServices::openUrl(helpUrl);
//		QProcess::startDetached( "x-www-browser", QStringList() << helpUrl); // TODO: Remove this 4.1 unix only version
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
		QString file = QFileDialog::getOpenFileName(this, "Choose a network file to open", DATA_EXAMPLES_PATH, networkFilter());
		if (file==QString::null) return;
		load(file);
	}
	void on_action_Save_triggered()
	{
		if (_networkFile.isNull()) on_action_Save_as_triggered();
		else save(_networkFile);
	}
	void on_action_Save_as_triggered()
	{
        QString file = QFileDialog::getSaveFileName(this, "", _networkFile, networkFilter());
        if (file==QString::null) return;
		save(file);
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
		_networkPlayer->Start();
		updatePlayStatusIndicator();
	}
	void on_action_Stop_triggered()
	{
		_networkPlayer->Stop();
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
private:
	NetworkCanvas * _canvas;
	QDialog * _aboutDialog;
	CLAM::Network _network;
	CLAM::NetworkPlayer * _networkPlayer;
	QString _networkFile;
	QLabel * _backendLabel;
	QLabel * _playingLabel;
};


