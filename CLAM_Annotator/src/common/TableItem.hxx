#ifndef TABLE_ITEM_H
#define TABLE_ITEM_H

#include "qtable.h"
#include "qpainter.h"
#include "qcolor.h"
#include "qrect.h"

class TableItem : public QTableItem
{
public:
    TableItem (QTable * table, EditType et ):QTableItem(table,et){}
    TableItem (QTable * table, EditType et, const QString & text ):QTableItem(table,et,text){}
    TableItem (QTable * table, EditType et, const QString & text, const QPixmap & p ):QTableItem(table,et,text,p){}
    virtual void paint( QPainter * p, const QColorGroup & cg, const QRect & cr, bool selected );
};


#endif
