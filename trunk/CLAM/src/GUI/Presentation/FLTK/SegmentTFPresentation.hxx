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

#ifndef __SEGMENTTFPRESENTATION__
#define __SEGMENTTFPRESENTATION__

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include "FLDisplayContainer.hxx"
#include "GLState.hxx"
#include "CBL.hxx"
#include "ViewConfiguration.hxx"
#include "Presentation.hxx"
#include "DataTypes.hxx"
#include "SegmentView.hxx"
#include "GLPortNew.hxx"
#include "GLTF2DRenderer.hxx"
#include "Array.hxx"
#include "GeometryKit.hxx"
using CLAM::DataArray;
using CLAM::TData;
using CLAM::TTime;


namespace CLAMGUI
{

	class SegmentTFGLState 
		: public GLState
	{
	public:

		void Apply()
		{
		glClearColor( 0.0, 0.0, 0.0, 0.0 );
		glShadeModel( GL_SMOOTH );
		glEnable(GL_DEPTH_TEST);

		}
	};

	class SegmentTFPresentation
		: public Presentation
	{
	public:

		SegmentTFPresentation( const char* label = 0 )
			: Presentation(), mGLContextAvailable( false )
		{
			Geometry g ( 100, 100, 640, 480 );

			Init ( g, label );
		}

		SegmentTFPresentation( const Geometry& g, const char* label )
			: Presentation(), mGLContextAvailable( false )
		{
			Init( g, label );
		}

		~SegmentTFPresentation()
		{
			// Freeing the windows and that

			delete mWindow;
			delete mRenderer;
		}

		void PublishCallbacks();

		void Show();

		inline void Do()
		{
			mPort->redraw();
		}

	protected:

		void Init()
		{
		}

		void Init( const Geometry& g, const char* label  );

		void CatchThoseFrames( const DataArray& array, bool isLinear );
		void CatchParams( TTime btime, TTime etime, TData specrng, unsigned int nframes );

	private:

		CBL::Functor2< const DataArray&, bool >             mDrawFrame;
		CBL::Functor4< TTime, TTime, TData, unsigned int >  mCatchParams;

		GLPort*                                             mPort;
		FLDisplayContainer*                                 mDispContainer;
		GLTF2DRenderer*                                     mRenderer;
		Fl_Double_Window*                                   mWindow;
		bool                                                mGLContextAvailable;
	};

}

#endif // SegmentTFPresentation.hxx
