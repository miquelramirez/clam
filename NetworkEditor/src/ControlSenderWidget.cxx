#include "ControlSenderWidget.hxx"
#include <QtGui/QSlider>
#include <QtGui/QDial>
#include <QtGui/QLabel>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHBoxLayout>
#include "QSynthKnob.hxx"

ControlSenderWidget::ControlSenderWidget(CLAM::Processing * processing)
	: _dial(0)
	, _slider(0)
	, _spinBox(0)
	, _updating(false)
{
	_sender = dynamic_cast<CLAM::OutControlSender* >(processing);
	CLAM_ASSERT(_sender, "ControlSenderWidget only works "
				"with OutControlSender processings.");
	init();
}

ControlSenderWidget::~ControlSenderWidget()
{
}

void ControlSenderWidget::init()
{
	const CLAM::OutControlSenderConfig * config = 
		dynamic_cast<const CLAM::OutControlSenderConfig *>(&_sender->GetConfig());
	CLAM_ASSERT( config, "Unexpected Configuration type for an OutControlSender" );

	_min = config->GetMin();
	_default = config->GetDefault();
	_max = config->GetMax();
	_step = config->GetStep();

	switch (config->GetControlRepresentation().GetValue()) {
	case CLAM::OutControlSenderConfig::EControlRepresentation::eUndetermined:
		break;
	case CLAM::OutControlSenderConfig::EControlRepresentation::eKnot:
		setLayout(new QVBoxLayout);
		createDial();
		createSpinBox();
		break;
	case CLAM::OutControlSenderConfig::EControlRepresentation::eHorizontalSlider:
		setLayout(new QHBoxLayout);
		createSlider(Qt::Horizontal);
		createSpinBox();
		break;
	case CLAM::OutControlSenderConfig::EControlRepresentation::eVerticalSlider:
		setLayout(new QVBoxLayout);
		createSlider(Qt::Vertical);
		createSpinBox();
		break;
	case CLAM::OutControlSenderConfig::EControlRepresentation::eSpinBox:
		setLayout(new QHBoxLayout);
		createSpinBox();
		break;
	}
	layout()->setMargin(1);
}

void ControlSenderWidget::createDial()
{
	_dial = new QSynthKnob;
	setupSlider(_dial);
	_dial->setNotchesVisible(true);
	layout()->addWidget(_dial);
}

void ControlSenderWidget::createSlider(Qt::Orientation align)
{
	_slider = new QSlider(align);
	setupSlider(_slider);
	if (align == Qt::Vertical) {
		QWidget *hbox = new QWidget;
		hbox->setLayout(new QHBoxLayout);
		hbox->layout()->addWidget(_slider);
		layout()->addWidget(hbox);
	}
	else
		layout()->addWidget(_slider);
}

void ControlSenderWidget::setupSlider(QAbstractSlider *slider)
{
	slider->setMinimumHeight(slider->sizeHint().height());
	slider->setMinimumWidth(slider->sizeHint().width());

	slider->setMinimum(0);
	slider->setMaximum(int(ceil((_max - _min) / _step)));
	slider->setValue(int(round((_default - _min) / _step)));

	connect(slider, SIGNAL(valueChanged(int)), 
			this, SLOT(stepControlChanged(int)));
}

void ControlSenderWidget::createSpinBox()
{
	_spinBox = new QDoubleSpinBox();
	layout()->addWidget(_spinBox);

	_spinBox->setMinimum(_min);
	_spinBox->setMaximum(_max);
	_spinBox->setSingleStep(_step);
	_spinBox->setValue(_default);

	connect(_spinBox, SIGNAL(valueChanged(double)), 
		this, SLOT(continuousControlChanged(double)));
}

void ControlSenderWidget::stepControlChanged(int value)
{
	if (_updating) return;
	_updating = true;

	double dvalue = _min + _step * value;
	if (_spinBox) _spinBox->setValue(dvalue);
	_sender->SendControl(dvalue);

	_updating = false;
}

void ControlSenderWidget::continuousControlChanged(double value)
{
	if (_updating) return;
	_updating = true;

	int svalue = int(round((value - _min) / _step));
	if (_slider) _slider->setValue(svalue);
	if (_dial) _dial->setValue(svalue);
	_sender->SendControl(value);

	_updating = false;
}


