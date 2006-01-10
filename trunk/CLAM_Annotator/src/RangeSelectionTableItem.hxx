#ifndef RANGE_SELECTION_TABLE_ITEM_HXX
#define RANGE_SELECTION_TABLE_ITEM_HXX

#include "TableItem.hxx"
#include <qguardedptr.h>
#include "Range.hxx"
#include <CLAM/DataTypes.hxx>

using CLAM_Annotator::Range;

class QProgressBar;
class SliderWithValue;

class RangeSelectionTableItem : public TableItem
{
public:
    RangeSelectionTableItem( QTable * table, EditType et,const QString & value, Range<CLAM::TData> fRange );
    RangeSelectionTableItem( QTable * table, EditType et,const QString & value, Range<int> iRange );
    QWidget * createEditor() const;
    void setContentFromEditor( QWidget * );
    void setText( const QString &);
    void paint( QPainter * p, const QColorGroup & cg , const QRect & cr, bool selected );

private:
    QGuardedPtr<SliderWithValue> mSlider;
    QGuardedPtr<QProgressBar> mProgressBar;
    Range<CLAM::TData> mFloatRange;
    Range<int> mIntegerRange;
    bool mHasFloatRange, mHasIntegerRange;

};

#endif
