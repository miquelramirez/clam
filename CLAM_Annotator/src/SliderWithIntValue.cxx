#include <qslider.h>
#include <qlabel.h>
#include "SliderWithIntValue.hxx"

SliderWithIntValue::SliderWithIntValue(QWidget * parent, const char * name , Qt::WFlags f)
    : SliderWithValue( parent, name, f)
{
    connect( mSlider, SIGNAL( valueChanged(int) ), mValue, SLOT( setNum(int) ) );
}

float SliderWithIntValue::floatValue()
{
    // empty body for convenience
    return 0.0f;
}

int SliderWithIntValue::intValue()
{
    return mSlider->value();
}

void SliderWithIntValue::setValue(int value)
{
    mSlider->setValue( value );
    mValue->setText(QString::number( value ) );
}

void SliderWithIntValue::setMaxValue(int value)
{
    mSlider->setMaximum(value);
}

void SliderWithIntValue::setMinValue(int value)
{
    mSlider->setMinimum(value);
}

// END
