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

#ifndef __QTSPECTRUMPRESENTATION__
#define __QTSPECTRUMPRESENTATION__

#include "Presentation.hxx"
#include "ViewConfiguration.hxx"
#include "SpectrumGView.hxx"
#include "CLAMGL.hxx"
#include "CBL.hxx"
#include "Viewport.hxx"
#include "GeometryKit.hxx"
#include "GLState.hxx"
#include "GLLinearSpRenderer.hxx"

using CLAM::DataArray;
using CLAM::TData;
using CLAM::EScale;


namespace CLAMGUI
{

class QtGLPort;
class QtDataDisplay;

class SpecGLSetup : public GLState
{
public:

	void Apply()
	{
		glClearColor( 0.0, 0.0, 0.0, 0.0 );
		glShadeModel( GL_FLAT );
	}
};

class QtSpectrumPresentation : public Presentation
{
public:
	QtSpectrumPresentation( const char* label = 0 )
		: Presentation(), mSpectralMetrixLocked( false )
	{
		Geometry geo( 100, 100, 800, 600 );
		Init( geo, label );
	}

	QtSpectrumPresentation( const Geometry& g, const char* label = 0 )
		: Presentation(), mSpectralMetrixLocked( false )
	{
		Init( g, label );
	}

	virtual ~QtSpectrumPresentation()
	{
	}

	void PublishCallbacks();

	void Show();

protected:

	virtual void Init( const Geometry& g, const char* label );
	virtual void UpdateMagData( const DataArray& array );
	virtual void UpdatePhaseData( const DataArray& array );
	void UpdateScale( unsigned int nbins, EScale scale );
	void UpdateSpectralRangeData( TData value );
	void UpdateSpectralResolutionData( TData value );

protected:

	CBL::Functor1< const DataArray& >                mGetMag;
	CBL::Functor1< const DataArray& >                mGetPhase;
	CBL::Functor1< TData >                           mGetRange;
	CBL::Functor1< TData >                           mGetResolution;
	CBL::Functor2< unsigned int, EScale >            mGetScale;

	GLLinearSpRenderer*                              mRenderer;
	QtDataDisplay*                                   display;
	QtGLPort*                                        mPort;

	Viewport                                         mSpecPresMetrix;
	bool                                             mSpectralMetrixLocked;
};

}

#endif // QtSpectrumPresentation.hxx

