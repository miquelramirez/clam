#include "ConfigurationDialog.hxx"

#include <qsplitter.h>
#include <qlistview.h>
#include "Table.hxx"
#include <qwidgetstack.h>

ConfigurationDialog::ConfigurationDialog( QWidget * parent, const char * name, WFlags f) : ConfigurationDialogBase( parent, name, f)
{
	mListView->reparent(mSplitter,QPoint(0,0));
  mWidgetStack->reparent(mSplitter,QPoint(1,0));
  new ListViewItem(0,mListView,"Colors");
  new ListViewItem(1,mListView,"Analyze");
  new ListViewItem(2,mListView,"Data Base");
  new ListViewItem(3,mListView,"Descriptors");
  new ListViewItem(4,mListView,"ShortCuts");
  
  Table7->setLeftMargin(0);
  Table7->setNumRows(0);
  Table7->setNumCols(2);
  Table7->setRowMovingEnabled(false);
  Table7->setColumnMovingEnabled(false);
  Table7->setReadOnly(false);
  Table7->setSelectionMode(QTable::NoSelection);
  Table7->horizontalHeader()->setLabel( 0, tr( "Property" ) );
  Table7->horizontalHeader()->setLabel( 1, tr( "Value" ) );
  Table7->setNumRows(Table7->numRows()+1);
  Table7->setItem(0,0,new TableItem(Table7,TableItem::Never,"asdf"));    

}

