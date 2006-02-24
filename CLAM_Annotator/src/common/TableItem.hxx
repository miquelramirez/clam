#ifndef TABLE_ITEM_H
#define TABLE_ITEM_H

#include <q3table.h>
#include <qpainter.h>
#include <qcolor.h>
#include <qrect.h>
//Added by qt3to4:
#include <QPixmap>

class TableItem : public Q3TableItem
{
public:
    TableItem (Q3Table * table, EditType et ):Q3TableItem(table,et){}
    TableItem (Q3Table * table, EditType et, const QString & text ):Q3TableItem(table,et,text){}
    TableItem (Q3Table * table, EditType et, const QString & text, const QPixmap & p ):Q3TableItem(table,et,text,p){}
    virtual void paint( QPainter * p, const QColorGroup & cg, const QRect & cr, bool selected );
};


#endif
