/*
 * V2MHelpViewer.hxx
 * Interface for V2MHelpViewer class
 * The help system.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#ifndef _V2MHELPVIEWER_
#define _V2MHELPVIEWER_

#include <qwidget.h>

class QTextBrowser;
class QFrame;
class QPushButton;

class V2MHelpViewer : public QWidget
{
	Q_OBJECT
public:
	V2MHelpViewer();

protected:
	void closeEvent(QCloseEvent *);

signals:
	void closed();

private slots:
    void textChanged();

private:
	QTextBrowser *browser;

	QFrame* topPanel;
	QFrame* container;

	QPushButton *buttonBackward,*buttonForward,*buttonHome,*buttonClose;

	void InitBrowser();
	void InitPanel();
};

#endif
