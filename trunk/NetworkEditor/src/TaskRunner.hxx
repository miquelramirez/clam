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

#ifndef TaskRunner_hxx
#define TaskRunner_hxx

#include <QtGui/QWidget>
#include <QtGui/QTextEdit>
#include <QtCore/QProcess>
//#include <QtCore/QTimer>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDockWidget>
#include <QtGui/QTextCursor>
#include <QtGui/QStatusBar>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QPushButton>
#include <iostream>



class TaskRunner : public QDockWidget
{
	Q_OBJECT
public:
	TaskRunner(QString objectName="TaskRunnerWidget", QWidget * parent = 0):
		_process(0)
	{
		_outputDisplay = new QTextEdit(this);
		QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel
                                      | QDialogButtonBox::Close,Qt::Vertical,this);
		_cancelButton=buttonBox->button(QDialogButtonBox::Cancel);
		_closeButton=buttonBox->button(QDialogButtonBox::Close);
		_cancelButton->setEnabled(true);
		_closeButton->setEnabled(false);
		connect(_closeButton, SIGNAL(clicked()), this, SLOT(close()));
		QHBoxLayout * mainBox=new QHBoxLayout();
		mainBox->addWidget(_outputDisplay);
		mainBox->addWidget(buttonBox);
		//TODO: keep the default dimensions without an intermediate widget
		//setWidget(dynamic_cast<QWidget *>(mainBox));
		QWidget * widget= new QWidget(this);
		widget->setLayout(mainBox);
		setWidget(widget);
		setAttribute(Qt::WA_DeleteOnClose, true);
		setObjectName(objectName);
	}

	virtual ~TaskRunner();
	typedef struct
	{
		QString command;
		QStringList arguments;
		QString workingDir;
	} CommandAndEnvironment;

	typedef QList<CommandAndEnvironment> CommandsAndEnvironmentsList;

	bool run (CommandsAndEnvironmentsList & commandsList, bool stopOnError=true)
	{
		_error=false;
		_stopOnError=stopOnError;
		_output.clear();
		_queuedCommandsList=commandsList;
		if (_queuedCommandsList.size()==0)
			return 0;
		return getAndRunQueuedCommand();
	}
	bool run (QString command, QStringList & arguments, QString workingDir)
	{
		_error=false;
		_output.clear();
		_queuedCommandsList.clear();
		return runCommand(command,arguments,workingDir);
	}

signals:
	void taskDone(bool success);
	void widgetDestructed();
private slots:
	void dumpError()
	{
		dump(_process->readAllStandardError(), true);
	}
	void dumpOutput()
	{
		dump(_process->readAllStandardOutput(), false);
	}
	void finishedCommand()
	{
		_error=(_error || (_process->exitCode()!=0));
		dumpOutput();
		dumpError();
		_output += tr("<div style='color: blue;'>Done.</div>");
		updateText();
		if (_queuedCommandsList.size()==0 or (_stopOnError and _error))
			finishedQueue();
		else
			getAndRunQueuedCommand();
	}
	void cancelProcess()
	{
		_queuedCommandsList.clear();
		_error=true;
		if (_process->state() != QProcess::NotRunning)
			_process->terminate();
	}
private:
	bool runCommand(QString command, QStringList & arguments, QString workingDir)
	{
		_output += ("<div style='color: blue;'>" + tr("Executing '<tt>%1 %2</tt>'") + "</div><br/>")
			.arg(command)
			.arg(arguments.join(" "));
		updateText();
		_process = new QProcess(this);
		_process->setWorkingDirectory(workingDir);
		connect(_cancelButton, SIGNAL(clicked()), this, SLOT(cancelProcess()));
		connect(_process, SIGNAL(readyReadStandardError()), this, SLOT(dumpError()));
		connect(_process, SIGNAL(readyReadStandardOutput()), this, SLOT(dumpOutput()));
		connect(_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finishedCommand()));
		_process->start(command, arguments);
		return _process->waitForStarted();
	}
	bool getAndRunQueuedCommand()
	{
		CommandAndEnvironment command = _queuedCommandsList.takeFirst();
		return runCommand(command.command, command.arguments, command.workingDir);
	}
	void finishedQueue()
	{
		_closeButton->setEnabled(true);
		_cancelButton->setEnabled(false);
		emit taskDone(!_error);
//		QTimer::singleShot(5000, this, SLOT(close()));
		//std::cout<<"Queued commands running finished!\n";
	}

	void dump(QString output, bool error)
	{
		output.replace("&","&amp;");
		output.replace("<","&lt;");
		output.replace("\n","<br/>");
		_output += QString("<tt style='color: ") + (error?"red":"green") + ";'>"+  output +"</tt>";
		updateText();
	}
	void updateText()
	{
		_outputDisplay->setHtml(_output);
		QTextCursor cursor = _outputDisplay->textCursor();
		cursor.movePosition(QTextCursor::End);
		_outputDisplay->setTextCursor(cursor);
		_outputDisplay->ensureCursorVisible();
	}
private:
	QTextEdit * _outputDisplay;
	QProcess * _process;
	QString _output;
	CommandsAndEnvironmentsList _queuedCommandsList;
	bool _error;
	bool _stopOnError;
	QPushButton * _cancelButton;
	QPushButton * _closeButton;
};
#endif//TaskRunner_hxx

