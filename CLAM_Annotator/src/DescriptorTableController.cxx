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

#include "DescriptorTableController.hxx"
#include "Project.hxx"
#include "DescriptorTablePlugin.hxx"
#include <QtGui/QTableWidget>
#include <QtGui/QHeaderView>

namespace CLAM_Annotator
{

	DescriptorTableController::DescriptorTableController(QTableWidget * table, const CLAM_Annotator::Project & project)
		: mTable(table)
		, mProject(project)
		, mScope("")
		, mElement(-1)
		, mData(0)
	{
		mTable->setColumnCount( 2 );
		mTable->setHorizontalHeaderLabels(QStringList() 
				<< tr("Descriptor" )
				<< tr("Value")
			);
		mTable->setRowCount( 0 );
		mTable->setSelectionMode( QAbstractItemView::NoSelection );
		mTable->setAlternatingRowColors(true);
		mTable->horizontalHeader()->setStretchLastSection(true);
		mTable->setTabKeyNavigation(false);
		mTable->setEditTriggers(QAbstractItemView::AllEditTriggers );
		mTable->setItemDelegate(this);
	}
	DescriptorTableController::~DescriptorTableController()
	{
		for (unsigned i = 0; i < mPlugins.size(); i++)
			delete mPlugins[i];
	}
	void DescriptorTableController::refreshSchema(const std::string & scope)
	{
		mTable->hide();
		mTable->setRowCount(0);

		for (unsigned i = 0; i < mPlugins.size(); i++)
			delete mPlugins[i];
		mPlugins.clear();

		mScope = scope;
		mElement = -1;

		CLAM_Annotator::Project::ScopeSchema attributes = mProject.GetScopeSchema(mScope);
		mTable->setRowCount(attributes.size());
		std::list<SchemaAttribute>::iterator attribute = attributes.begin();
		for(unsigned row = 0 ; attribute != attributes.end(); attribute++)
		{
			DescriptorTablePlugin * itemController 
				= createItemController(mTable, row, *attribute);
			mTable->setVerticalHeaderItem (row,  new QTableWidgetItem("") );
			if (!itemController) continue; // No plugin available for that type
			mPlugins.push_back(itemController);
			row++;
		}
		mTable->setRowCount(mPlugins.size()); // Some attributes were filtered
		mTable->resizeColumnToContents(0);
		mTable->resizeRowsToContents();
		mTable->show();
	}
	void DescriptorTableController::refreshData(int element, CLAM::DescriptionDataPool * dataPool)
	{
		if (mScope=="") return;
		mData = dataPool;
		mElement=element;
//		mTable->setColumnReadOnly(1,mElement==-1); // QTPORT
		for (unsigned i = 0; i<mPlugins.size(); i++)
			mPlugins[i]->refreshData(mElement, *dataPool);
	}
	void DescriptorTableController::updateData(int row, CLAM::DescriptionDataPool * dataPool)
	{
		if (mElement==-1) return;

		DescriptorTablePlugin * itemController = mPlugins[row];
		itemController->updateData(*dataPool);
	}

	int DescriptorTableController::descriptorIndexInTable(const std::string& name)
	{
		//TODO: should find a more efficient search algorithm

		CLAM_Annotator::Project::ScopeSchema attributes = mProject.GetScopeSchema(mScope);
		std::list<SchemaAttribute>::iterator attribute = attributes.begin();
		for(int i = 0 ; attribute != attributes.end(); attribute++, i++)
		{
			if (attribute->GetName() == name) return i;
		}
		return -1;
	}

	QWidget * DescriptorTableController::createEditor(
			QWidget * parent, const QStyleOptionViewItem & option,
			const QModelIndex & index) const
	{
		if (index.column()==0) return 0; // Attribute names are read only
		if (mElement==-1) return 0; // When no element, just avoid edition
		QWidget * editor = mPlugins[index.row()]->createEditor(parent, option);
		if (editor)
		{
			editor->installEventFilter(const_cast<DescriptorTableController*>(this));
			return editor;
		}
		return QItemDelegate::createEditor(parent, option, index);
	}
	void DescriptorTableController::setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		if (index.column()==0) return; // Attribute names are read only
		mPlugins[index.row()]->fillEditor(editor, *mData);
	}
	void DescriptorTableController::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
	{
		if (index.column()==0) return; // Attribute names are read only
		mPlugins[index.row()]->takeEditorContent(editor, *mData);
		// KLUDGE!! Why in hell this function is const!!
		const_cast<DescriptorTableController*>(this)->emitContentEdited(index.row());
	}
	void DescriptorTableController::emitContentEdited(int row)
	{
		emit contentEdited(row);
	}
}

