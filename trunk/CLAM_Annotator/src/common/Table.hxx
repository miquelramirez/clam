#ifndef TABLE_H
#define TABLE_H

#include "qtable.h"

class Table : public QTable
{
    Q_OBJECT
public:
    Table( QWidget * parent = 0, const char * name = 0 ):QTable(parent,name){}
    Table( int numRows, int numCols, QWidget * parent = 0, const char * name = 0 ):QTable(numRows,numCols,parent,name){};
protected slots:    
    virtual void columnClicked ( int col );
    virtual void swapColumns(int col1,int col2,bool swapHeader);
signals:
    void SwapColumns(int col1,int col2,bool swapHeader);
};

#endif
