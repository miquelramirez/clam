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
#include <QtCore/QTimer>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDockWidget>
#include <QtGui/QTextCursor>
#include <iostream>

class TaskRunner : public QDockWidget
{
	Q_OBJECT
public:
	TaskRunner(QWidget * parent = 0)
	{
		_outputDisplay = new QTextEdit(this);
//		QVBoxLayout * layout = new QVBoxLayout(this);
		//layout()->addWidget(_outputDisplay);
		setWidget(_outputDisplay);
		setAttribute(Qt::WA_DeleteOnClose, true);
	}
	virtual ~TaskRunner();
	bool run(QString command, QStringList & arguments, QString workingDir)
	{
		_output = ("<div style='color: blue;'>" + tr("Executing '<tt>%1 %2</tt>'") + "</div><br/>")
			.arg(command)
			.arg(arguments.join(" "));
		updateText();
		_process = new QProcess(this);
		_process->setWorkingDirectory(workingDir);
		connect(_process, SIGNAL(readyReadStandardError()), this, SLOT(dumpError()));
		connect(_process, SIGNAL(readyReadStandardOutput()), this, SLOT(dumpOutput()));
		connect(_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished()));
		_process->start(command, arguments);
		return _process->waitForStarted();
	}
signals:
	void taskDone(bool success);
private slots:
	void finished()
	{
		dumpOutput();
		dumpError();
		_output += tr("<div style='color: blue;'>Done.</div>");
		updateText();
		QTimer::singleShot(5000, this, SLOT(close()));
		emit taskDone(_process->exitCode()==0);
	}
	void dumpError()
	{
		dump(_process->readAllStandardError(), true);
	}
	void dumpOutput()
	{
		dump(_process->readAllStandardOutput(), false);
	}
private:
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
};


#endif//TaskRunner_hxx

