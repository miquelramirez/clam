#include "uic_MainWindow.hxx"
#include "NetworkCanvas.hxx"
#include "ProcessingTree.hxx"
#include <QtGui/QVBoxLayout>
#include <QtGui/QScrollArea>
#include <QtGui/QDockWidget>
#include <QtGui/QWhatsThis>
#include <QtCore/QProcess>
#include "uic_About.hxx"
#include <CLAM/Network.hxx>
#include <CLAM/NetworkPlayer.hxx>
#include <CLAM/CLAMVersion.hxx>
#include "NetworkEditorVersion.hxx"

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

		QScrollArea * w = new QScrollArea(this);
		w->setWidgetResizable(true);
		_canvas = new NetworkCanvas;
		setCentralWidget(w);
		w->setWidget(_canvas);
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


		connect(ui.action_Show_processing_toolbox, SIGNAL(toggled(bool)), dock, SLOT(setVisible(bool)));
		connect(ui.action_Print, SIGNAL(triggered()), _canvas, SLOT(print()));
	}
public slots:
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
		// TODO: Choose the network type (jack, alsa...)?
		// TODO: Set the filename to null
		// TODO: Erase the CLAM network
		_canvas->clear();
	}
private:
	NetworkCanvas * _canvas;
	QDialog * _aboutDialog;
	CLAM::Network * _network;
	CLAM::NetworkPlayer * _networkPlayer;
};


