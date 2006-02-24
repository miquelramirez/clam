#include "TableItem.hxx"


void TableItem::paint(QPainter * p, const QColorGroup & cg, const QRect & cr, bool selected )
{
    if (row()%2==0)
    {
	const QColorGroup *newCG=new QColorGroup(cg.foreground(),cg.button(),cg.light(),cg.dark(),cg.mid(),cg.text(),cg.brightText(),QColor(245,245,245),cg.background());
	Q3TableItem::paint(p,*newCG,cr,selected);
    }
    
    else Q3TableItem::paint(p,cg,cr,selected);
}
