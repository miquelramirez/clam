#include <qslider.h>
#include <qlabel.h>
#include "SliderWithIntValue.hxx"

SliderWithIntValue::SliderWithIntValue(QWidget * parent, const char * name , WFlags f)
    : SliderWithValue( parent, name, f)
{
    connect( mSlider, SIGNAL( valueChanged(int) ), mValue, SLOT( setNum(int) ) );
}

void SliderWithIntValue::setValue(int value)
{
    mSlider->setValue( value );
    mValue->setText(QString::number( value ) );
}

void SliderWithIntValue::setMaxValue(int value)
{
    mSlider->setMaxValue(value);
}

void SliderWithIntValue::setMinValue(int value)
{
    mSlider->setMinValue(value);
}

// END
