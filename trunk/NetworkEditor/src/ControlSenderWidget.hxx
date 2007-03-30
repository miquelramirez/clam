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

class ControlSenderWidget : public QWidget
{
	Q_OBJECT
public:
	ControlSenderWidget(CLAM::Processing * processing)
	{
		_sender = dynamic_cast<CLAM::OutControlSender* >(processing);
		CLAM_ASSERT(_sender, "ControlSenderWidget works just for OutControlSender processings.");
		const CLAM::OutControlSenderConfig * config = 
			dynamic_cast<const CLAM::OutControlSenderConfig *>(&_sender->GetConfig());
		CLAM_ASSERT( config, "Unexpected Configuration type for an OutControlSender" );

		_min = config->GetMin();
		_default = config->GetDefault();
		_max = config->GetMax();
		_step = config->GetStep();

		bool wantLabel = false, wantDial = false;
		bool wantSlider = false, wantSpinBox = false;
		Qt::Orientation sliderAlign = Qt::Horizontal;
		
		CLAM::OutControlSenderConfig::EControlRepresentation rep =
				config->GetControlRepresentation();
		switch (rep.GetValue()) {
		case CLAM::OutControlSenderConfig::EControlRepresentation::eUndetermined: {
			// no GUI!
			wantLabel = true;
			break;
		}
		case CLAM::OutControlSenderConfig::EControlRepresentation::eKnot:
			wantSpinBox = wantDial = true;
			sliderAlign = Qt::Vertical;
			break;
		case CLAM::OutControlSenderConfig::EControlRepresentation::eHorizontalSlider:
			wantSpinBox = wantSlider = true;
			break;
		case CLAM::OutControlSenderConfig::EControlRepresentation::eVerticalSlider:
			wantSpinBox = wantSlider = true;
			sliderAlign = Qt::Vertical;
			break;
		case CLAM::OutControlSenderConfig::EControlRepresentation::eSpinBox:
			wantSpinBox = true;
			break;
		}

		if (sliderAlign == Qt::Vertical) 
			setLayout(new QVBoxLayout);
		else
			setLayout(new QHBoxLayout);
		layout()->setMargin(1);

		if (wantLabel) {
			QLabel *label = new QLabel();
			label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			label->setNum(_default);
			label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
			layout()->addWidget(label);
		}

		if (wantDial) {
			_dial = new QDial();
			setupSlider(_dial);
			_dial->setNotchesVisible(true);
			layout()->addWidget(_dial);
		} else {
			_dial = NULL;
		}
		if (wantSlider) {
			_slider = new QSlider(sliderAlign);
			setupSlider(_slider);
			if (sliderAlign == Qt::Vertical) {
				QWidget *hbox = new QWidget;
				hbox->setLayout(new QHBoxLayout);
				hbox->layout()->addWidget(_slider);
				layout()->addWidget(hbox);
			}
			else
				layout()->addWidget(_slider);
		} else {
			_slider = NULL;
		}
		if (wantSpinBox) {
			_spinBox = new QDoubleSpinBox();
			layout()->addWidget(_spinBox);

			_spinBox->setMinimum(_min);
			_spinBox->setMaximum(_max);
			_spinBox->setSingleStep(_step);
			_spinBox->setValue(_default);

			connect(_spinBox, SIGNAL(valueChanged(double)), 
				this, SLOT(continuousControlChanged(double)));
		} else {
			_spinBox = NULL;
		}
		_updating = false;
	}
	~ControlSenderWidget();

private:
	void setupSlider(QAbstractSlider *slider) {
		slider->setMinimumHeight(slider->sizeHint().height());
		slider->setMinimumWidth(slider->sizeHint().width());

		slider->setMinimum(0);
		slider->setMaximum(int(ceil((_max - _min) / _step)));
		slider->setValue(int(round((_default - _min) / _step)));

		connect(slider, SIGNAL(valueChanged(int)), 
				this, SLOT(stepControlChanged(int)));
	}

private slots:
	void stepControlChanged(int value)
	{
		if (_updating) return;
		_updating = true;
		double dvalue = _min + _step * value;
		if (_spinBox) _spinBox->setValue(dvalue);
		_sender->SendControl(dvalue);
		_updating = false;
	}
	void continuousControlChanged(double value)
	{
		if (_updating) return;
		_updating = true;
		int svalue = int(round((value - _min) / _step));
		if (_slider) _slider->setValue(svalue);
		if (_dial) _dial->setValue(svalue);
		_sender->SendControl(value);
		_updating = false;
	}
private:
	QSlider * _slider;
	QDoubleSpinBox * _spinBox;
	QDial * _dial;
	CLAM::OutControlSender * _sender;
	double _min;
	double _default;
	double _max;
	double _step;
	bool _updating;
};

#endif//ControlSenderWidget_hxx

