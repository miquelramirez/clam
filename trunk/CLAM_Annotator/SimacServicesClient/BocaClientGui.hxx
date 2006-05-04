#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>
#include <QtGui/QMessageBox>
#include "ui_GUI.hxx"
#include "BocaTaskRunner.hxx"

#ifndef I18N_PATH
#ifdef WIN32
#define I18N_PATH "../"
#else
#define I18N_PATH "/home/vokimon/CajitasDeArena/CLAM-Install/"
#endif
#endif

class BocaClientGui : public QDialog
{
	Q_OBJECT
private:
	Ui::GUI ui;
public:
	BocaClientGui(QString taskFile, QString projectFile, QString path)
	{
		ui.setupUi(this);
		if (!taskFile.isEmpty() && projectFile.isEmpty())
		{
			// TODO: The project was a name not a file!!
			projectFile = taskFile;
			projectFile.replace(".task","");
			projectFile += ".pro";
		}
		ui.taskEdit->setText(taskFile);
		ui.projectEdit->setText(projectFile);
		ui.pathEdit->setText(path);
		updateButtonsEnabling();
		connect(ui.logEdit, SIGNAL( taskEnded(int) ), this, SLOT( completedStep(int) ) );
		currentStep = 0;
	}
	~BocaClientGui() {}
public slots:
	void on_taskButton_clicked()
	{
		QString s = QFileDialog::getOpenFileName(
				this, tr("Choose a BOCA task file"), ".",
				tr("Simac Task file (*.task)\nAll Files (*)")
			);
		if ( s.isNull() ) return;
		ui.taskEdit->setText(s);
	}
	void on_pathButton_clicked()
	{
		ui.goButton->setEnabled(false);
		QString s = QFileDialog::getExistingDirectory(
				this, "Choose a directory to place the project", ".",
				QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
			);
		if ( s.isNull() ) return;
		ui.pathEdit->setText(s);
	}
	void on_taskEdit_textChanged()
	{
		updateButtonsEnabling();
	}
	void on_projectEdit_textChanged()
	{
		updateButtonsEnabling();
	}
	void on_pathEdit_textChanged()
	{
		updateButtonsEnabling();
	}
	void on_goButton_clicked()
	{
		completedStep(0);
	}
	void completedStep(int previousStepErrorCode)
	{
		unsigned previousStep = currentStep++;
		switch (previousStep)
		{
			case 0:
			{
				ui.logEdit->run("python", QStringList() 
						<< "Manager.py"
						<< "processtask"
						<< "sample.task"
						<< "project"
						<< "/home/vokimon/borrame",
						"SimacServicesClient");
			} break;
			case 1:
			{
				ui.logEdit->run("Annotator", QStringList() 
						<< "/home/vokimon/borrame/project.pro",
						".");
			} break;
			case 2:
			{
				ui.logEdit->run("python", QStringList() 
						<< "Manager.py"
						<< "upload"
						<< "modifiedList",
						"SimacServicesClient");
			} break;
			case 3:
			{
			} break;
			default:
			std::cerr << "Step no esperat " << previousStep;
		}
	}
private:
	void updateButtonsEnabling()
	{
		bool goEnabled = true;
		if (ui.taskEdit->text().isEmpty()) goEnabled=false;
		if (ui.projectEdit->text().isEmpty()) goEnabled=false;
		if (ui.pathEdit->text().isEmpty()) goEnabled=false;
		// TODO: Check the path is valid
		// TODO: Check the task is valid
		ui.goButton->setEnabled(goEnabled);
	}
	unsigned currentStep;
};


