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

#ifndef __AUDIOBROWSER__
#define __AUDIOBROWSER__

#include "Presentation.hxx"
#include <FL/Fl_Group.H>

#include "FLDisplayContainer.hxx"
#include "GLPortSigSlot.hxx"
#include "GLWaveRenderer.hxx"
#include "GLState.hxx"
#include "AudioGView.hxx"
#include "ViewConfiguration.hxx"
#include "CLAMGL.hxx"
#include "CBL.hxx"
#include "GeometryKit.hxx"
#include "AudioBrowserWindow.hxx" 
#include "AudioPresentation.hxx"

using CLAM::DataArray;
using CLAM::TData;

namespace CLAM
{
	class Audio;
}
using CLAM::Audio;


namespace CLAMGUI
{
	
	class AudioBrowser
		: public Presentation
	{
	public:
		
		AudioBrowser( int X, int Y, int W, int H, const char* l = 0 );

		AudioBrowser( const Geometry& g, const char* label = 0 );
		
		~AudioBrowser();
		
		virtual void PublishCallbacks();

		virtual void Show();
		
		void Do() { mGLsurface->redraw(); }

		Fl_Window* GetWindow() { return mWindow; }
		
		void setPainting(bool painting) { mGLsurface->setPainting(painting); }
		Slotv1<CLAM::TData>* GetFrameSlot() { return mGLsurface->getFrameSlot(); } 
		Slotv1<bool>* GetPaintSlot() { return mGLsurface->getPaintSlot(); } 
		Signalv1<double>* GetSignal() { return mGLsurface->getSignal(); } 
		
	protected:
		
		void Init(const Geometry& g, const char* label );

		void UpdateBufferData( const DataArray& array );
		
		void UpdateSampleRateData( TData value );
		
		void UpdateDurationData( TData value );

	private:

		CBL::Functor1< const DataArray& >   mDrawBufferCb;
		CBL::Functor1< int >                mDrawSampleRateCb;
		CBL::Functor1< TData >              mDrawDurationCb;

		GLWaveRenderer*                     mRenderer;
		FLDisplayContainer*                 mDispContainer;
		PresentationWindow*                 mWindow;
		
		GLPortSigSlot*          mGLsurface;
		FLDisplayContainer*     mContainer;

};

}

#endif // AudioBrowser.hxx
