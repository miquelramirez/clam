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

#ifndef _FLDisplayArray_
#define _FLDisplayArray_

#include "FLDisplay.hxx"
#include "Array.hxx"
#include "Decorator.hxx"
#include "FLVertLine.hxx"
//#include "FLPoint.hxx"
#include "GeometryKit.hxx"
#include <FL/fl_draw.H>
#include <list>
#include <vector>
#include <iostream>


namespace CLAMGUI

{

template <class T, class DecoratorType = Decorator< std::vector<int> > > class FLDisplayArray:public FLDisplay
{
private:

	typedef std::list< CLAM::Array<T>* > ArrList;
	typedef std::list< Fl_Color > ColorList;
	
	ArrList                       mRendereable;
	ColorList                     mColorList;
	DecoratorType                 mDispDecor;
	FLVertLine                    mDecPol;

public:

	FLDisplayArray()
		: FLDisplay( 0, 0, 0, 0 )
	{

	}

	FLDisplayArray( CLAM::Array<T> *pArray )
		: FLDisplay(0,0,0,0)
	{
		mRendereable.push_back( pArray );
		mColorList.push_back( FL_GREEN );
	}

	FLDisplayArray( CLAM::Array<T>* pArray, const CLAM::Array<int>& mask, Fl_Color mask_color )
		: FLDisplay(0,0,0,0)
	{
		mRendereable.push_back( pArray );
		mColorList.push_back( FL_GREEN );

		mDispDecor.SetUp( mask.GetPtr(), mask.GetPtr()+mask.Size() );
		mDecPol.myColor = mask_color;
	}
	
	FLDisplayArray( int x, int y, int w, int h, CLAM::Array<T> *pArray)
		: FLDisplay( x, y, w, h)
	{
		mRendereable.push_back( pArray );
		mColorList.push_back( FL_GREEN );
	}

	void AddArray( CLAM::Array<T> *pArray, Fl_Color color )
	{
		mRendereable.push_back( pArray );
		mColorList.push_back( color );
	}

	void DrawSelf( void )
	{
		ArrList::iterator i;
		ColorList::iterator j;

		i = mRendereable.begin();
		j = mColorList.begin();

		while ( i != mRendereable.end() && j != mColorList.end() )
		{
			DrawSelf( (*i), (*j) );
			i++;
			j++;
		}
	}

	void DrawSelf(CLAM::Array<T>* mpArray, Fl_Color color)
	{
		Recti viewport( x(), y(), w(), h() );
		double dx = w()/mHorRange.mSize;
		double scaley = h()/mVerRange.mSize;
		double fx;
		int i,cx,cy,px,py;	
		
		mDispDecor.SetViewport( viewport );
		fl_color(color);

		i = int(mHorRange.mPosition);
		
		fx = x() + (static_cast<double>(i)-mHorRange.mPosition)*dx;
		py = static_cast<int>(y()-( static_cast<double>((*mpArray)[i])-mVerRange.mPosition) * scaley);
		cx = static_cast<int>(fx);
		px = cx;

		

		while ( cx <= x()+w()+dx && i < mHorRange.mTotal )
		{
			if ( i < (*mpArray).Size() )
			{
				cy = static_cast<int>( y() - (static_cast<double>((*mpArray)[i])-mVerRange.mPosition )* scaley);
			}


			fl_line( px, py, cx, cy );

			if ( mDispDecor( i, cx, mDecPol ) )
			{
			}
			

			i++;

			px = cx;
			py = cy;

			fx+=dx;
			cx = static_cast<int>(fx);
		}
	}

};

}

#endif



