#include "ProjectEditor.hxx"
#include "Project.hxx"
#include <QtGui/QFileDialog>
#include <iostream>

ProjectEditor::~ProjectEditor()
{
}

void ProjectEditor::setProject(CLAM_Annotator::Project & project)
{
	mProject = & project;
	ui.schema->setText(project.GetSchema().c_str());
	ui.suffix->setEditText(project.PoolSuffix().c_str());
	if (project.HasExtractor())
		ui.extractor->setText(project.GetExtractor().c_str());
	ui.projectInfo->setPlainText(project.GetDescription().c_str());
	ui.htmlPreview->setHtml(project.GetDescription().c_str());
}

void ProjectEditor::on_projectInfo_textChanged()
{
	ui.htmlPreview->setHtml(ui.projectInfo->toPlainText());
	mProject->SetDescription(ui.projectInfo->toPlainText().toStdString());
}

void ProjectEditor::on_schemaBrowseButton_clicked()
{
	QString file = QFileDialog::getOpenFileName(this, 
			"Open a Description Scheme",
			mProject->GetSchema().c_str(),
			"Description Schemes (*.sc)"
			);
	if (file.isNull()) return;
	// TODO: Check that the schema loads well
	mProject->SetSchema(file.toStdString());
	setProject(*mProject);
}

void ProjectEditor::on_extractorBrowseButton_clicked()
{
	QString file = QFileDialog::getOpenFileName(this, 
			"Select an extractor executable",
			mProject->GetSchema().c_str(),
			"Description Schemes (*.sc)"
			);
	if (file.isNull()) return;
	mProject->SetExtractor(file.toStdString());
	setProject(*mProject);
}


