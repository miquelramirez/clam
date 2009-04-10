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
	mProject.SetSources(project.GetSources());
	mProject.SetMaps(project.GetMaps());
	if (project.HasConfiguration())
	{
		mProject.SetConfiguration(project.GetConfiguration());
	}
	mProject.SetPoolSuffix(project.PoolSuffix());
	if (project.HasViews())
		mProject.SetViews(project.GetViews());
	for (unsigned i=0; i<mProject.GetSources().size(); i++)
	{
		CLAM_Annotator::Extractor & extractor = mProject.GetSources()[i];
		extractor.generateSchema(this);
	}
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
	project.SetSources(mProject.GetSources());
	project.SetMaps(mProject.GetMaps());
	std::cout << "Aggregation Script:\n" 
		<< project.aggregationScript() << std::endl;
}

void ProjectEditor::updateFields()
{
	ui.projectFile->setText(mProject.File().c_str());
//	ui.schema->setText(mProject.GetSchema().c_str());
	ui.suffix->setEditText(mProject.PoolSuffix().c_str());
//	if (mProject.HasExtractor())
//		ui.extractor->setText(mProject.GetExtractor().c_str());
	ui.projectInfo->setPlainText(mProject.GetDescription().c_str());
//	ui.configurationInfo->setPlainText(mProject.GetConfiguration().c_str());
	ui.htmlPreview->setHtml(mProject.GetDescription().c_str());
	ui.sources->clear();
	for (unsigned i=0; i<mProject.GetSources().size(); i++)
	{
		CLAM_Annotator::Extractor & extractor = mProject.GetSources()[i];
		QString itemName = extractor.GetName().c_str();
		QListWidgetItem * item = new QListWidgetItem(itemName);
		ui.sources->addItem(item);
	}
	ui.mappings->bindProject(mProject);
}



void ProjectEditor::on_newSourceButton_clicked()
{
	CLAM_Annotator::Extractor extractor;
	extractor.SetName("Extractor");
	SourceEditor * editor = new SourceEditor(extractor, this);
	if (editor->exec() != QDialog::Accepted) return; // Cancelled
	mProject.GetSources().push_back(extractor);
	updateFields();
}

void ProjectEditor::on_sources_itemActivated(QListWidgetItem * item)
{
	int row = ui.sources->row(item);
	int size = mProject.GetSources().size();
	CLAM_ASSERT(row>=0 && row<size,
		"ProjectEditor: Unexpected item");

	CLAM_Annotator::Extractor extractor = mProject.GetSources()[row]; // Copia
	SourceEditor * editor = new SourceEditor(extractor, this);
	if (editor->exec() != QDialog::Accepted) return; // Cancelled

	mProject.GetSources()[row] = extractor; // Commit
	updateFields();
}

void ProjectEditor::on_mappings_itemChanged(QTreeWidgetItem * item, int col)
{
	std::cout << "* Mappings item changed" << std::endl;
	ui.mappings->takeMaps();
	CLAM::XMLStorage::Dump(mProject, "Project", std::cout);
}

void ProjectEditor::on_projectInfo_textChanged()
{
	ui.htmlPreview->setHtml(ui.projectInfo->toPlainText());
	mProject.SetDescription(ui.projectInfo->toPlainText().toStdString());
}

void ProjectEditor::on_configurationInfo_textChanged()
{
//	mProject.SetConfiguration(ui.configurationInfo->toPlainText().toStdString());
}

