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

#ifndef __SEGMENTTFVIEW__
#define __SEGMENTTFVIEW__

#include "GView.hxx"
#include "ViewConfiguration.hxx"
#include "Segment.hxx"
#include "CBL.hxx"
#include "Enum.hxx"
#include "ErrGUI.hxx"
#include "Component.hxx"

using CLAM::Enum;
using CLAM::Segment;
using CLAM::Frame;
using CLAM::TData;
using CLAM::TTime;
using CLAM::DataArray;
using CLAM::Component;

namespace CLAMGUI
{

class SegmentTFView
	: public View
{
	// Callbacks typedefs
	typedef CBL::Functor2< const DataArray&, bool >             FloatArrayCb;
	typedef CBL::Functor4< TTime, TTime, TData, unsigned int>   ParametersCb;

	// Services definition
public:

	class Services
		: public Enum
	{
	public:
		static tEnumValue sEnumValues[];
		static tValue     sDefault;

		Services()
			: Enum( sEnumValues, sDefault )
		{
		}

		Services( tValue v )
			: Enum( sEnumValues, v )
		{
		}

		Services( string s )
			: Enum( sEnumValues, s )
		{
		}

		~Services()
		{
		}

		Component* Species() const
		{
			return new Services;
		}

		typedef enum 
		{ 
			GetTFData = 0,
			GetTFParameters
		} tEnum;
	};

	// methods definition
public:

	SegmentTFView()
		: View(), mObservedObj( NULL ), mFramesGetter( NULL ),mParametersGetter( NULL )
	{
	}

	virtual ~SegmentTFView()
	{
	}
	
	virtual const char* GetClassName()
	{
		return "SegmentTFView";
	}

	virtual void Refresh();
	
	virtual void Configure( Aspect& cfg );

	virtual void Release();

	virtual void BindTo( Segment* obj );

	// attributes
private:

	Segment*            mObservedObj;

	FloatArrayCb*       mFramesGetter;
	ParametersCb*       mParametersGetter;

};

}

#endif // SegmentTFView.hxx
