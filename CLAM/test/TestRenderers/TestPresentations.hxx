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

#ifndef __TESTPRESENTATIONS__
#define __TESTPRESENTATIONS__

#include "Presentation.hxx"
#include "GLState.hxx"
#include "Array.hxx"
#include "CBL.hxx"
#include "GeometryKit.hxx"

#ifdef WIN32
#include "CLAM_windows.h"
#endif
#include <GL/gl.h>

class Fl_Double_Window; // Forward declaration

using CLAM::DataArray;
using CLAM::GLState;
using CLAMGUI::Presentation;
using CLAMGUI::Geometry;

// Forward declarations
namespace CLAMGUI
{
	class GLPort;
	class GLPhaseSpRenderer;
	class GLLinearSpRenderer;
	class GLLogSpRenderer;
	class GLLogPowSpRenderer;
	class View;
}

using CLAMGUI::GLPort;
using CLAMGUI::GLPhaseSpRenderer;
using CLAMGUI::GLLinearSpRenderer;
using CLAMGUI::GLLogSpRenderer;
using CLAMGUI::GLLogPowSpRenderer;
using CLAMGUI::View;

namespace CLAM
{
	class FLDisplayContainer;
}

using CLAM::FLDisplayContainer;
using CLAM::TData;

namespace CLAMGUITest
{

	class GenericGLState
    : public GLState
	{
	public:
		
		void Apply()
			{
				glClearColor( 0.0, 0.0, 0.0, 0.0 );
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_CULL_FACE);
				glShadeModel( GL_FLAT );
			}
	};
	
	class TestPhasePresentation
		: public Presentation
	{
	public:
		
		TestPhasePresentation ( )
			: Presentation()
			{
				Geometry g( 100, 100, 640, 400 );
				Init( g );
			}

		TestPhasePresentation( const Geometry& g ) 
			: Presentation()
			{
				Init( g );
			}
		
		virtual ~TestPhasePresentation();
		
		void LinkWithView( View* view );
		
		void PublishCallbacks();

		void Show();
		
		void Do();
	protected:
		
		virtual void Init( const Geometry& g );
		
		virtual void UpdatePhaseData( const DataArray& array );
		
		virtual void UpdateSpectralRangeData( TData value );
		
	protected:
		
		CBL::Functor1< const DataArray& > mPhaseDataCb;
		CBL::Functor1< TData >            mRangeCb;
		
		GLPort*                           mPort;
		GLPhaseSpRenderer*                  mRenderer;
		FLDisplayContainer*               mDispContainer;
		Fl_Double_Window*                 mWindow;
		
	};
	
	class TestLinearMagPresentation
		: public Presentation
	{
	public:

		TestLinearMagPresentation ( )
			: Presentation()
			{
				Geometry g( 100, 100, 640, 400 );
				Init( g );
			}
		
		TestLinearMagPresentation( const Geometry& g )
			: Presentation()
			{
				Init ( g );
			}
		
		virtual ~TestLinearMagPresentation();
		
		void LinkWithView( View* view );
		
		void PublishCallbacks();
		
		void Show();
		
		void Do();
		
	protected:
		
		virtual void Init(  const Geometry& g );
		
		virtual void UpdateMagData( const DataArray& array );
		
		virtual void UpdateSpectralRangeData( TData value );
		
	protected:
		
		CBL::Functor1< const DataArray& > mMagDataCb;
		CBL::Functor1< TData >            mRangeCb;
		
		GLPort*                           mPort;
		GLLinearSpRenderer*                  mRenderer;
		FLDisplayContainer*               mDispContainer;
		Fl_Double_Window*                 mWindow;
		
	};

	class TestLogMagPresentation
		: public Presentation
	{
	public:
		
		TestLogMagPresentation ( )
			: Presentation()
			{
				Geometry g( 100, 100, 640, 400 );
				Init( g );
			}
		
		TestLogMagPresentation( const Geometry& g )
			: Presentation()
			{
				Init( g );
			}
		
		virtual ~TestLogMagPresentation();
		
		void LinkWithView( View* view );
		
		void PublishCallbacks();
		
		void Show();
		
		void Do();
		
	protected:
		
		virtual void Init(  const Geometry& g );
		
		virtual void UpdateMagData( const DataArray& array );
		
		virtual void UpdateSpectralRangeData( TData value );
		
	protected:
		
		CBL::Functor1< const DataArray& > mMagDataCb;
		CBL::Functor1< TData >            mRangeCb;

		GLPort*                           mPort;
		GLLogSpRenderer*                  mRenderer;
		FLDisplayContainer*               mDispContainer;
		Fl_Double_Window*                 mWindow;
		
	};
	
	class TestLogPowPresentation
		: public Presentation
	{
	public:
		
		TestLogPowPresentation ( )
			: Presentation()
			{
				Geometry g( 100, 100, 640, 400 );
				Init( g );
			}

		TestLogPowPresentation( const Geometry& g) 
			: Presentation()
			{
				Init( g );
			}
		
		virtual ~TestLogPowPresentation();

		void LinkWithView( View* view );
		
		void PublishCallbacks();
		
		void Show();
		
		void Do();
		
	protected:
		
		virtual void Init(  const Geometry& g );
		
		virtual void UpdateMagData( const DataArray& array );
		
		virtual void UpdateSpectralRangeData( TData value );
		
	protected:
		
		CBL::Functor1< const DataArray& > mMagDataCb;
		CBL::Functor1< TData >            mRangeCb;
		
		GLPort*                           mPort;
		GLLogPowSpRenderer*               mRenderer;
		FLDisplayContainer*               mDispContainer;
		Fl_Double_Window*                 mWindow;
		
	};
	
}


#endif // TestPresentations.hxx
