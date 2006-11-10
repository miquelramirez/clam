#ifndef ControlSenderWidget_hxx
#define ControlSenderWidget_hxx


#include <CLAM/Processing.hxx>
#include <QtGui/QSlider>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHBoxLayout>
#include <CLAM/OutControlSender.hxx>

class ControlSenderWidget : public QWidget
{
	Q_OBJECT
public:
	ControlSenderWidget(CLAM::Processing * processing)
	{
		_sender = dynamic_cast<CLAM::OutControlSender* >(processing);
		CLAM_ASSERT(_sender,
				"ControlSenderWidget works just for OutControlSender processings.");
		setLayout(new QHBoxLayout);
		layout()->setMargin(1);

		_slider = new QSlider(Qt::Horizontal);
		_slider->setMinimumHeight(_slider->sizeHint().height());
		layout()->addWidget(_slider);
		_spinBox = new QDoubleSpinBox();
		layout()->addWidget(_spinBox);

		const CLAM::OutControlSenderConfig * config = 
			dynamic_cast<const CLAM::OutControlSenderConfig *>(&_sender->GetConfig());
		CLAM_ASSERT( config, "Unexpected Configuration type for an OutControlSender" );

		_min = config->GetMin();
		_default = config->GetDefault();
		_max = config->GetMax();
		_step = config->GetStep();

		_spinBox->setMinimum(_min);
		_spinBox->setMaximum(_max);
		_spinBox->setValue(_default);

		int nSteps = (_max-_min)/_step;

		_slider->setMinimum(0);
		_slider->setMaximum(nSteps);
		_slider->setValue((_default-_min)/_step);
		connect(_slider, SIGNAL(valueChanged(int)), 
				this, SLOT(stepControlChanged(int)));
		connect(_spinBox, SIGNAL(valueChanged(double)), 
				this, SLOT(continuousControlChanged(double)));
	}
	~ControlSenderWidget();
private slots:
	void stepControlChanged(int value)
	{
		double dvalue = _min + value*_step;
		_slider->setValue(value);
		_spinBox->setValue(dvalue);
		_sender->GetOutControls().Get( "out" ).SendControl( dvalue );
	}
	void continuousControlChanged(double value)
	{
		_slider->setValue((value-_min)/_step);
		_spinBox->setValue(value);
		_sender->GetOutControls().Get( "out" ).SendControl( value );
	}
private:
	QSlider * _slider;
	QDoubleSpinBox * _spinBox;
	CLAM::OutControlSender * _sender;
	double _min;
	double _default;
	double _max;
	double _step;
};





#endif//ControlSenderWidget_hxx

