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

#ifndef __SPECTRUMPRESENTATION__
#define __SPECTRUMPRESENTATION__

#include "ProcDataPresentation.hxx"
#include "FLDisplayContainer.hxx"
#include <FL/Fl_Double_Window.H>
#include "GLPortNew.hxx"
#include "GLLinearSpRenderer.hxx"
#include "GLState.hxx"
#include "SpectrumGView.hxx"
#include "ViewConfiguration.hxx"
#include "CLAMGL.hxx"
#include "CBL.hxx"
#include "GeometryKit.hxx"
#include "Viewport.hxx"
#include "GlobalEnums.hxx"


using CLAM::DataArray;
using CLAM::TData;
using CLAM::EScale;

namespace CLAMGUI
{

	class SpecPresGLState : public GLState
	{
	public:
		
		
		void Apply()
			{
				glClearColor( 0.0, 0.0, 0.0, 0.0 );
				glShadeModel( GL_FLAT );
			}
	};

	template<>
	class ProcDataPresentation< Spectrum >
		: public Presentation
	{
	public:
		
	ProcDataPresentation( const char* label = 0)
		: Presentation(), mSpectralMetrixLocked( false )
	{
		Geometry geo ( 100, 100, 800, 600 );
		Init( geo, label );
	}
	

	ProcDataPresentation( const Geometry& g, const char* label = 0)
		: Presentation(), mSpectralMetrixLocked( false )
	{
		Init( g, label );
	}
	
	virtual ~ProcDataPresentation()
	{
		delete mWindow;
		delete mRenderer;
	}	
	
	void PublishCallbacks();
	
	
	void Show();
	
	
	void Do()
	{
		mDispContainer->redraw();
	}
	

protected:
	
	
	virtual void Init( const Geometry& g, const char* label );
	virtual void UpdateMagData( const DataArray& array );
	virtual void UpdatePhaseData( const DataArray& array );
	void UpdateScale( unsigned int nbins, EScale scale );
	void UpdateSpectralRangeData( TData value );
	
	void UpdateSpectralResolutionData( TData value );
	
protected:
	
	
	CBL::Functor1< const DataArray& >               mDrawMagCb;
	CBL::Functor1< const DataArray& >               mDrawPhaseCb;
	CBL::Functor1< TData >                          mDrawRangeCb;
	CBL::Functor1< TData >                          mDrawResolutionCb;
	CBL::Functor2< unsigned int, EScale >           mDrawScaleCb;
	
	GLPort*                           mPort;
	GLLinearSpRenderer*               mRenderer;
	FLDisplayContainer*               mDispContainer;
	Fl_Double_Window*                 mWindow;

	Viewport                          mSpecPresMetrix;
	bool                              mSpectralMetrixLocked;
	
};

}

#endif // SpectrumPresentation.hxx
