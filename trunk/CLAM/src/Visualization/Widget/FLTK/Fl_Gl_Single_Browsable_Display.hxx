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

#ifndef __FL_GL_SINGLE_BROWSABLE_DISPLAY__
#define __FL_GL_SINGLE_BROWSABLE_DISPLAY__

#include "Fl_Gl_Single_Display.hxx"
#include "GLRenderingManager.hxx"
#include "DataTypes.hxx"
#include "Slotv1.hxx"
#include "Signalv1.hxx"


namespace CLAMVM
{
	using SigSlot::Signalv1;
	using SigSlot::Slotv1;

	class Fl_Gl_Single_Browsable_Display : public Fl_Gl_Single_Display
	{
	public:
		Fl_Gl_Single_Browsable_Display( int X, int Y, int W, int H, const char* label = 0);

		~Fl_Gl_Single_Browsable_Display();

		void draw_overlay(  );
		
 		int handle( int event ); // Fl_Gl_Window required interface

		void SetPainting(  );
		void UnsetPainting(  );
		void SetPos( CLAM::TData pos );

		Signalv1<double> mMouseEvent;
		Slotv1<CLAM::TData> mNewFrame;

	protected:
		void DrawContents();

		bool mPainting;
		double mPos;

	private:
		GLRenderingManager* mRenderer;
	};
}

#endif // Fl_Gl_Single_Display.hxx
