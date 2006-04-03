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
		void on_schemaBrowseButton_clicked();
		void on_suffix_editTextChanged();
		void on_extractorBrowseButton_clicked();
	private:
		void updateFields();
		void updateProject();
	private:
		Ui::ProjectEditor ui;
		CLAM_Annotator::Project mProject;
};

#endif//ProjectEditor_hxx

