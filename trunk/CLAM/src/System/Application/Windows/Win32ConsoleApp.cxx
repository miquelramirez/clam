#include "Win32ConsoleApp.hxx"
#include "DXFullDuplex.hxx"
using namespace CLAM;

#ifndef WIN32
#error "Hey Linux User! You shouldn't be compiling this"
#endif

void Win32ConsoleApp::Run( int argc, char** argv )
{
	if ( DXFullDuplex::shMainWnd == 0 )
		{
			DXFullDuplexHook( Win32ConsoleApp::GetHandle() );
		}

	Start();
	UserMain();
	Stop();

}

HWND Win32ConsoleApp::GetHandle( void )
{

	HWND console_hwnd = 0;

	const int console_title_size = 1024;

	char consoleTitle[ console_title_size ];

	GetConsoleTitle( consoleTitle, console_title_size );

	console_hwnd = FindWindow( NULL, consoleTitle );

	return console_hwnd;

}

void Win32ConsoleApp::UserMain()
{

	while(1)
		{
			Sleep(10);
		}
}
