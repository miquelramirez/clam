
#ifndef __PROCESSINGTREE_HXX__
#define __PROCESSINGTREE_HXX__

#include <qlistview.h>
#include <string>
#include "Signalv2.hxx"
#include "Slotv0.hxx"
#include "Qt_NetworkPresentation.hxx"

namespace CLAM
{
	class Processing;
}

namespace NetworkGUI
{

class ProcessingTree : public QListView
{
Q_OBJECT
public:
	ProcessingTree( Qt_NetworkPresentation & network, QWidget * parent = 0, const char * name = 0 );
	virtual ~ProcessingTree();

protected:
	int mNumProc;
	bool mItemPressed;
	QListViewItem * mSelectedItem;

	void contentsMouseReleaseEvent ( QMouseEvent * e );
	void startDrag();
	void IncreaseNumProc();

public slots:

void CreateProcessing(QListViewItem *);
	void PressProcessing(QListViewItem *);

public: //signals
	SigSlot::Signalv2 < const std::string &, CLAM::Processing * > AddNewProcessing;      
	SigSlot::Slotv0 ProcessingCreated;
};

} // namespace NetworkGUI

#endif //  __PROCESSINGTREE_HXX__
