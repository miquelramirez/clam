#ifndef LIST_VIEW_ITEM
#define LIST_VIEW_ITEM

#include <qlistview.h>

class ListViewItem : public QListViewItem
{
private:
    int row_;
    void init();
public:    
    ListViewItem ( int row, QListView * parent ):QListViewItem(parent),row_(row) {init();}
    ListViewItem ( int row, QListViewItem * parent ):QListViewItem(parent),row_(row) {init();}
    ListViewItem ( int row, QListView * parent, ListViewItem * after ):QListViewItem(parent,after),row_(row) {init();}
    ListViewItem ( int row, QListViewItem * parent, ListViewItem * after ):QListViewItem(parent,after),row_(row) {init();}
    ListViewItem ( int row, QListView * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null ):QListViewItem(parent,label1,label2,label3,label4,label5,label6,label7,label8),row_(row) {init();}
    ListViewItem ( int row, QListViewItem * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null ):QListViewItem(parent,label1,label2,label3,label4,label5,label6,label7,label8),row_(row) {init();}
    ListViewItem ( int row, QListView * parent, ListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null ):QListViewItem(parent,after,label1,label2,label3,label4,label5,label6,label7,label8),row_(row) {init();}
    ListViewItem ( int row, QListViewItem * parent, ListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null ):QListViewItem(parent,after,label1,label2,label3,label4,label5,label6,label7,label8),row_(row){init();}
    virtual void paintCell ( QPainter * p, const QColorGroup & cg, int column, int width, int align );
};

#endif
