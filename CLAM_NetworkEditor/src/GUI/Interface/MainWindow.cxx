
#include "MainWindow.hxx"
#include <qstatusbar.h>
#include <qtoolbar.h>
#include <qbutton.h>
#include <qlayout.h> 
#include <iostream>

namespace NetworkGUI
{

MainWindow::MainWindow()
	: mNetwork(this),
	  mFactory( this , "factory" ),
	  mNetworkActions( this )
{
	mNetwork.setFocus();	
	mFactory.move(640, 10);
	mNetworkActions.move(640, 300);

	setCentralWidget( &mNetwork );
	setCaption( "CLAM Network Editor" );
	resize( 800, 600 );

	NewMessageToStatus.Wrap( this, &MainWindow::OnNewMessageToStatus );
	mNetwork.SendNewMessageToStatus.Connect( NewMessageToStatus );
	statusBar()->message( "Ready to edit" );

	mFactory.AddNewProcessing.Connect( mNetwork.AddNewProcessing );
	mNetworkActions.ChangeNetworkState.Connect( mNetwork.ChangeState );

}

void MainWindow::OnNewMessageToStatus( const std::string & message)
{
	statusBar()->message( QString( message.c_str() ), 2000);
}

MainWindow::~MainWindow()
{
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

} // namespace NetworkGUI
