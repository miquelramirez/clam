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

#ifndef __OSCILLATORPRESENTATION__
#define __OSCILLATORPRESENTATION__

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include "FLKnob.hxx"
#include "CBL.hxx"
#include "ProcObjPresentation.hxx"
#include "DataTypes.hxx"
#include "GeometryKit.hxx"
using CLAM::TData;

namespace CLAM
{
	class Oscillator;
}

using CLAM::Oscillator;

namespace CLAMGUI
{

	template <>
	class ProcObjPresentation<Oscillator>
		: public Presentation
	{
		//Callbacks
		static void callback(Fl_Widget* source, void* module);
		
	public:
		ProcObjPresentation( const char* label = 0)
			: Presentation()
			{
				Geometry geo( 100, 100, 300, 200 );
				Init( geo, label );
			}
		ProcObjPresentation( const Geometry& g, const char* label = 0 )
			: Presentation()
			{
				Init( g, label );
			}
		~ProcObjPresentation()
			{
				// Free the windows
				delete mWindow;
			}

		void PublishCallbacks();
		
		void Show();
		
	protected:
		
		
		void Init( const Geometry& g, const char* l );
		
		void GetFreq( TData freq );
		
		void GetAmp( TData mag );
		
		void GetModIndex( TData modidx );
		
		void GetPhase( TData phase );
		
		TData SendFreq();
		
		TData SendAmp();

		TData SendModIndex();

		TData SendPhase();

	private:

		CBL::Functor1< TData >    mDrawFreq;
		CBL::Functor1< TData >    mDrawAmp;
		CBL::Functor1< TData >    mDrawModIndex;
		CBL::Functor1< TData >    mDrawPhase;
		CBL::Functor0R< TData >   mSendFreq;
		CBL::Functor0R< TData >   mSendAmp;
		CBL::Functor0R< TData >   mSendModIndex;
		CBL::Functor0R< TData >   mSendPhase;

		Fl_Window*                mWindow;
		Fl_Knob*                  mFreqKnob;
		Fl_Knob*                  mAmpKnob;
		Fl_Knob*                  mModIdxKnob;
		Fl_Knob*                  mPhaseKnob;

	};

}

#endif // OscillatorPresentation.hxx
