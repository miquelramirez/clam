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

#ifndef __TOKENDELAYVIEW__
#define __TOKENDELAYVIEW__

#include "GView.hxx"
#include "ViewConfiguration.hxx"
#include "ProcObjView.hxx"
#include "TokenDelay.hxx"
#include "Enum.hxx"
#include "Component.hxx"
#include "InControlView.hxx"
#include "CBL.hxx"
#include "SpectrumGView.hxx"

using CLAM::TokenDelayConfig;
using CLAM::Enum;
using CLAM::Component;
using CLAM::TData;


namespace CLAMGUI
{

	typedef TokenDelay<Spectrum> SpectralDelay;

template <>
class ProcObjView< SpectralDelay >
	: public View
{

	typedef CBL::Functor1< TData >   PublishTDataCb;

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
			GetDelay = 0,
			SetDelay,
			GetDelayOutputView
		} tEnum;

	};
 
public:

	ProcObjView()
		: View(), mDelayGetter( NULL )
	{
		Release();
	}

	virtual ~ProcObjView()
	{
	}

	virtual const char* GetClassName()
	{
		return "SpectralDelayView";
	}

	virtual void UpdateModel();

	virtual void NotifyUpdate()
	{
		if ( HasServicesLocked() && HasModelLocked() )
		{
			mControlView.NotifyUpdate();
		}
	}

	virtual void Refresh();

	virtual void Release();

	virtual void Configure( Aspect& cfg );

	virtual void BindTo( SpectralDelay* obj );

private:

	SpectralDelay*            mObservedObj;

	PublishTDataCb*           mDelayGetter;
	ProcDataView<Spectrum>    mSpectrumView;
	InControlView             mControlView;

};


}

#endif // TokenDelayView.hxx
