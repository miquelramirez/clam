#include <qslider.h>
#include <qlabel.h>
#include "SliderWithIntValue.hxx"

SliderWithIntValue::SliderWithIntValue(QWidget * parent, Qt::WFlags f)
    : SliderWithValue( parent, f)
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

void SliderWithIntValue::setMaximum(int value)
{
    mSlider->setMaximum(value);
}

void SliderWithIntValue::setMinimum(int value)
{
    mSlider->setMinimum(value);
}

// END
