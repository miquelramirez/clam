#include "SliderWithValue.hxx"
#include <qslider.h>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>

SliderWithValue::SliderWithValue( QWidget * parent, Qt::WFlags f) : QWidget( parent, f )
{
	QHBoxLayout * layout = new QHBoxLayout(this);
	mSlider = new QSlider(this);
	mSlider->setPageStep(1);
	mSlider->setOrientation( Qt::Horizontal );
	layout->addWidget( mSlider );
	mValue = new QLabel(this);
	layout->addWidget( mValue );
}

SliderWithValue::~SliderWithValue()
{
}



