/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "MainWindow.hxx"
#include "ProcessingTree.hxx"
#include <qaction.h>
#include <qstatusbar.h>
#include <qtoolbar.h>
#include <qbutton.h>
#include <qlayout.h> 
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qfiledialog.h>
#include <qapplication.h>
#include <qlabel.h>
#include <string>
#include "aboutdialog.h"

#include "NetworkController.hxx"

#include <iostream> // TODO: remove

namespace NetworkGUI
{

	MainWindow::MainWindow() :
		  QMainWindow( 0, "", WGroupLeader ),
		  mNetworkPresentation(this),
		  mDockProcMenu(0),
		  mNetworkIsRunning(false)
	{
		setCentralWidget( &mNetworkPresentation );
		setCaption( "CLAM Network Editor" );
		resize(800,600 );
	
		SlotSendMessageToStatus.Wrap( this, &MainWindow::SendMessageToStatus );
		mNetworkPresentation.SignalSendMessageToStatus.Connect( SlotSendMessageToStatus );
		SignalNewNetworkSignal.Connect( mNetworkPresentation.SlotClear );
		statusBar()->message( "Ready to edit" );

		SignalStartNetwork.Connect( mNetworkPresentation.SlotStartThread );
		SignalStopNetwork.Connect( mNetworkPresentation.SlotStopThread );
		SignalChangeOSCState.Connect( mNetworkPresentation.SlotChangeOSCState );


		QPopupMenu * file = new QPopupMenu( this );
		QPopupMenu * view = new QPopupMenu( this );
		QPopupMenu * networkActions = new QPopupMenu( this );
//TODO		QPopupMenu * outControlActions = new QPopupMenu( this );
		menuBar()->insertItem( "&File", file );
		menuBar()->insertItem( "&View", view );
		menuBar()->insertItem( "Network &Actions", networkActions );
		menuBar()->insertItem( "About", this, SLOT(ShowAboutDlg()));
		setCentralWidget(&mNetworkPresentation);
 

///////// FILE MENU /////////////////////////////////////////////
		QAction* newNetwork = new QAction( this, "NewNetwork" );
		newNetwork->addTo( file );
	    newNetwork->setMenuText( tr( "&New" ) );
		newNetwork->setAccel( tr( "CTRL+N" ) );
		connect( newNetwork, SIGNAL( activated() ), this, SLOT( NewNetwork() ) );

		QAction* loadNetwork = new QAction( this, "LoadNetwork" );
		loadNetwork->addTo( file );
	    loadNetwork->setMenuText( tr( "&Load" ) );
		loadNetwork->setAccel( tr( "CTRL+O" ) );
		connect( loadNetwork, SIGNAL( activated() ), this, SLOT( LoadNetwork() ) );
		
		QAction* saveNetwork = new QAction( this, "SaveNetwork" );
		saveNetwork->addTo( file );
	    saveNetwork->setMenuText( tr( "&Save" ) );
		saveNetwork->setAccel( tr( "CTRL+S" ) );
		connect( saveNetwork, SIGNAL( activated() ), this, SLOT( SaveNetwork() ) );

		QAction* saveAsNetwork = new QAction( this, "SaveAsNetwork" );
		saveAsNetwork->addTo( file );
	    saveAsNetwork->setMenuText( tr( "&Save As" ) );
		saveAsNetwork->setAccel( tr( "CTRL+SHIFT+S" ) );
		connect( saveAsNetwork, SIGNAL( activated() ), this, SLOT( SaveAsNetwork() ) );

		file->insertSeparator();

		QAction* quitProgram = new QAction( this, "Quit" );
		quitProgram->addTo( file );
	    quitProgram->setMenuText( tr( "&Quit" ) );
		quitProgram->setAccel( tr( "CTRL+Q" ) );
		connect( quitProgram, SIGNAL( activated() ), qApp, SLOT( quit() ) );

///////// VIEW MENU /////////////////////////////////////////////
		QAction* viewProcessingMenu = new QAction( this, "ShowProcMenu" );
		viewProcessingMenu->addTo( view );
	    viewProcessingMenu->setMenuText( tr( "&Processing Menu" ) );
		viewProcessingMenu->setAccel( tr( "CTRL+M" ) );
		connect( viewProcessingMenu, SIGNAL( activated() ), this, SLOT( ShowProcMenu() ) );
		
///////// NETWORK ACTIONS MENU /////////////////////////////////////////////
		QAction* startNetwork = new QAction( this, "StartNetwork" );
		startNetwork->addTo( networkActions );
	    startNetwork->setMenuText( tr( "&Start Network" ) );
		startNetwork->setAccel( tr( "Space" ) );
		connect( startNetwork, SIGNAL( activated() ), this, SLOT( StartNetwork() ) );

		QAction* stopNetwork = new QAction( this, "StopNetwork" );
		stopNetwork->addTo( networkActions );
	    stopNetwork->setMenuText( tr( "&Stop Network" ) );
		stopNetwork->setAccel( tr( "Esc" ) );
		connect( stopNetwork, SIGNAL( activated() ), this, SLOT( StopNetwork() ) );

#if USE_OSCPACK		
		QAction* startOSCListening = new QAction( this, "StartOSCListening" );
		startOSCListening->addTo( networkActions );
	    startOSCListening->setMenuText( tr( "Start Listening OSC Packets" ) );
		startOSCListening->setAccel( tr( "O" ) );
		connect( startOSCListening, SIGNAL( activated() ), this, SLOT( StartOSCListening() ) );

		QAction* stopOSCListening = new QAction( this, "StopOSCListening" );
		stopOSCListening->addTo( networkActions );
	    stopOSCListening->setMenuText( tr( "Stop Listening OSC Packets" ) );
		stopOSCListening->setAccel( tr( "P" ) );
		connect( stopOSCListening, SIGNAL( activated() ), this, SLOT( StopOSCListening() ) );
#endif

/////////////////////////////////////////////////////////////////////////////

		mDockProcMenu = new QDockWindow( QDockWindow::InDock, this );
		mDockProcMenu->setResizeEnabled( true );
		mDockProcMenu->setCloseMode( QDockWindow::Always );
	
		addToolBar( mDockProcMenu, Qt::DockLeft );

		mDockProcMenu->setFixedExtentWidth( 160 );
		mDockProcMenu->setCaption( tr( "Processing menu" ) );
		mDockProcMenu->show();
		setDockEnabled( mDockProcMenu, Qt::DockTop, false );
		setDockEnabled( mDockProcMenu, Qt::DockBottom, false );

		mpAboutDlg = new AboutDialog( this, "NetworkEditor_AboutDialog", true );
		mpAboutDlg->versionInfo->setText(tr(
					"<p><b>Network Editor version %1</b></p>"
					"<p>Using CLAM version %2</p>"
					)
				.arg("0.2.1-CVS")
				.arg("0.91-CVS"));
		setIcon(*(mpAboutDlg->icon()));

		ProcessingTree * procTree = new ProcessingTree( mNetworkPresentation, mDockProcMenu );
		mDockProcMenu->setWidget( procTree );
		setActiveWindow();
		mNetworkPresentation.Show();
	}

