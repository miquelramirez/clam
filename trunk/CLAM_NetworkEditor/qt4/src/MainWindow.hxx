#include "uic_MainWindow.hxx"
#include "NetworkCanvas.hxx"
#include "ProcessingTree.hxx"
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

#ifndef DATA_EXAMPLES_PATH
// TODO: QT4PORT without the ../
#define DATA_EXAMPLES_PATH "../example-data"
#endif


class MainWindow : public QMainWindow
{
	Q_OBJECT
	Ui::MainWindow ui;
public:
	virtual ~MainWindow();
	MainWindow()
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
		_networkPlayer = new CLAM::BlockingNetworkPlayer(); // TODO: Delete this on destruction
		_networkPlayer->SetNetwork(_network);

		connect(ui.action_Show_processing_toolbox, SIGNAL(toggled(bool)), dock, SLOT(setVisible(bool)));
		connect(ui.action_Print, SIGNAL(triggered()), _canvas, SLOT(print()));
		connect(_canvas, SIGNAL(changed()), this, SLOT(updateCaption()));
		updateCaption();

	}

	QString networkFilter() {return tr("CLAM Network files (*.clamnetwork)"); }

	bool askUserSaveChanges()
	{
		bool goOn = true;
		bool abort = false;
		if (not _canvas->isChanged()) return goOn;
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
	void clear()
	{
		_networkPlayer->Stop();
		_networkFile = QString();
		_network.Clear();
		_canvas->loadNetwork(&_network);
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
	}
	void on_action_Whats_this_triggered()
	{
		QWhatsThis::enterWhatsThisMode();
	}
	void on_action_Online_tutorial_triggered()
	{
		QString helpUrl = "http://iua-share.upf.es/wikis/clam/index.php/Network_Editor_tutorial";

		// TODO: To be multiplatform, enable this when migrating to 4.2
		// QDesktopServices::openUrl(helpUrl);
		QProcess::startDetached( "x-www-browser", QStringList() << helpUrl);
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
				tr("<p>Dummy networks are not playable.</p>"
				"<p>Dummy networks are used to draw arbitrary networks without"
				" real processings under the boxes, so you cannot play them.</p>"
				"<p>To have a playable network, create a new network or load an existing one.</p>"));
			return;
		}
		if (  _network.IsEmpty() )
		{
			QMessageBox::critical(this, tr("Unable to play the network"), 
					tr("<p>A network without processings is not playable.</p>"));
			return;
		}
		if (not _networkPlayer->IsCallbackBased() and not _network.HasSyncSource() )
		{
			QMessageBox::critical(this, tr("Unable to play the network"), 
				tr("<p>The network needs an AudioIn or AudioOut in order to be playable.</p>"));
			return;
		}
		_networkPlayer->Start();
	}
	void on_action_Stop_triggered()
	{
		_networkPlayer->Stop();
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
};


