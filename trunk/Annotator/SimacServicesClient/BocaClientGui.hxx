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

#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>
#include <QtGui/QMessageBox>
#include <QtCore/QFileInfo>
#include "ui_GUI.hxx"
#include "BocaTaskRunner.hxx"

#ifndef I18N_PATH
#ifdef WIN32
#define I18N_PATH "../"
#else
#define I18N_PATH "/home/vokimon/CajitasDeArena/CLAM-Install/"
#endif
#endif

#ifdef WIN32
#define BocaTaskManager "BocaTaskManager"
#else
#define BocaTaskManager "SimacServicesClient/Manager.py"
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
		if (!taskFile.isEmpty())
		{
			QFileInfo fileInfo(taskFile);
			if (projectFile.isEmpty())
			{
				projectFile = fileInfo.completeBaseName();
			}
			if (path.isEmpty())
			{
				path = fileInfo.path();
			}
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
		QString task = ui.taskEdit->text();
		QString project = ui.projectEdit->text();
		QString path = ui.pathEdit->text();

		unsigned previousStep = currentStep++;
		switch (previousStep)
		{
			case 0:
			{
				ui.logEdit->run(BocaTaskManager, QStringList() 
						<< "download"
						<< task
						<< project
						<< path
						, ".");
			} break;
			case 1:
			{
				if (previousStepErrorCode!=0)
				{
					currentStep=0;
					return;
				}
				runAnnotator(path + "/" + project + ".pro");
				QProcess lister;
				lister.start(BocaTaskManager, QStringList() 
						<< "listmodified"
						<< task
						<< project
						<< path
						);
				lister.waitForFinished();
				QString files = lister.readAll();
				if (files.trimmed().isEmpty())
				{
					QMessageBox::warning(this, tr("No descriptors downloaded"),
							tr("No modification has been detected. No upload needed.")
							);
					currentStep=0;
					return;
				}
				int response = QMessageBox::question(this, tr("Uploading changes"),
						tr("The following files have been updated.\n\n")
						+files
						+tr("\nDo you want to upload them back to the BOCA server?"),
						tr("Upload"), tr("Don't upload")
						);
				if (response==0)
				{
					ui.logEdit->run(BocaTaskManager, QStringList() 
							<< "upload"
							<< task
							<< project
							<< path
							,".");
				}
			} break;
			case 2:
			{
				currentStep=0;
			} break;
			default:
			std::cerr << "Step no esperat " << previousStep;
		}
	}
	void runAnnotator(QString project)
	{
			hide();
			update();
			QString command;
			command += "Annotator \"";
			command += project;
			command += "\"";
			std::system(command.toStdString().c_str());
			show();
			update();
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


