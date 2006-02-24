
#ifndef COMBO_TABLE_ITEM_H
#define COMBO_TABLE_ITEM_H

#include <q3combobox.h>
#include <qstringlist.h>

#include "TableItem.hxx"

#include <vector>

class ComboTableItem : public TableItem
{
public:
    ComboTableItem( Q3Table *table, std::vector<QStringList> &list, bool editable = FALSE );
    ~ComboTableItem();
    virtual QWidget *createEditor() const;
    virtual void setContentFromEditor( QWidget *w );
    virtual void paint( QPainter *p, const QColorGroup &cg,
                        const QRect &cr, bool selected );
    virtual void setCurrentItem( int i );
    virtual void setCurrentItem( const QString &i );
    int currentItem() const;
    QString currentText() const;
    int count() const;
#if !defined(Q_NO_USING_KEYWORD)
    using Q3TableItem::text;
#endif
    QString text( int i ) const;
    virtual void setEditable( bool b );
    bool isEditable() const;
    virtual void setStringList( const QStringList &l );

    int rtti() const;
    static int RTTI;

    QSize sizeHint() const;

private:
    QComboBox *cb;
    std::vector<QStringList> entries;
    int current;
    bool edit;
    static QComboBox *fakeCombo;
    static QWidget *fakeComboWidget;
    static int fakeRef;

};
#endif
