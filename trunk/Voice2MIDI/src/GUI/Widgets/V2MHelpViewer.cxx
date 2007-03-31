/*
 * V2MHelpViewer.cxx
 * Implementation for V2MHelpViewer class. 
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include <q3textbrowser.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qfont.h>
#include <q3frame.h>
#include "V2MIconData.hxx"
#include "V2MHelpViewer.hxx"

V2MHelpViewer::V2MHelpViewer() : QWidget(0,"help window",Qt::WStyle_Customize|Qt::WStyle_Title|Qt::WStyle_SysMenu|Qt::WStyle_Minimize|Qt::WDestructiveClose)
{
	setFixedSize(640,480);
	setGeometry(100,100,width(),height());

	// set icon ///////////////////////////////
	QPixmap main_icon((const char**)icon_help_data);
	setIcon(main_icon);

	InitBrowser();
	InitPanel();
}

void V2MHelpViewer::InitBrowser()
{
	container = new Q3Frame(this);
	container->setFixedSize(width(),440);
	container->setGeometry(0,40,container->width(),container->height());

	browser = new Q3TextBrowser(container);
	browser->setFixedSize(container->width(),container->height());

    browser->mimeSourceFactory()->setFilePath("doc/");
    browser->setFrameStyle(Q3Frame::Panel | Q3Frame::Sunken);
    connect(browser,SIGNAL(textChanged()),this,SLOT(textChanged()));

	browser->setSource("index.htm");
}

void V2MHelpViewer::InitPanel()
{
	// top panel //////////////////////////////////////////
	topPanel = new Q3Frame(this);
	topPanel->setFixedSize(width(),40);
    topPanel->setGeometry(0,0,width(),40); 
    topPanel->setFrameStyle(Q3Frame::Panel | Q3Frame::Sunken);

	// icons /////////////////////////////////////////////
	QPixmap icon_backward((const char**)icon_back_data);
	QPixmap icon_forward((const char**)icon_forward_data);
	QPixmap icon_home((const char**)icon_home_data);

	// backward button 
    buttonBackward = new QPushButton(topPanel);
	buttonBackward->setPixmap(icon_backward);
	buttonBackward->setFixedSize(30,30);
	buttonBackward->setGeometry(10,5,30,30);
	connect(buttonBackward,SIGNAL(clicked()),browser,SLOT(backward()));
    connect(browser,SIGNAL(backwardAvailable(bool)),buttonBackward,SLOT(setEnabled(bool)));
    buttonBackward->setEnabled(false);
	QToolTip::add(buttonBackward,"backward");
    
	// forward button  
	buttonForward = new QPushButton(topPanel);
	buttonForward->setPixmap(icon_forward);
	buttonForward->setFixedSize(30,30);
	buttonForward->setGeometry(buttonBackward->x()+buttonBackward->width(),5,30,30);
	connect(buttonForward,SIGNAL(clicked()),browser,SLOT(forward()));
    connect(browser,SIGNAL(forwardAvailable(bool)),buttonForward,SLOT(setEnabled(bool)));
    buttonForward->setEnabled(false);
	QToolTip::add(buttonForward,"forward");

    // home button 
	buttonHome = new QPushButton(topPanel);
	buttonHome->setPixmap(icon_home);
	buttonHome->setFixedSize(30,30);
	buttonHome->setGeometry(buttonForward->x()+buttonForward->width(),5,30,30);
	connect(buttonHome,SIGNAL(clicked()),browser,SLOT(home()));
	QToolTip::add(buttonHome,"home");

	// close button 
	QFont f("Helvetica",10,QFont::Bold);
	buttonClose = new QPushButton(topPanel);
	buttonClose->setFixedSize(60,30);
	buttonClose->setGeometry(topPanel->width()-70,5,60,30);
	buttonClose->setFont(f);
	buttonClose->setText("Close");
	connect(buttonClose,SIGNAL(clicked()),this,SLOT(close()));
	QToolTip::add(buttonClose,"window close");

}

void V2MHelpViewer::textChanged()
{
	if(browser->documentTitle().isNull()) 
	{
		setCaption( "Voice2MIDI help - " + browser->context());
    }
    else 
	{
		setCaption( "Voice2MIDI help - " + browser->documentTitle());
    }
    
}

void V2MHelpViewer::closeEvent(QCloseEvent *)
{
	emit closed();
}

// END

