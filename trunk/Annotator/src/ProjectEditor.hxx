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

#ifndef ProjectEditor_hxx
#define ProjectEditor_hxx

#include "ui_ProjectEditor.hxx"
#include "Project.hxx"
#include "ui_SourceEditor.hxx"

class GraphicConfigEditor;

namespace CLAM_Annotator { class Project; }

class ProjectEditor : public QDialog
{
		Q_OBJECT
	public:
		ProjectEditor(QWidget * parent =0)
			: QDialog(parent)
		{
			ui.setupUi(this);
			ProjectEditor::on_projectInfo_textChanged();
		}
		virtual ~ProjectEditor();
		const CLAM_Annotator::Project & editedProject()
		{
			return mProject;
		}
		void setProject(const CLAM_Annotator::Project & project);
		void applyChanges(CLAM_Annotator::Project & project);
		void setProjectPath(const std::string & file);
	private slots:
		void on_projectInfo_textChanged(); // no
		void on_configurationInfo_textChanged(); // done
		void on_schemaBrowseButton_clicked(); // ?
		void on_suffix_editTextChanged(); // done
		void on_extractor_textChanged(); // done
		void on_extractorBrowseButton_clicked();
		void on_loadConfigurationButton_clicked(); //
		void on_testExtractorButton_clicked();
		void on_newSourceButton_clicked();
		void on_removeSourceButton_clicked();
		void on_sources_itemActivated(QListWidgetItem * item);
		void on_mappings_itemChanged(QTreeWidgetItem * item, int col);
	private:
		void updateFields();
	private:
		Ui::ProjectEditor ui;
		CLAM_Annotator::Project mProject;
		GraphicConfigEditor * mGraphicConfigEditor;
};

class SourceEditor  : public QDialog
{
		Q_OBJECT
	public:
		SourceEditor(CLAM_Annotator::Extractor & extractor, QWidget * parent =0)
			: QDialog(parent)
			, _extractor(extractor)
		{
			_ui.setupUi(this);
			updateFields();
		}
	private slots:
		void on_loadConfigurationButton_clicked();
		void on_extractorBrowseButton_clicked();
		void on_extractor_textChanged();
		void on_sourceName_textChanged();
		void on_configurationInfo_textChanged();
		void on_suffix_editTextChanged();
		void on_buttonBox_accepted();
	private:
		void updateFields();
		bool check();
		Ui::SourceEditor _ui;
		CLAM_Annotator::Extractor & _extractor;
	
};

#endif//ProjectEditor_hxx

