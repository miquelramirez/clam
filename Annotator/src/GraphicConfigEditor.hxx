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

#ifndef GraphicConfigEditor_hxx
#define GraphicConfigEditor_hxx

#include "ui_GraphicConfigEditor.hxx"
#include "AggregationEditor.hxx"

class GraphicConfigEditor : public QDialog
{
		Q_OBJECT
	public:
		GraphicConfigEditor(QWidget * parent =0)
			: QDialog(parent)
		{
			_ui.setupUi(this);
			mAggregationEditor = new AggregationEditor(this);  // new AggregationEditor(this);
			_ui.verticalLayout_2->insertWidget(0,mAggregationEditor);
		}
		void setConfiguration(std::string config);
		std::string getConfiguration();
		virtual ~GraphicConfigEditor();


	private slots:
		void on_okButton_clicked();
		void on_cancelButton_clicked();

		
	private:
		std::string mConfig;
		Ui::GraphicConfigEditor _ui;
		AggregationEditor * mAggregationEditor;
};

#endif//GraphicConfigEditor_hxx

