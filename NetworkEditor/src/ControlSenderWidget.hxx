#ifndef ControlSenderWidget_hxx
#define ControlSenderWidget_hxx

#include <math.h>

#include <CLAM/Processing.hxx>
#include <QtGui/QSlider>
#include <QtGui/QDial>
#include <QtGui/QLabel>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHBoxLayout>
#include <CLAM/OutControlSender.hxx>

class ControlSenderWidgetParams;

class ControlSenderWidget : public QWidget
{
	Q_OBJECT
public:
	ControlSenderWidget(CLAM::Processing * processing);
	~ControlSenderWidget();

protected:
	void init(ControlSenderWidgetParams &params);

	void createLabel(const ControlSenderWidgetParams &params);
	void createDial(const ControlSenderWidgetParams &params);
	void createSlider(const ControlSenderWidgetParams &params);
	void createSpinBox(const ControlSenderWidgetParams &params);

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

