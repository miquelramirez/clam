#include "Table.hxx"

#include <iostream>

void Table::columnClicked ( int col )
{
    sortColumn(col,TRUE,TRUE);
    selectColumn(col);
}

void Table::swapColumns(int col1,int col2,bool swapHeader=false)
{
    QTable::swapColumns(col1,col2,swapHeader);
    emit SwapColumns(col1,col2,swapHeader);
}
