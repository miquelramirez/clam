#ifndef ControlSenderWidget_hxx
#define ControlSenderWidget_hxx

#include <math.h>

#include <CLAM/Processing.hxx>
#include <CLAM/OutControlSender.hxx>
#include <QtGui/QWidget>

class QDial;
class QSlider;
class QDoubleSpinBox;
class QAbstractSlider;

class ControlSenderWidget : public QWidget
{
	Q_OBJECT
public:
	ControlSenderWidget(CLAM::Processing * processing);
	~ControlSenderWidget();

protected:
	void init();

	void createLabel();
	void createDial();
	void createSlider(Qt::Orientation);
	void createSpinBox();
	void setupSlider(QAbstractSlider *slider);

private slots:
	void stepControlChanged(int value);
	void continuousControlChanged(double value);

private:
	QDial * _dial;
	QSlider * _slider;
	QDoubleSpinBox * _spinBox;
	CLAM::OutControlSender * _sender;
	double _min;
	double _default;
	double _max;
	double _step;
	bool _updating;
};

#endif//ControlSenderWidget_hxx

