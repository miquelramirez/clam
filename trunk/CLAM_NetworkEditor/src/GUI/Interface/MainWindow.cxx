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
#include <qstatusbar.h>
#include <qtoolbar.h>
#include <qbutton.h>
#include <qlayout.h> 
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qfiledialog.h>
#include <qapplication.h>
#include <string>
#include "aboutdialog.h"

#include <iostream> // TODO: remove

namespace NetworkGUI
{

	MainWindow::MainWindow()
		: mNetwork(this),
		  mDockProcMenu(0),
		  QMainWindow( 0, "", WGroupLeader )
	{
		setCentralWidget( &mNetwork );
		setCaption( "CLAM Network Editor" );
		resize( 800, 600 );
	
		SlotSendMessageToStatus.Wrap( this, &MainWindow::SendMessageToStatus );
		mNetwork.SignalSendMessageToStatus.Connect( SlotSendMessageToStatus );
		SignalNewNetworkSignal.Connect( mNetwork.SlotClear );
		statusBar()->message( "Ready to edit" );

		SignalChangeNetworkState.Connect( mNetwork.SlotChangeState );


		QPopupMenu * file = new QPopupMenu( this );
		QPopupMenu * view = new QPopupMenu( this );
		QPopupMenu * networkActions = new QPopupMenu( this );
		QPopupMenu * outControlActions = new QPopupMenu( this );
		menuBar()->insertItem( "File", file );
		menuBar()->insertItem( "View", view );
		menuBar()->insertItem( "Network Actions", networkActions );
		menuBar()->insertItem( "About", this, SLOT(ShowAboutDlg()));
		setCentralWidget(&mNetwork);
 
		file->insertItem("New", this, SLOT(NewNetwork()));
		file->insertItem("Load", this, SLOT(LoadNetwork()));
		file->insertItem("Save", this, SLOT(SaveNetwork()));
		file->insertItem("Save As", this, SLOT(SaveAsNetwork()));
		file->insertSeparator();
		file->insertItem("Exit", qApp, SLOT(quit()));

		view->insertItem("Processing Menu", this, SLOT(ShowProcMenu()));

		networkActions->insertItem("Start", this, SLOT(StartNetwork()));
		networkActions->insertItem("Stop", this, SLOT(StopNetwork()));	

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

		ProcessingTree * procTree = new ProcessingTree( mNetwork, mDockProcMenu );
		mDockProcMenu->setWidget( procTree );
		setActiveWindow();
		mNetwork.Show();
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
		return mNetwork;
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

//	SignalNewNetworkSignal.Emit();
		if (s!=QString::null)
			mNetwork.SignalLoadNetworkFrom.Emit(std::string(s.ascii()));
	}


	void MainWindow::SaveNetwork()
	{
		QString s = QFileDialog::getSaveFileName(
			"",
			"XML Files (*.xml)",
			this,
			"save file dialog"
			"Choose a file to save network" );

		if (s!=QString::null)
		{
			//TODO code for saving positions
			//  call method to mNetwork : SaveNetwork passing filename
			//  QtNetworkPresentation::SaveNetwork (slot) :
			//     execute Signal SaveNetworkTo passing ref to stream
			//     add elem with positions: (name, xpos, ypos) tuples
			//     save doc
			mNetwork.SignalSaveNetworkTo.Emit(std::string(s.ascii()));
		}
	}

	void MainWindow::SaveAsNetwork()
	{
		SaveNetwork();
	}

	void MainWindow::StartNetwork()
	{
		SignalChangeNetworkState.Emit(true);
	}

	void MainWindow::StopNetwork()
	{
		SignalChangeNetworkState.Emit(false);
	}

	void MainWindow::ShowProcMenu()
	{
		mDockProcMenu->show();
	}

} // namespace NetworkGUI
