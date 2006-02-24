#ifndef RANGE_SELECTION_TABLE_ITEM_HXX
#define RANGE_SELECTION_TABLE_ITEM_HXX

#include "TableItem.hxx"
#include <qpointer.h>
#include "Range.hxx"
#include <CLAM/DataTypes.hxx>

using CLAM_Annotator::Range;

class Q3ProgressBar;
class SliderWithValue;

class RangeSelectionTableItem : public TableItem
{
public:
    RangeSelectionTableItem( Q3Table * table, EditType et,const QString & value, Range<CLAM::TData> fRange );
    RangeSelectionTableItem( Q3Table * table, EditType et,const QString & value, Range<int> iRange );
    QWidget * createEditor() const;
    void setContentFromEditor( QWidget * );
    void setText( const QString &);
    void paint( QPainter * p, const QColorGroup & cg , const QRect & cr, bool selected );

private:
    QPointer<SliderWithValue> mSlider;
    QPointer<Q3ProgressBar> mProgressBar;
    Range<CLAM::TData> mFloatRange;
    Range<int> mIntegerRange;
    bool mHasFloatRange, mHasIntegerRange;

};

#endif
