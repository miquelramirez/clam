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

#ifndef __SEGMENTVIEW__
#define __SEGMENTVIEW__

#include "ProcDataView.hxx"
#include "GView.hxx"
#include "ViewConfiguration.hxx"
#include "Segment.hxx"
#include "SegmentTFView.hxx"
#include "CBL.hxx"
#include "Enum.hxx"

using CLAM::Enum;
using CLAM::Segment;
using CLAM::TData;
using CLAM::TTime;
using CLAM::DataArray;

namespace CLAMGUI
{
template<>
class ProcDataView< Segment > 
	: public View
{
	// Callbacks typedefs
	typedef CBL::Functor2< const DataArray&, bool >      FloatArrayCb;
	typedef CBL::Functor1< TTime >                       TimeCb;
	typedef CBL::Functor1< TData >                       TDataCb;
	typedef CBL::Functor1< int >                         IntCb;

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
			GetTFView = 0,
			GetDuration,
		} tEnum;
	};

	// methods definition
public:

	ProcDataView()
		: View(), mObservedObj( NULL ), mDurationGetter( NULL )
	{
	}

	virtual ~ProcDataView()
	{
	}
	
	virtual const char* GetClassName() const
	{
		return "SegmentView";
	}

	virtual void Refresh();
	
	virtual void Configure( Aspect& cfg );

	virtual void Release();


	virtual void BindTo( Segment* obj );


	// attributes
private:

	Segment*            mObservedObj;
	SegmentTFView      mTFView;
	TimeCb*             mDurationGetter;

};

}

#endif // SegmentView.hxx