void ProjectEditor::on_suffix_editTextChanged()
{
	mProject.SetPoolSuffix(ui.suffix->currentText().toStdString());
}
void ProjectEditor::on_extractor_textChanged()
{
//	mProject.SetExtractor(ui.extractor->text().toStdString());
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
	CLAM_Annotator::Project copyProject = mProject;
	QDialog dialog(this);
	dialog.setLayout(new QVBoxLayout(&dialog));
	AggregationEditor * mapEditor = new AggregationEditor(this);
	dialog.layout()->addWidget(mapEditor);
	QDialogButtonBox * buttons = new QDialogButtonBox(&dialog);
	dialog.layout()->addWidget(buttons);
	connect(buttons, SIGNAL(accepted()), &dialog, SLOT(accept()));
	connect(buttons, SIGNAL(rejected()), &dialog, SLOT(reject()));
	mapEditor->bindProject(copyProject);
	mapEditor->loadConfig(mProject.GetConfiguration());
	if(dialog.exec()==QDialog::Rejected) return;	
	mapEditor->takeMaps();
	mProject = copyProject;
	updateFields();
}

void ProjectEditor::on_testExtractorButton_clicked()
{
	QString file = QFileDialog::getOpenFileName(this, 
			"Open a Description Scheme",
			0,
			"Wavefile (*.mp3)"
			);
	if (file.isNull()) return;
	CLAM_Annotator::Extractor extractor;
	mProject.dumpExtractorInfo(extractor);
//	if (not extractor.computeDescriptors(this, file))
//		ui.extractor->setStyleSheet("background:#fbb; color:black;");
}

///// Source Editor

void SourceEditor::updateFields()
{
	_ui.sourceName->setText(_extractor.GetName().c_str());
//	_ui.schema->setText(_extractor.GetSchema().c_str());
	_ui.suffix->setEditText(_extractor.GetPoolSuffix().c_str());
//	_ui.extractor->setText(_extractor.GetExtractor().c_str());
//	_ui.configurationInfo->setPlainText(_extractor.GetConfiguration().c_str());
}

void SourceEditor::on_loadConfigurationButton_clicked()
{
	QString file = QFileDialog::getOpenFileName(this, 
			"Select a configuration file to load",
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
	_extractor.SetConfiguration(configData);
	updateFields();
}

void SourceEditor::on_extractorBrowseButton_clicked()
{
	QString file = QFileDialog::getOpenFileName(this, 
			"Select an extractor executable",
			_extractor.GetExtractor().c_str(),
			"Executable file (*)"
			);
	if (file.isNull()) return;
	_extractor.SetExtractor(file.toStdString());
	updateFields();
}

void SourceEditor::on_sourceName_textChanged()
{
//	_extractor.SetName(_ui.sourceName->text().toStdString());
}

void SourceEditor::on_extractor_textChanged()
{
//	_extractor.SetExtractor(_ui.extractor->text().toStdString());
}
void SourceEditor::on_configurationInfo_textChanged()
{
	_extractor.SetConfiguration(_ui.configurationInfo->toPlainText().toStdString());
}

void SourceEditor::on_suffix_editTextChanged()
{
	_extractor.SetPoolSuffix(_ui.suffix->currentText().toStdString());
}

void SourceEditor::on_buttonBox_accepted()
{
	if (not check()) return;
	accept();
}

bool SourceEditor::check()
{
	QString context = "Checking extractor";
	_ui.extractor->setStyleSheet("");
	_ui.configurationInfo->setStyleSheet("");
	if (not _extractor.isRunnable(this))
	{
		_ui.extractor->setStyleSheet("background-color: #fbb; color:black;");
		QMessageBox::warning(this, context,
			tr("The extractor command is not runnable"));
		return false;
	}
	try
	{
		if (not _extractor.generateSchema(this))
		{
			_ui.configurationInfo->setStyleSheet("background-color: #fbb; color:black;");
			QMessageBox::warning(this, context,
				tr("Cannot obtain the schema from the extractor."));
			return false;
		}
	}
	catch (CLAM::XmlStorageErr & e)
	{
		_ui.extractor->setStyleSheet("background-color: #fbb; color:black;");
		QMessageBox::warning(this, context,
			tr("Generated schema is not valid.\n%1")
			.arg(e.what())
		);
		return false;
	}
	return true;
}



