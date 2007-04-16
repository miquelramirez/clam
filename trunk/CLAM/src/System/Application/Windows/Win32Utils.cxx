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

#include "Win32Utils.hxx"
#include "CLAM_windows.h"
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

