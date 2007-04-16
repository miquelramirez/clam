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

class BocaTaskRunner : public QTextEdit
{
	Q_OBJECT
public:
	BocaTaskRunner(QWidget * parent = 0)
	{
//		QVBoxLayout * layout = new QVBoxLayout(this);
		setAttribute(Qt::WA_DeleteOnClose, true);
	}
	virtual ~BocaTaskRunner();
	bool run(QString command, QStringList & arguments, QString workingDir)
	{
		mOutput += ("<div style='color: blue;'>" + tr("Executing '<tt>%1 %2</tt>'") + "</div><br/>")
			.arg(command)
			.arg(arguments.join(" "));
		updateText();
		mProcess = new QProcess(this);
		mProcess->setWorkingDirectory(workingDir);
		connect(mProcess, SIGNAL(readyReadStandardError()), this, SLOT(dumpError()));
		connect(mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(dumpOutput()));
		connect(mProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
		mProcess->start(command, arguments);
		return mProcess->waitForStarted();
	}
signals:
	void taskEnded(int errorCode);
private slots:
	void finished(int errorCode, QProcess::ExitStatus status)
	{
		mOutput += tr("<div style='color: blue;'>Done.</div>");
		updateText();
		emit taskEnded(errorCode);
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
		setHtml(mOutput);
		QTextCursor cursor = textCursor();
		cursor.movePosition(QTextCursor::End);
		setTextCursor(cursor);
		ensureCursorVisible();
	}
private:
	QProcess * mProcess;
	QString mOutput;
};


#endif//TaskRunner_hxx

