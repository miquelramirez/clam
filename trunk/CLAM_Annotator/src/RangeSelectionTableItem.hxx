#ifndef RANGE_SELECTION_TABLE_ITEM_HXX
#define RANGE_SELECTION_TABLE_ITEM_HXX

#include "TableItem.hxx"
#include <qguardedptr.h>

class QProgressBar;
class SliderWithValue;

class RangeSelectionTableItem : public TableItem
{
	public:
		RangeSelectionTableItem( QTable * table, EditType et, const QString & value );
		QWidget * createEditor() const;
		void setContentFromEditor( QWidget * );
		void setText( const QString &);
		void paint( QPainter * p, const QColorGroup & cg , const QRect & cr, bool selected );
	private:
		QGuardedPtr<SliderWithValue> mSlider;
		QGuardedPtr<QProgressBar> mProgressBar;
};

#endif
