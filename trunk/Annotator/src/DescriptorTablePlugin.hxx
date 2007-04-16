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

#ifndef _DescriptorTablePlugin_hxx_
#define _DescriptorTablePlugin_hxx_

#include <string>

class QTableWidget;
class QWidget;
class QStyleOptionViewItem;
namespace CLAM { class DescriptionDataPool; }
namespace CLAM_Annotator { class SchemaAttribute; }

namespace CLAM_Annotator
{
	class DescriptorTablePlugin
	{
	public:
		DescriptorTablePlugin(QTableWidget * parent, unsigned row, const SchemaAttribute & scheme);
		virtual ~DescriptorTablePlugin();
		void refreshData(int element, CLAM::DescriptionDataPool & dataPool);
		virtual void refreshData(CLAM::DescriptionDataPool & dataPool) = 0;
		virtual void updateData(CLAM::DescriptionDataPool & dataPool) = 0;
		virtual QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option);
		virtual void fillEditor(QWidget * editor, CLAM::DescriptionDataPool & dataPool);
		virtual void takeEditorContent(QWidget * editor, CLAM::DescriptionDataPool & dataPool);
		void clearData();
	protected:
		QTableWidget * mTable;
		const unsigned mRow;
		const std::string mScope;
		const std::string mName;
		int mElement;
	};

	DescriptorTablePlugin * createItemController(
			QTableWidget * parent, 
			unsigned row, 
			const SchemaAttribute & scheme);

}

#endif//_DescriptorTablePlugin_hxx_

