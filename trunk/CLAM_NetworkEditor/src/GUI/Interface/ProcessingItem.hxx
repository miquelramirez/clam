
#ifndef __PROCESSINGITEM_HXX__
#define __PROCESSINGITEM_HXX__

#include <qlistview.h> 

namespace NetworkGUI
{

class ProcessingItem  : public QListViewItem
{
public:
	ProcessingItem( QListView * parent = 0, const char* name = 0 );
	ProcessingItem( QListViewItem * parent = 0, const char* name = 0 );
	virtual ~ProcessingItem();
};

} // namespace NetworkGUI

#endif // __PROCESSINGITEM_HXX__
