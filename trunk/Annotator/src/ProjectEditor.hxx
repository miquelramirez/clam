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
		void on_projectInfo_textChanged();
		void on_configurationInfo_textChanged();
		void on_schemaBrowseButton_clicked();
		void on_suffix_editTextChanged();
		void on_extractor_textChanged();
		void on_extractorBrowseButton_clicked();
		void on_configurationBrowseButton_clicked();
		void on_configuration_textChanged();
		void on_configurationEditButton_clicked();
		void on_buttonBox_accepted();
		void on_buttonBox_rejected();
	private:
		void updateFields();
		void updateProject();
	private:
		Ui::ProjectEditor ui;
		CLAM_Annotator::Project mProject;
};

#endif//ProjectEditor_hxx

