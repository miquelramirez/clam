#ifndef LIST_VIEW_ITEM
#define LIST_VIEW_ITEM

#include <q3listview.h>

class ListViewItem : public Q3ListViewItem
{
private:
    int row_;
    void init();
public:    
    ListViewItem ( int row, Q3ListView * parent ):Q3ListViewItem(parent),row_(row) {init();}
    ListViewItem ( int row, Q3ListViewItem * parent ):Q3ListViewItem(parent),row_(row) {init();}
    ListViewItem ( int row, Q3ListView * parent, ListViewItem * after ):Q3ListViewItem(parent,after),row_(row) {init();}
    ListViewItem ( int row, Q3ListViewItem * parent, ListViewItem * after ):Q3ListViewItem(parent,after),row_(row) {init();}
    ListViewItem ( int row, Q3ListView * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null ):Q3ListViewItem(parent,label1,label2,label3,label4,label5,label6,label7,label8),row_(row) {init();}
    ListViewItem ( int row, Q3ListViewItem * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null ):Q3ListViewItem(parent,label1,label2,label3,label4,label5,label6,label7,label8),row_(row) {init();}
    ListViewItem ( int row, Q3ListView * parent, ListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null ):Q3ListViewItem(parent,after,label1,label2,label3,label4,label5,label6,label7,label8),row_(row) {init();}
    ListViewItem ( int row, Q3ListViewItem * parent, ListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null ):Q3ListViewItem(parent,after,label1,label2,label3,label4,label5,label6,label7,label8),row_(row){init();}
    virtual void paintCell ( QPainter * p, const QColorGroup & cg, int column, int width, int align );
};

#endif
