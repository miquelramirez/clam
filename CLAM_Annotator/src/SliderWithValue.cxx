#include "SliderWithValue.hxx"
#include <qslider.h>
#include <qlabel.h>
#include <qlayout.h>

SliderWithValue::SliderWithValue( QWidget * parent, const char * name, WFlags f) : QWidget( parent, name, f)
{
	QHBoxLayout * layout = new QHBoxLayout(this);
	mSlider = new QSlider(this, "slider");
	mSlider->setPageStep(1);
	mSlider->setOrientation( Qt::Horizontal );
	layout->addWidget( mSlider );
	mValue = new QLabel(this, "value");
	layout->addWidget( mValue );
}

SliderWithValue::~SliderWithValue()
{
}


int SliderWithValue::value()
{
    return mSlider->value();
}

