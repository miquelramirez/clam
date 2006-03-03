#ifndef TaskRunner_hxx
#define TaskRunner_hxx

#include <QtGui/QWidget>
#include <QtGui/QTextEdit>
#include <QtCore/QProcess>
#include <QtCore/QTimer>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDockWidget>
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
	}
	virtual ~TaskRunner();
	bool run(QString command, QStringList & arguments, QString workingDir)
	{
		std::cout << "yop" << std::endl;
		mProcess = new QProcess(this);
		mProcess->setWorkingDirectory(workingDir);
		connect(mProcess, SIGNAL(readyReadStandardError()), this, SLOT(dumpError()));
		connect(mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(dumpOutput()));
		connect(mProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished()));
		mOutputDisplay->setHtml("<div style='color: blue;'>Executing "+ command +"</div>");
		mProcess->start(command, arguments);
		return mProcess->waitForStarted();
	}
private slots:
	void finished()
	{
		std::cout << "finished" << std::endl;
		mOutputDisplay->append("<div style='color: blue;'>Done.</div>");
		QTimer::singleShot(5000, this, SLOT(close()));
	}
	void dumpError()
	{
		std::cout << "Weo" << std::endl;
		mOutputDisplay->append("<pre style='color: red;'>"+  mProcess->readAllStandardError() +"</pre>");
	}
	void dumpOutput()
	{
		std::cout << "douuu" << std::endl;
		mOutputDisplay->append("<pre style='color: green;'>"+  mProcess->readAllStandardOutput() +"</pre>");
	}
private:
	QTextEdit * mOutputDisplay;
	QProcess * mProcess;
	
};


#endif//TaskRunner_hxx
