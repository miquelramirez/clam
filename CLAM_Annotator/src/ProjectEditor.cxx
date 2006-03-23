#include "ProjectEditor.hxx"
#include "Project.hxx"
#include <QtGui/QFileDialog>
#include <iostream>

ProjectEditor::~ProjectEditor()
{
}

void ProjectEditor::setProject(const CLAM_Annotator::Project & project)
{
	mProject = project;
	updateFields();
}
void ProjectEditor::updateFields()
{
	ui.schema->setText(mProject.GetSchema().c_str());
	ui.suffix->setEditText(mProject.PoolSuffix().c_str());
	if (mProject.HasExtractor())
		ui.extractor->setText(mProject.GetExtractor().c_str());
	ui.projectInfo->setPlainText(mProject.GetDescription().c_str());
	ui.htmlPreview->setHtml(mProject.GetDescription().c_str());
}

void ProjectEditor::on_projectInfo_textChanged()
{
	ui.htmlPreview->setHtml(ui.projectInfo->toPlainText());
	mProject.SetDescription(ui.projectInfo->toPlainText().toStdString());
}

void ProjectEditor::on_suffix_editTextChanged()
{
	mProject.SetPoolSuffix(ui.suffix->currentText().toStdString());
	updateFields();
}

void ProjectEditor::on_schemaBrowseButton_clicked()
{
	QString file = QFileDialog::getOpenFileName(this, 
			"Open a Description Scheme",
			mProject.GetSchema().c_str(),
			"Description Schemes (*.sc)"
			);
	if (file.isNull()) return;
	// TODO: Check that the schema loads well
	mProject.SetSchema(file.toStdString());
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


