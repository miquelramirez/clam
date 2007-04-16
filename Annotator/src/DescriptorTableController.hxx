/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _DescriptorTableController_hxx_
#define _DescriptorTableController_hxx_

#include <string>
#include <vector>
#include <QtGui/QItemDelegate>

class QTableWidget;

namespace CLAM
{
	class DescriptionDataPool;
	class Text;
}

namespace CLAM_Annotator
{
	class Project;
	class Enumerated;
	class DescriptorTablePlugin;
}

namespace CLAM_Annotator
{

class DescriptorTableController : public QItemDelegate
{
	Q_OBJECT
private:
	QTableWidget * mTable;
	const CLAM_Annotator::Project & mProject;
	std::string mScope;
	int mElement;
	std::vector<DescriptorTablePlugin*> mPlugins;
	CLAM::DescriptionDataPool * mData;
public:
	DescriptorTableController(QTableWidget * table, const CLAM_Annotator::Project & project);
	~DescriptorTableController();
	void refreshSchema(const std::string & scope);
	void refreshData(int element, CLAM::DescriptionDataPool * dataPool);
	void updateData(int row, CLAM::DescriptionDataPool * dataPool);

	// Delegate interface
	virtual QWidget * createEditor (QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
	virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
	virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
signals:
	void contentEdited(int row);
private:
	void emitContentEdited(int row);
	int descriptorIndexInTable(const std::string& name);
};
} // namespace CLAM_Annotator


#endif//_DescriptorTableController_hxx_

