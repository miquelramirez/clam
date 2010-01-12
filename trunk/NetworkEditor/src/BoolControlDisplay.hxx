#ifndef BoolControlDisplay_hxx
#define BoolControlDisplay_hxx

#include <CLAM/Processing.hxx>
#include <QtGui/QWidget>
#include <vector>

class QLabel;

class BoolControlDisplay : public QWidget
{
	Q_OBJECT
public:
	BoolControlDisplay(CLAM::Processing *processing);
	~BoolControlDisplay();

protected:
	void timerEvent(QTimerEvent *event);

private:
	CLAM::Processing *_processing;
	std::vector<QLabel *> _labels;
	QPixmap _ledOn;
	QPixmap _ledOff;
};

#endif //BoolControlDisplay_hxx


