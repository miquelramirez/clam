
#include "ProcessingItem.hxx"

namespace NetworkGUI
{

ProcessingItem::ProcessingItem( QListView * parent , const char* name )
	: QListViewItem( parent , 0 )
{
	setText( 0, name );
	setDropEnabled(false);
	setDragEnabled(true);
}

ProcessingItem::ProcessingItem( QListViewItem * parent , const char* name )
	: QListViewItem( parent , 0 )
{
	setText( 0, name );
	setDropEnabled(false);
	setDragEnabled(true);
}

ProcessingItem::~ProcessingItem()
{
}

} // namespace NetworkGUI
