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
#include <iostream>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qfiledialog.h>
#include <qapplication.h>
#include <string>

namespace NetworkGUI
{

MainWindow::MainWindow()
	: mNetwork(this),
	  mDockProcMenu(0),
	  QMainWindow( 0, "", WGroupLeader )
{
	mNetwork.setFocus();

	setCentralWidget( &mNetwork );
	setCaption( "CLAM Network Editor" );
	resize( 800, 600 );
	
	NewMessageToStatus.Wrap( this, &MainWindow::OnNewMessageToStatus );
	mNetwork.SendNewMessageToStatus.Connect( NewMessageToStatus );
	NewNetworkSignal.Connect( mNetwork.Clear );
	statusBar()->message( "Ready to edit" );

	ChangeNetworkState.Connect( mNetwork.ChangeState );


	QPopupMenu * file = new QPopupMenu( this );
	QPopupMenu * view = new QPopupMenu( this );
	QPopupMenu * networkActions = new QPopupMenu( this );
        menuBar()->insertItem( "File", file );
        menuBar()->insertItem( "View", view );
	menuBar()->insertItem( "Network Actions", networkActions );
        menuBar()->insertItem( "About", this, SLOT(About()));

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

	ProcessingTree * procTree = new ProcessingTree( mNetwork, mDockProcMenu );
	mDockProcMenu->setWidget( procTree );
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnNewMessageToStatus( const std::string & message)
{
	statusBar()->message( QString( message.c_str() ), 2000);
}

Qt_NetworkPresentation & MainWindow::GetNetworkPresentation()
{
	return mNetwork;
}

void MainWindow::Start()
{
	std::cout << "starting network" << std::endl;
}

void MainWindow::Stop()
{
	std::cout << "stopping network" << std::endl;
}

void MainWindow::NewNetwork()
{
	std::cout << "new network" << std::endl;
	NewNetworkSignal.Emit();
}

void MainWindow::LoadNetwork()
{
/*
	QFileDialog* fd = new QFileDialog( this, "file dialog", TRUE );
	fd->setMode( QFileDialog::AnyFile);
	fd->addFilter( "XML Files (*.xml)" );
	fd->show();
*/

	QString s = QFileDialog::getOpenFileName(
		"",
		"XML Files (*.xml)",
		this,
		"open file dialog"
		"Choose a file to load network" );

	NewNetworkSignal.Emit();
	if (s!=QString::null)
		mNetwork.LoadNetworkFrom.Emit(std::string(s.ascii()));
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
		mNetwork.SaveNetworkTo.Emit(std::string(s.ascii()));
}

void MainWindow::SaveAsNetwork()
{
	std::cout << "saving network as" << std::endl;
	SaveNetwork();
}

void MainWindow::StartNetwork()
{
	ChangeNetworkState.Emit(true);
	
		
}

void MainWindow::StopNetwork()
{
	ChangeNetworkState.Emit(false);
}

void MainWindow::ShowProcMenu()
{
	mDockProcMenu->show();
}

} // namespace NetworkGUI