	void MainWindow::ShowAboutDlg()
	{
		mpAboutDlg->show();
	}

	MainWindow::~MainWindow()
	{
	}

	void MainWindow::SendMessageToStatus( const std::string & message)
	{
		statusBar()->message( QString( message.c_str() ), 2000);
	}

	Qt_NetworkPresentation & MainWindow::GetNetworkPresentation()
	{
		return mNetworkPresentation;
	}

	void MainWindow::NewNetwork()
	{
		SignalNewNetworkSignal.Emit();
	}

	void MainWindow::LoadNetwork()
	{
		QString s = QFileDialog::getOpenFileName(
			"",
			"XML Files (*.xml)",
			this,
			"open file dialog"
			"Choose a file to load network" );

		if (s==QString::null)
			return;
		
		mNetworkPresentation.GetNetworkController().LoadNetworkFrom(std::string(s.ascii()) );
		mCurrentNetwork = s;
	}


	void MainWindow::SaveAsNetwork()
	{
		QString s = QFileDialog::getSaveFileName(
			mCurrentNetwork,
			"XML Files (*.xml)",
			this,
			"save file dialog"
			"Choose a file to save network" );

		if (s==QString::null) 
			return;

		mCurrentNetwork = s;
		SaveNetwork();	
	}

	void MainWindow::SaveNetwork()
	{
		if (mCurrentNetwork==QString::null)
			return;
		std::string xmlfile = mCurrentNetwork.ascii();
		mNetworkPresentation.GetNetworkController().SaveNetworkTo(xmlfile);
	}

	void MainWindow::StartNetwork()
	{
		SignalStartNetwork.Emit();
	}

	void MainWindow::StopNetwork()
	{
		SignalStopNetwork.Emit();
	}

	void MainWindow::StartOSCListening()
	{
		SignalChangeOSCState.Emit(true);
	}

	void MainWindow::StopOSCListening()
	{
		SignalChangeOSCState.Emit(false);
	}

	void MainWindow::ShowProcMenu()
	{
		mDockProcMenu->show();
	}

} // namespace NetworkGUI
