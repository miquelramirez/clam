/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

