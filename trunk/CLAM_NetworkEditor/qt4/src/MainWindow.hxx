#include "uic_MainWindow.hxx"
#include "NetworkCanvas.hxx"
#include "ProcessingTree.hxx"
#include <QtGui/QVBoxLayout>
#include <QtGui/QScrollArea>
#include <QtGui/QDockWidget>
#include <QtGui/QWhatsThis>

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
		NetworkCanvas * canvas = new NetworkCanvas;
		setCentralWidget(w);
		w->setWidget(canvas);
		QDockWidget * dock = new QDockWidget(this);
		NetworkGUI::ProcessingTree * processingTree = new NetworkGUI::ProcessingTree(dock);
		dock->setWidget(processingTree);
		addDockWidget(Qt::LeftDockWidgetArea, dock);

		connect(ui.action_Enable_processings_menu, SIGNAL(toggled(bool)), dock, SLOT(setVisible(bool)));
		connect(ui.action_Print, SIGNAL(triggered()), canvas, SLOT(print()));
	}
public slots:
	void on_action_Whats_this_triggered()
	{
		std::cout << "Whats" << std::endl;
		QWhatsThis::enterWhatsThisMode();
	}
};


