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

#ifndef SourceEditor_hxx
#define SourceEditor_hxx

#include "ui_SourceEditor.hxx"
#include "AggregationEditor.hxx"  //may be iterating

/*
struct configurationParser
{
  std::string * source, *extractor, *suffix, *schemaFile, *configFile, *path;
  std::string *targetScope, *targetAttribute, *sourceId, *sourceScope, *sourceAttribute;
  int sourceCnt, attributeCnt;
};
*/

class SourceEditor : public QDialog
{
		Q_OBJECT
	public:
		SourceEditor(AggregationEditor * parent =0, AggregationEditor::configurationParser* parser=0)
			: QDialog(parent)
		{
			_ui.setupUi(this);
			loadParameter(parser);
			mAggregationEditor = parent;
			connect(_ui.parameterTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(setItemChangedFlag())); 
			_itemChangedFlag=0;
		}
		virtual ~SourceEditor();
		void applyUpdates();

	private:

		void loadParameter(AggregationEditor::configurationParser* parser);
		
	private slots:
		void setItemChangedFlag();
		void on_addButton_clicked();
		void on_deleteButton_clicked();
		
	private:
		Ui::SourceEditor _ui;
		int _itemChangedFlag;
		AggregationEditor * mAggregationEditor;
};

#endif

