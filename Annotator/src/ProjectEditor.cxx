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

#include "ProjectEditor.hxx"
#include "ConfigurationEditor.hxx"
#include "GraphicConfigEditor.hxx"
#include <QtGui/QFileDialog>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QString>
#include <iostream>

ProjectEditor::~ProjectEditor()
{
}

// New project initialization
void ProjectEditor::setProjectPath(const std::string & file)
{
	mProject.AddAll();
	mProject.UpdateData();
	mProject.SetProjectPath(file);
	mProject.SetDescription(
		tr("<h1>My annotation project</h1>\n"
		"<h2>Description</h2>\n"
		"<p>This is a place holder for the project documentation."
		"Edit at will.</p>\n").toStdString()
		);
	updateFields();
}

// Edit project initialization
void ProjectEditor::setProject(const CLAM_Annotator::Project & project)
{
	mProject.AddAll();
	mProject.UpdateData();
	mProject.SetProjectPath(project.File());
	mProject.SetDescription(project.GetDescription());
	mProject.SetSchema(project.GetSchema());
	mProject.SetExtractor(project.GetExtractor());
	if (project.HasConfiguration())
	{
		mProject.SetConfiguration(project.GetConfiguration());
	}
	mProject.SetPoolSuffix(project.PoolSuffix());
	if (project.HasViews())
		mProject.SetViews(project.GetViews());
	updateFields();
}

void ProjectEditor::applyChanges(CLAM_Annotator::Project & project)
{
	project.AddAll();
	project.UpdateData();
	project.SetProjectPath(mProject.File());
	project.SetDescription(mProject.GetDescription());
	project.SetConfiguration(mProject.GetConfiguration());
	project.SetSchema(mProject.GetSchema());
	project.SetExtractor(mProject.GetExtractor());
	project.SetPoolSuffix(mProject.PoolSuffix());
}

void ProjectEditor::updateFields()
{
	ui.projectFile->setText(mProject.File().c_str());
	ui.schema->setText(mProject.GetSchema().c_str());
	ui.suffix->setEditText(mProject.PoolSuffix().c_str());
	if (mProject.HasExtractor())
		ui.extractor->setText(mProject.GetExtractor().c_str());
	ui.projectInfo->setPlainText(mProject.GetDescription().c_str());
	ui.configurationInfo->setPlainText(mProject.GetConfiguration().c_str());
	ui.htmlPreview->setHtml(mProject.GetDescription().c_str());
}


void ProjectEditor::on_projectInfo_textChanged()
{
	ui.htmlPreview->setHtml(ui.projectInfo->toPlainText());
	mProject.SetDescription(ui.projectInfo->toPlainText().toStdString());
}

void ProjectEditor::on_configurationInfo_textChanged()
{
	mProject.SetConfiguration(ui.configurationInfo->toPlainText().toStdString());
}

void ProjectEditor::on_suffix_editTextChanged()
{
	mProject.SetPoolSuffix(ui.suffix->currentText().toStdString());
}

void ProjectEditor::on_schemaBrowseButton_clicked()
{
	QString file = QFileDialog::getOpenFileName(this, 
			"Open a Description Scheme",
			mProject.RelativeToAbsolute(mProject.GetSchema()).c_str(),
			"Description Schemes (*.sc)"
			);
	if (file.isNull()) return;
	// TODO: Check that the schema loads well
	mProject.SetSchema(mProject.AbsoluteToRelative(file.toStdString()));
	updateFields();
}

void ProjectEditor::on_extractorBrowseButton_clicked()
{
	QString file = QFileDialog::getOpenFileName(this, 
			"Select an extractor executable",
			mProject.GetExtractor().c_str(),
			"Executable file (*)"
			);
	if (file.isNull()) return;
	mProject.SetExtractor(file.toStdString());
	updateFields();
}

void ProjectEditor::on_loadConfigurationButton_clicked()
{
	QString file = QFileDialog::getOpenFileName(this, 
			"Select a configuration file",
			0,
			"Configuration file (*)"
			);
	if (file.isNull()) return;
	std::cout << "Loading config " << file.toStdString() << std::endl;
	QFile loadedConfig(file);
	if (not loadedConfig.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, tr("Loading configuration"),
			tr("Unable to open the selected file"));
		return;
	}
	CLAM::Text configData = loadedConfig.readAll().constData();
	mProject.SetConfiguration(configData);
	updateFields();
}


void ProjectEditor::on_graphicalEditButton_clicked()
{
	mGraphicConfigEditor = new GraphicConfigEditor(this);
	mGraphicConfigEditor->setConfiguration(mProject.GetConfiguration().c_str());
	if(mGraphicConfigEditor->exec()==QDialog::Rejected) return;	

	mProject.SetConfiguration(mGraphicConfigEditor->getConfiguration()); // parameter of std::string type 
}

void ProjectEditor::on_extractor_textChanged()
{
	mProject.SetExtractor(ui.extractor->text().toStdString());
}



