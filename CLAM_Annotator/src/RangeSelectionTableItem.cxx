#include "RangeSelectionTableItem.hxx"
#include "SliderWithFloatValue.hxx"
#include "SliderWithIntValue.hxx"
#include <q3progressbar.h>
#include <QStyle>
#include <QStyleOptionProgressBar>

RangeSelectionTableItem::RangeSelectionTableItem( Q3Table * table, 
						  EditType et, 
						  const QString & value, 
						  Range<CLAM::TData> fRange) 
	: TableItem( table, et, value),
	  mHasFloatRange(true),
	  mHasIntegerRange(false)
{
	mProgressBar = new Q3ProgressBar( table->viewport() );
	mFloatRange.SetMin(fRange.GetMin());
	mFloatRange.SetMax(fRange.GetMax());
	mProgressBar->setTotalSteps( int(mFloatRange.GetMax()*10.0f)-int(mFloatRange.GetMin()*10.0f));
	mProgressBar->setProgress( int(value.toFloat()*10.0f)-int(mFloatRange.GetMin()*10.0f));
}

RangeSelectionTableItem::RangeSelectionTableItem( Q3Table * table, 
						  EditType et, 
						  const QString & value, 
						  Range<int> iRange) 
	: TableItem( table, et, value),
	  mHasFloatRange(false),
	  mHasIntegerRange(true)
{
	mProgressBar = new Q3ProgressBar( table->viewport() );
	mIntegerRange.SetMin(iRange.GetMin());
	mIntegerRange.SetMax(iRange.GetMax());
	mProgressBar->setTotalSteps(mIntegerRange.GetMax()-mIntegerRange.GetMin());
	mProgressBar->setProgress(value.toInt()-mIntegerRange.GetMin());
}

QWidget * RangeSelectionTableItem::createEditor() const
{
	if(mHasIntegerRange)
	{
		SliderWithIntValue * slider = new SliderWithIntValue( table()->viewport());
		slider->setMinimum(mIntegerRange.GetMin());
		slider->setMaximum(mIntegerRange.GetMax());
		slider->setValue( text().toInt() );
		return slider;
	}
	else if(mHasFloatRange)
	{
		SliderWithFloatValue * slider = new SliderWithFloatValue( table()->viewport());
		slider->setMinimum(int(mFloatRange.GetMin())*10);
		slider->setMaximum(int(mFloatRange.GetMax())*10);
		slider->setValue( int(text().toFloat()*10.0f) );
		return slider; 
	}
	return 0; // ERROR?
}

void RangeSelectionTableItem::setText( const QString &s )
{
	if ( mSlider )
	{
		if(mHasIntegerRange)
		{
			mSlider->setValue( s.toInt() );
		}
		else if(mHasFloatRange)
		{
			mSlider->setValue( int(s.toFloat()*10.0f) );
		}
	}
	Q3TableItem::setText( s );
}

void RangeSelectionTableItem::paint( QPainter * p, const QColorGroup & cg, const QRect & cr, bool selected)
{
	Q3TableItem::paint(p, cg, cr, selected);
	return;
	QColorGroup *newCG=new QColorGroup(cg);
	mProgressBar->resize(cr.width(),cr.height());
	if (row()%2==0)
	{
		newCG->setBrush(QColorGroup::Highlight,QColor(245,245,245));
	}
	else
	{
		newCG->setBrush(QColorGroup::Highlight,cg.base());
	}
	QStyleOptionProgressBar option;
	option.state = QStyle::State_None;
	if(isEnabled() && table()->isEnabled())
		option.state |= QStyle::State_Enabled;
	table()->style()->drawPrimitive( QStyle::PE_IndicatorProgressChunk, &option, p, mProgressBar);
	table()->style()->drawControl ( QStyle::CE_ProgressBarGroove, &option, p, mProgressBar );
	newCG->setBrush(QColorGroup::Highlight,QColor( 0, 0, int( float(mProgressBar->progress())/mProgressBar->totalSteps()*255.0 ) ) );
	table()->style()->drawControl ( QStyle::CE_ProgressBarContents, &option, p, mProgressBar );

}

void RangeSelectionTableItem::setContentFromEditor( QWidget * w)
{
	if (w->inherits("SliderWithValue"))
	{
		if(mHasIntegerRange)
		{
			SliderWithIntValue * slider = (SliderWithIntValue*) w;
			mProgressBar->setProgress( slider->intValue()-mIntegerRange.GetMin() );
			setText( QString::number( slider->intValue()) );
		}
		else if(mHasFloatRange)
		{
			SliderWithFloatValue * slider = (SliderWithFloatValue*) w;
			mProgressBar->setProgress( int(slider->floatValue()*10.0f)-int(mFloatRange.GetMin()*10.0f) );
			setText( QString::number(slider->floatValue(),'f',1) );
		}
	}
	else
	{
		TableItem::setContentFromEditor( w );
	}

			
}
