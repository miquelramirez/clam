#ifndef __SLIDERWITHFLOATVALUE__
#define __SLIDERWITHFLOATVALUE__

#include "SliderWithValue.hxx"

class SliderWithFloatValue : public SliderWithValue
{
    Q_OBJECT
public:
    SliderWithFloatValue(QWidget * parent = 0, const char * = 0, Qt::WFlags f = 0);

    int intValue();

    float floatValue();
    void setValue(int);

    void setMaxValue(int);
    void setMinValue(int);

private slots:
    void setLabelValue(int);

};

#endif

