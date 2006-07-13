#include "uic_MainWindow.hxx"
#include "NetworkCanvas.hxx"
#include <QtGui/QVBoxLayout>
#include <QtGui/QScrollArea>

class MainWindow : public QMainWindow
{
	Q_OBJECT
	Ui::MainWindow ui;
public:
	virtual ~MainWindow();
	MainWindow()
	{
		ui.setupUi(this);
		QScrollArea * w = new QScrollArea(this);
		w->setWidgetResizable(true);
		NetworkCanvas * canvas = new NetworkCanvas;
		setCentralWidget(w);
		w->setWidget(canvas);
	}
};


