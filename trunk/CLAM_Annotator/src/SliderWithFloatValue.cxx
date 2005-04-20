#include <qslider.h>
#include <qlabel.h>
#include "SliderWithFloatValue.hxx"

SliderWithFloatValue::SliderWithFloatValue(QWidget * parent, const char * name, WFlags f)
    : SliderWithValue(parent, name, f)
{
    connect( mSlider, SIGNAL( valueChanged(int) ), this, SLOT( setLabelValue(int) ) );
}

int SliderWithFloatValue::intValue()
{
    // empty body for convenience
    return 0;
}

float SliderWithFloatValue::floatValue()
{
    return float(mSlider->value())*0.1f;
}

void SliderWithFloatValue::setValue(int value)
{
    mSlider->setValue( value );
    mValue->setText( QString::number(double(value)*0.1,'f',1) );
}

void SliderWithFloatValue::setMaxValue(int value)
{
    mSlider->setMaxValue(value*10);
}

void SliderWithFloatValue::setMinValue(int value)
{
    mSlider->setMinValue(value*10);
}

void SliderWithFloatValue::setLabelValue(int value)
{
    mValue->setText( QString::number(double(value)*0.1,'f',1) );
}

//END

