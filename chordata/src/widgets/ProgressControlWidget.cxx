#include "ProgressControlWidget.hxx"
#include <cmath>

ProgressControlWidget::ProgressControlWidget(QWidget *parent, CLAM::Processing *processing)
	: QSlider( parent ),
	  _processing( processing ),
	  _updating( false ),
	  _jumping( false )
{
	setOrientation(Qt::Horizontal);

	connect(this, SIGNAL(valueChanged(int)), 
		this, SLOT(sliderValueChanged(int)));
	connect(this, SIGNAL(sliderPressed()), 
		this, SLOT(sliderPressed()));
	connect(this, SIGNAL(sliderReleased()), 
		this, SLOT(sliderReleased()));
	
	startTimer(50);
}

ProgressControlWidget::~ProgressControlWidget()
{
}

CLAM::Processing* ProgressControlWidget::GetProcessing()
{
	return _processing;
}
	
void ProgressControlWidget::SetProcessing(CLAM::Processing *processing)
{
	_processing = processing;
}

void ProgressControlWidget::sliderValueChanged(int value)
{
	if (_updating) return;
	
	if (!_processing)
		return;
	
	double dvalue = float(value) / 100.0;
	SendFloatToOutControl(*_processing,0,dvalue);
}

void ProgressControlWidget::sliderPressed()
{
	_jumping = true;
}

void ProgressControlWidget::sliderReleased()
{
	_jumping = false;
}

void ProgressControlWidget::timerEvent(QTimerEvent *event)
{
	if (_jumping) return;
	
	if (!_processing)
		return;
	
	_updating = true;
	int value = int(std::floor(GetFloatFromInControl(*_processing,0) * 100.0));
	setValue(value);
	_updating = false;
}
