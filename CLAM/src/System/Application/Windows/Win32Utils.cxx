#include "Win32Utils.hxx"
#include <windows.h>
#undef GetClassName
#undef CreateWindow
#include "DXFullDuplex.hxx"

namespace CLAM
{

template <typename DXDeviceType>
void DXHook( void* wndhndler, DXDeviceType* homemade_tmpl_func_overload = 0 )
{
	DXDeviceType::shMainWnd = (HWND)wndhndler;
}

static HWND GetConsoleHandle()	
{
	HWND console_hwnd = 0;

	// MRJ: I haven't got the slightest idea which is the
	// maximum length for a Windoze console title
	const int console_title_size = 8192;

	char consoleTitle[ console_title_size ];

	GetConsoleTitle( consoleTitle, console_title_size );

	console_hwnd = FindWindow( NULL, consoleTitle );

	return console_hwnd;
	
}

void EnableConsoleAudio()
{
	DXHook<DXFullDuplex>( GetConsoleHandle() );
}

}
