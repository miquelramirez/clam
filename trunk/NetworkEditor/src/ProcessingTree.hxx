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

#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
//#include <QtGui/QLabel>
//#include <iostream>

namespace CLAM
{
	class Processing;
}

namespace NetworkGUI
{

class ProcessingTree : public QWidget
{
Q_OBJECT
public:
	/** TODO: this tree should be derived from the CLAM::Factory instance */
	ProcessingTree( QWidget * parent = 0);
	virtual ~ProcessingTree();
private:
	QLineEdit * _searchEdit;
	QPushButton * _clearButton;
	QTreeWidget * _treeWidget;
public slots:
	void PressProcessing(QTreeWidgetItem *, int column);
private slots:
	void filterProcessings ( const QString & text )
	{
		QTreeWidgetItem * parentItem=NULL;
		QTreeWidgetItemIterator itemIt (_treeWidget, QTreeWidgetItemIterator::NoChildren);
		while (*itemIt)
		{
			// if it is on a new category
			if (parentItem != (*itemIt)->parent())
			{
				parentItem=(*itemIt)->parent();
				parentItem->setHidden(true);
			}
			if (itemContains((*itemIt),text))
			{
				(*itemIt)->setHidden(false);
				parentItem->setHidden(false);
			}
			else
				(*itemIt)->setHidden(true);

			++itemIt;
		}
		if (text=="")
			_treeWidget->collapseAll();
		else
			_treeWidget->expandAll();
	}
private:
	bool itemContains (QTreeWidgetItem * item, const QString & text)
	{
		return (item->text(1).contains(text,Qt::CaseInsensitive));
	}
};

} // namespace NetworkGUI

#endif //  __PROCESSINGTREE_HXX__

