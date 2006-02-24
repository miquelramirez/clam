#include "SliderWithValue.hxx"
#include <qslider.h>
#include <qlabel.h>
#include <qlayout.h>
//Added by qt3to4:
#include <Q3HBoxLayout>

SliderWithValue::SliderWithValue( QWidget * parent, const char * name, Qt::WFlags f) : QWidget( parent, name, f)
{
	Q3HBoxLayout * layout = new Q3HBoxLayout(this);
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



