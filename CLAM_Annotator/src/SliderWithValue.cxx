#include "SliderWithValue.hxx"
#include <qslider.h>
#include <qlabel.h>
#include <qlayout.h>

SliderWithValue::SliderWithValue( QWidget * parent, const char * name, WFlags f) : QWidget( parent, name, f)
{
	QHBoxLayout * layout = new QHBoxLayout(this);
	mSlider = new QSlider(this, "slider");
	mSlider->setMaxValue(10);
	mSlider->setMinValue(0);
	mSlider->setPageStep(1);
	mSlider->setOrientation( Qt::Horizontal );
	layout->addWidget( mSlider );
	mValue = new QLabel(this, "value");
	layout->addWidget( mValue );
	connect( mSlider, SIGNAL( valueChanged(int) ), mValue, SLOT( setNum(int) ) );
}

int SliderWithValue::value()
{
	return mSlider->value();
}

void SliderWithValue::setValue(int value)
{
	mSlider->setValue( value );
	mValue->setText(QString().setNum( value ) );
}

