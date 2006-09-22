#include "MainWindow.hxx"


MainWindow::~MainWindow()
{
	if ( _networkPlayer) delete _networkPlayer;
}

