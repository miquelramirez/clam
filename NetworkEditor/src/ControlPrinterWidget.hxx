#ifndef ControlPrinterWidget_hxx
#define ControlPrinterWidget_hxx

#include <CLAM/Processing.hxx>
#include <QtGui/QWidget>
#include <vector>

class QLabel;

class ControlPrinterWidget : public QWidget
{
	Q_OBJECT
public:
	ControlPrinterWidget(CLAM::Processing *processing);
	~ControlPrinterWidget();

protected:
	void timerEvent(QTimerEvent *event);

private:
	void updateLabels();
	CLAM::Processing *_processing;
	std::vector<QLabel *> _labels;
};

#endif //ControlPrinterWidget_hxx


