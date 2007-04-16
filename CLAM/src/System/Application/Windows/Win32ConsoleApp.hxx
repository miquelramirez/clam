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

#ifndef __WIN32CONSOLEAPP__
#define __WIN32CONSOLEAPP__

#ifndef WIN32
#error "Hey you, Linux User! You shouldn't be including this thing"
#endif

#include "BaseAudioApplication.hxx"
#include "CLAM_windows.h"
#undef GetClassName
#undef CreateWindow

namespace CLAM
{

#ifdef WIN32

	// :MRJ:TODO: I don't know why DirectXAudioDevice class hasn't the static pointer already defined
	// instead of relying that responsability to such a concrete class as DXFullDuplex. While I haven't
	// got some time to rework the thing we will need the following ugly kludge...

	class DXFullDuplex;

	template <typename DXDeviceType>
	void DXHook( DXDeviceType* homemade_tmpl_func_overload, void* wndhndler )
	{
		DXDeviceType::shMainWnd = (HWND)wndhndler;
	}

#define DXFullDuplexHook( handler ) DXHook( (DXFullDuplex*)NULL, handler ) 

#endif


class Win32ConsoleApp 
: public BaseAudioApplication
{
public:

	Win32ConsoleApp()
	{
	}

	virtual ~Win32ConsoleApp()
	{
	}


	void Run( int argc = 0, char** argv = 0 );
	virtual void UserMain(void);
	virtual void AudioMain(void) = 0;
	static HWND GetHandle( void );	

private:

	virtual void AppCleanup(void)
	{
	}
	
};



	
}

#endif // Win32ConsoleApp.hxx

