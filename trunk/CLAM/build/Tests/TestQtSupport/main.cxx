#include "Assert.hxx"
#include <qapplication.h>
#include <stdio.h>
#include "MainWindow.hxx"
#include "ControlPanel.h"



int main( int argc, char** argv )
{
	QApplication dummy(argc, argv);
	ControlPanel panel;
	

	dummy.setMainWidget(&panel);
	
	NetworkGUI::MainWindow* widget = new NetworkGUI::MainWindow( );
	CLAM_ASSERT( widget != NULL, "Could not create widget!" );
	panel.show();
	return dummy.exec();

}

