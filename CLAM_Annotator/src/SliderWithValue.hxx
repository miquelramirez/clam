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
		int value();
		void setValue(int);
	private:
		QGuardedPtr<QSlider> mSlider;
		QGuardedPtr<QLabel> mValue;
		
};

#endif


