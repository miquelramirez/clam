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
