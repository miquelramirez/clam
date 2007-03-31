#include "ControlSenderWidget.hxx"

class ControlSenderWidgetParams {
public:
	ControlSenderWidgetParams() : label(false), dial(false),
		slider(false), spinBox(false), align(Qt::Horizontal) { }

	bool label, dial, slider, spinBox;
	Qt::Orientation align;
};

ControlSenderWidget::ControlSenderWidget(CLAM::Processing * processing) :
	_dial(NULL), _slider(NULL), _spinBox(NULL), _updating(false)
{
	_sender = dynamic_cast<CLAM::OutControlSender* >(processing);
	CLAM_ASSERT(_sender, "ControlSenderWidget only works "
				"with OutControlSender processings.");

	ControlSenderWidgetParams params;
	init(params);

	createLabel(params);
	createDial(params);
	createSlider(params);
	createSpinBox(params);
}

ControlSenderWidget::~ControlSenderWidget()
{
}

void ControlSenderWidget::init(ControlSenderWidgetParams &params)
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
		params.label = true;
		break;
	case CLAM::OutControlSenderConfig::EControlRepresentation::eKnot:
		params.spinBox = params.dial = true;
		params.align = Qt::Vertical;
		break;
	case CLAM::OutControlSenderConfig::EControlRepresentation::eHorizontalSlider:
		params.spinBox = params.slider = true;
		break;
	case CLAM::OutControlSenderConfig::EControlRepresentation::eVerticalSlider:
		params.spinBox = params.slider = true;
		params.align = Qt::Vertical;
		break;
	case CLAM::OutControlSenderConfig::EControlRepresentation::eSpinBox:
		params.spinBox = true;
		break;
	}
	if (params.align == Qt::Vertical) 
		setLayout(new QVBoxLayout);
	else
		setLayout(new QHBoxLayout);
	layout()->setMargin(1);
}

void ControlSenderWidget::createLabel(const ControlSenderWidgetParams &params)
{
	if (!params.label)
		return;

	QLabel *label = new QLabel();
	label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	label->setNum(_default);
	label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
	layout()->addWidget(label);
}

void ControlSenderWidget::createDial(const ControlSenderWidgetParams &params)
{
	if (!params.dial)
		return;

	_dial = new QDial();
	setupSlider(_dial);
	_dial->setNotchesVisible(true);
	layout()->addWidget(_dial);
}

void ControlSenderWidget::createSlider(const ControlSenderWidgetParams &params)
{
	if (!params.slider)
		return;

	_slider = new QSlider(params.align);
	setupSlider(_slider);
	if (params.align == Qt::Vertical) {
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

void ControlSenderWidget::createSpinBox(const ControlSenderWidgetParams &params)
{
	if (!params.spinBox)
		return;

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


