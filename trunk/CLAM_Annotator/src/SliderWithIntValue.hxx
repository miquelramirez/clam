#ifndef __SLIDERWITHINTVALUE__
#define __SLIDERWITHINTVALUE__

#include "SliderWithValue.hxx"

class SliderWithIntValue : public SliderWithValue
{
    Q_OBJECT
public:
    SliderWithIntValue(QWidget * parent = 0, Qt::WFlags f = 0);

    float floatValue();
    
    int intValue();
    void setValue(int);

    void setMaximum(int);
    void setMinimum(int);

};

#endif

