/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

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
