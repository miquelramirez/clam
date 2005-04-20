#ifndef SLIDER_WITH_VALUE_HXX
#define SLIDER_WITH_VALUE_HXX

#include <qwidget.h>
#include <qguardedptr.h>

class QSlider;
class QLabel;

class SliderWithValue : public QWidget
{
    Q_OBJECT

public:
    SliderWithValue( QWidget * parent = 0, const char * = 0, WFlags f = 0);
    virtual ~SliderWithValue();

    int value();

    virtual void setValue(int)=0;
    virtual void setMaxValue(int)=0;
    virtual void setMinValue(int)=0;

protected:
    QGuardedPtr<QSlider> mSlider;
    QGuardedPtr<QLabel> mValue;
		
};

#endif


