/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __GUIAudioApplication__
#define __GUIAudioApplication__

#include "BaseAudioApplication.hxx"
#include <FL/Fl_Button.H>

namespace CLAM {


#ifdef WIN32
	#undef CreateWindow

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

	class GUIAudioApplication:public BaseAudioApplication
	{
	private:
		static void SAudioStartStop(
			Fl_Button* b,BaseAudioApplication *pThis);
	public:
		GUIAudioApplication();
		void Run(int argc,char** argv);
	protected:
		virtual Fl_Window* CreateWindow(int argc,char** argv);
		virtual void UserMain(void);
		virtual void AudioMain(void) = 0;
		virtual void AppCleanup(void){};
	};

} // namespace CLAM

#endif
