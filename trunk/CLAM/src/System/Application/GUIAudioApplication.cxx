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


#include "GUIAudioApplication.hxx"

#include <FL/Fl_Window.H>
#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Light_Button.H>
#ifdef WIN32
#include "DXFullDuplex.hxx"
#endif

#ifdef CreateWindow
	#undef CreateWindow
#endif

namespace CLAM {

	GUIAudioApplication::GUIAudioApplication():BaseAudioApplication()
	{
	}

	void GUIAudioApplication::SAudioStartStop(
		Fl_Button* b,BaseAudioApplication *pThis)
	{
		GUIAudioApplication* tmp = (GUIAudioApplication*) pThis;
		if (b->value())
			tmp->Start();
		else
			tmp->Stop();
	}

	Fl_Window* GUIAudioApplication::CreateWindow(int argc,char** argv)
	{
		Fl_Window* w = 0;
		w = new Fl_Window(200,30);
		{
			Fl_Button* o = new Fl_Light_Button(3,3,60,18,"Audio");
			o->type(FL_TOGGLE_BUTTON);
			o->labelsize(12);
			o->box(FL_THIN_UP_BOX);
			o->callback((Fl_Callback*)SAudioStartStop,this);
		}
		{
			Fl_Box* b = new Fl_Box(65,3,130,26,
					       "CLAM Audio I/O Control\nPress button to start/stop");
			b->labelsize(11);
			b->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
		}
		w->set_non_modal();

		return w;
	}

	void GUIAudioApplication::Run(int argc,char** argv)
	{
#ifdef WIN32
		if (DXFullDuplex::shMainWnd == 0) {
#endif
			Fl_Window* w = CreateWindow(argc,argv);
			w->show(argc,argv);
#ifdef WIN32
			DXFullDuplexHook( fl_xid(w) );
		}
#endif
		UserMain();	
	}
	
	void GUIAudioApplication::UserMain(void)
	{
		Fl::run();
	}

} // namespace CLAM
