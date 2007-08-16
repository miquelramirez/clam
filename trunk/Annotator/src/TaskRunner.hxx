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
		mOutputDisplay = new QTextEdit(this);
//		QVBoxLayout * layout = new QVBoxLayout(this);
		layout()->addWidget(mOutputDisplay);
		setAttribute(Qt::WA_DeleteOnClose, true);
	}
	virtual ~TaskRunner();
	bool run(QString command, QStringList & arguments, QString workingDir)
	{
		mOutput = ("<div style='color: blue;'>" + tr("Executing '<tt>%1 %2</tt>'") + "</div><br/>")
			.arg(command)
			.arg(arguments.join(" "));
		updateText();
		mProcess = new QProcess(this);
		mProcess->setWorkingDirectory(workingDir);
		connect(mProcess, SIGNAL(readyReadStandardError()), this, SLOT(dumpError()));
		connect(mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(dumpOutput()));
		connect(mProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished()));
		mProcess->start(command, arguments);
		return mProcess->waitForStarted();
	}
signals:
	void taskDone(bool success);
private slots:
	void finished()
	{
		dumpOutput();
		dumpError();
		mOutput += tr("<div style='color: blue;'>Done.</div>");
		updateText();
		QTimer::singleShot(5000, this, SLOT(close()));
		emit taskDone(mProcess->exitCode()==0);
	}
	void dumpError()
	{
		dump(mProcess->readAllStandardError(), true);
	}
	void dumpOutput()
	{
		dump(mProcess->readAllStandardOutput(), false);
	}
private:
	void dump(QString output, bool error)
	{
		output.replace("&","&amp;");
		output.replace("<","&lt;");
		output.replace("\n","<br/>");
		mOutput += QString("<tt style='color: ") + (error?"red":"green") + ";'>"+  output +"</tt>";
		updateText();
	}
	void updateText()
	{
		mOutputDisplay->setHtml(mOutput);
		QTextCursor cursor = mOutputDisplay->textCursor();
		cursor.movePosition(QTextCursor::End);
		mOutputDisplay->setTextCursor(cursor);
		mOutputDisplay->ensureCursorVisible();
	}
private:
	QTextEdit * mOutputDisplay;
	QProcess * mProcess;
	QString mOutput;
};


#endif//TaskRunner_hxx

