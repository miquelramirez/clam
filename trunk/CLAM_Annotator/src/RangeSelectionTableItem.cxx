#include "RangeSelectionTableItem.hxx"
#include "SliderWithFloatValue.hxx"
#include "SliderWithIntValue.hxx"
#include <qprogressbar.h>
#include <qstyle.h>

RangeSelectionTableItem::RangeSelectionTableItem( QTable * table, 
						  EditType et, 
						  const QString & value, 
						  Range<CLAM::TData> fRange) 
    : TableItem( table, et, value),
      mHasFloatRange(true),
      mHasIntegerRange(false)
{
	( (RangeSelectionTableItem*) this)->mProgressBar = new QProgressBar( table->viewport() );
	mFloatRange.SetMin(fRange.GetMin());
	mFloatRange.SetMax(fRange.GetMax());
	mProgressBar->setTotalSteps( int(mFloatRange.GetMax()*10.0f)-int(mFloatRange.GetMin()*10.0f));
	mProgressBar->setProgress( int(value.toFloat()*10.0f)-int(mFloatRange.GetMin()*10.0f));
}

RangeSelectionTableItem::RangeSelectionTableItem( QTable * table, 
						  EditType et, 
						  const QString & value, 
						  Range<int> iRange) 
    : TableItem( table, et, value),
      mHasFloatRange(false),
      mHasIntegerRange(true)
{
	( (RangeSelectionTableItem*) this)->mProgressBar = new QProgressBar( table->viewport() );
	mIntegerRange.SetMin(iRange.GetMin());
	mIntegerRange.SetMax(iRange.GetMax());
	mProgressBar->setTotalSteps(mIntegerRange.GetMax()-mIntegerRange.GetMin());
	mProgressBar->setProgress(value.toInt()-mIntegerRange.GetMin());
}

QWidget * RangeSelectionTableItem::createEditor() const
{
    if(mHasIntegerRange)
    {
	((RangeSelectionTableItem*) this)->mSlider = new SliderWithIntValue( table()->viewport(), "slider");
	mSlider->setMinValue(mIntegerRange.GetMin());
	mSlider->setMaxValue(mIntegerRange.GetMax());
	mSlider->setValue( text().toInt() );
    }
    else if(mHasFloatRange)
    {
	((RangeSelectionTableItem*) this)->mSlider = new SliderWithFloatValue( table()->viewport(), "slider");
	mSlider->setMinValue(int(mFloatRange.GetMin())*10);
	mSlider->setMaxValue(int(mFloatRange.GetMax())*10);
	mSlider->setValue( int(text().toFloat()*10.0f) );
    }
    return mSlider; 
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
	    float value = s.toFloat();
	    mSlider->setValue( int(value*10.0f) );
	}
    }
    QTableItem::setText( s );
}

void RangeSelectionTableItem::paint( QPainter * p, const QColorGroup & cg, const QRect & cr, bool selected)
{
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
	QStyle::SFlags flags = QStyle::Style_Default;
	if(isEnabled() && table()->isEnabled())
		flags |= QStyle::Style_Enabled;
	table()->style().drawPrimitive( QStyle::PE_ProgressBarChunk, p, mProgressBar->rect(), *newCG, flags );
	table()->style().drawControl ( QStyle::CE_ProgressBarGroove, p, mProgressBar, table()->style().subRect (QStyle::SR_ProgressBarGroove, mProgressBar) , *newCG, flags );
	newCG->setBrush(QColorGroup::Highlight,QColor( 0, 0, static_cast<int>( static_cast<float>( mProgressBar->progress() )/float(mProgressBar->totalSteps())*255.0 ) ) );
	table()->style().drawControl ( QStyle::CE_ProgressBarContents, p, mProgressBar, table()->style().subRect (QStyle::SR_ProgressBarContents, mProgressBar ), *newCG, flags );  

}

void RangeSelectionTableItem::setContentFromEditor( QWidget * w)
{
	if (w->inherits("SliderWithValue"))
	{
	    if(mHasIntegerRange)
	    {
		mProgressBar->setProgress( mSlider->intValue()-mIntegerRange.GetMin() );
		setText( QString::number( mSlider->intValue()) );
	    }
	    else if(mHasFloatRange)
	    {
		mProgressBar->setProgress( int(mSlider->floatValue()*10.0f)-int(mFloatRange.GetMin()*10.0f) );
		setText( QString::number(mSlider->floatValue(),'f',1) );
	    }
	}
	else
	{
		TableItem::setContentFromEditor( w );
	}

			
}
