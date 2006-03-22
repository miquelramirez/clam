#ifndef ProjectEditor_hxx
#define ProjectEditor_hxx

#include "ui_ProjectEditor.hxx"
namespace CLAM_Annotator { class Project; }

class ProjectEditor : public QDialog
{
		Q_OBJECT
	public:
		ProjectEditor(QWidget * parent =0)
			: QDialog(parent)
		{
			ui.setupUi(this);
		}
		virtual ~ProjectEditor();
		void setProject(CLAM_Annotator::Project & project);
	private slots:
		void on_projectInfo_textChanged();
		void on_schemaBrowseButton_clicked();
		void on_extractorBrowseButton_clicked();
	private:
		Ui::ProjectEditor ui;
		CLAM_Annotator::Project * mProject;
};

#endif//ProjectEditor_hxx
