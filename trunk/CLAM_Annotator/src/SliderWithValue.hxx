#ifndef SLIDER_WITH_VALUE_HXX
#define SLIDER_WITH_VALUE_HXX

#include <qwidget.h>
#include <qpointer.h>
//Added by qt3to4:
#include <QLabel>

class QSlider;
class QLabel;

class SliderWithValue : public QWidget
{
	Q_OBJECT

public:
	SliderWithValue( QWidget * parent = 0, Qt::WFlags f = 0);
	virtual ~SliderWithValue();

	virtual int intValue()=0;
	virtual float floatValue()=0;

	virtual void setValue(int)=0;
	virtual void setMaximum(int)=0;
	virtual void setMinimum(int)=0;

protected:
	QPointer<QSlider> mSlider;
	QPointer<QLabel> mValue;
		
};

#endif


