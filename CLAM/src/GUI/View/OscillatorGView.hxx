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

#ifndef __OSCILLATORVIEW__
#define __OSCILLATORVIEW__

#include "DataTypes.hxx"
#include "ProcObjView.hxx"
#include "InControlView.hxx"
#include "Oscillator.hxx"
#include "Enum.hxx"
#include "ErrGUI.hxx"

using CLAM::Component;

using CLAM::Enum;
using CLAM::Oscillator;
using CLAM::OscillatorConfig;
using CLAM::TData;
using CLAM::EOscillatorControls;
#include "CBL.hxx"


namespace CLAMGUI
{

	template<>
	class ProcObjView< Oscillator > 
		: public View
	{

		typedef CBL::Functor1 < TData >         PublishTDataCb;

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
				GetPitch = 0,
				GetAmp,
				GetModIdx,
				GetPhase,
				SetPitch,
				SetAmp,
				SetModIdx,
				SetPhase
			} tEnum;

		};


	public:

		ProcObjView() :
			View(), mObservedObj( NULL ), RenderFreq( NULL ), RenderAmp( NULL ), RenderModIndex( NULL ),
			RenderPhase( NULL )
		{
		}

		virtual ~ProcObjView()
		{
		}

		virtual const char* GetClassName()
		{
			return "OscillatorView";
		}

		virtual void UpdateModel();

		virtual void NotifyUpdate()
		{
			if ( HasServicesLocked() && HasModelLocked() )
			{
				mFreqView.NotifyUpdate();
				mAmpView.NotifyUpdate();
				mPhaseView.NotifyUpdate();
				mModIdxView.NotifyUpdate();
			}
		}

		virtual void Refresh();

		virtual void Release();

		virtual void Configure( Aspect& cfg );

		virtual void BindTo( Oscillator* obj )
		{
			CLAM_ASSERT( obj!=NULL, "Model Object was null!!" );

			mObservedObj = obj;

			mFreqView.BindTo( obj, EOscillatorControls::pitch );
			mAmpView.BindTo( obj, EOscillatorControls::amplitude );
			mModIdxView.BindTo( obj, EOscillatorControls::modidx );
			mPhaseView.BindTo( obj, EOscillatorControls::phase );

			LockModel();
		}

	private:
		Oscillator*          mObservedObj;

		// Control views

		InControlView        mFreqView;
		InControlView        mAmpView;
		InControlView        mModIdxView;
		InControlView        mPhaseView;

		// Output
		PublishTDataCb*      RenderFreq;
		PublishTDataCb*      RenderAmp;
		PublishTDataCb*      RenderModIndex;
		PublishTDataCb*      RenderPhase;

	};

}

#endif // Oscillator.hxx
