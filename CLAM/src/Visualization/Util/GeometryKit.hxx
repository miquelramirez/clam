/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _GEOMETRYKIT_
#define _GEOMETRYKIT_

#include <utility>

namespace CLAMGUI
{
	/**
	 *	This class encapsulates the concept of a screen rectangle
	 *	Note it is templatized
	 */
	template <class T> class Rect
	{
	private:
		/**
		 *	top-left x axis coordinate
		 */
		T mXPos;
		/**
		 *	top-left y axis coordinate
		 */
		T mYPos;
		/**
		 *	widget width
		 */
		T mWidth;
		/**
		 *	widget height
		 */
		T mHeight;
	public:
		/**
		 *	Rect class constructor
		 *	@param x x-axis coordinate
		 *	@param y y-axis coordinate
		 *	@param w widget's width
		 *	@param h widget's height
		 */
		Rect()
			: mXPos(0), mYPos(0), mWidth(0), mHeight(0)
		{
		}
		
		Rect (T x, T y, T w, T h) : mXPos(x), mYPos(y), mWidth(w), mHeight(h)
		{
			
		}
		
		Rect (const Rect<T>& obj) : mXPos(obj.GetX()), mYPos(obj.GetY()), mWidth(obj.GetW()), mHeight(obj.GetH())
		{
			
		}
		
		/**
		 *	Rect class destructor
		 */
		virtual ~Rect()
		{
			
		}
		
		/**
		 *	Assignment operator overloaded in order to make easier to
		 *	change the Rect coordinates at once
		 *	@param r the original rect, possibly instantiated on the fly
		 */
		void operator=(const Rect& r)
		{
			this->mXPos	 = r.mXPos;
			this->mYPos	 = r.mYPos;
			this->mWidth	= r.mWidth;
			this->mHeight = r.mHeight;
		}
		
		/**
		 *	Inspector for querying a Rect object about its top-left x-axis coord
		 *	@return the coordinate
		 */
		T GetX() const
		{
			return mXPos;
		}
		
		/**
		 *	Inspector for querying a Rect object about its top-left y-axis coord
		 *	@return the coordinate
		 */
		T GetY() const
		{
			return mYPos;
		}
		
		
		/**
		 *	Inspector for querying a Rect object about its width
		 *	@return rect instance actual width
		 */
		T GetW() const
		{
			return mWidth;
		}
		
		/**
		 *	Inspector for querying a Rect object about its height
		 *	@return rect instance actual height
		 */
		T GetH() const
		{
			return mHeight;
		}

		/**
		 *	Modifier for setting the object's top-left x-axis coordinate
		 *	@param x the new x coordinate value
		 */
		void SetX(T x)
		{
			mXPos = x;
		}
		
		/**
		 *	Modifier for setting the object's top-left y-axis coordinate
		 *	@param y the new y coordinate value
		 */
		void SetY(T y)
		{
			mYPos = y;
		}
		
		/**
		 *	Modifier for setting the object's width
		 *	@param w the new value for the width
		 */
		void SetW(T w)
		{
			mWidth = w;
		}
		
		/**
		 *	Modifier for setting the object's height
		 *	@param h the new value for the height
		 */
		void SetH(T h)
		{
			mHeight = h;
		}
		
	};
	
	/**
	 *	Rect<T> specialization for representing an Integer values rectangle
	 */
	typedef Rect<int> Recti;	
	/**
	 *	Rect<T> specialization for representing a floating point values rectangle
	 */
	typedef Rect<double> Rectd; // Double defined rectangle
	
	typedef Rect<int> Geometry;
}

#endif

