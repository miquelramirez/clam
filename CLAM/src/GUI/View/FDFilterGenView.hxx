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

#ifndef __FDFILTERGENVIEW__
#define __FDFILTERGENVIEW__

#include "GView.hxx"
#include "ViewConfiguration.hxx"
#include "ProcObjView.hxx"
#include "FDFilterGen.hxx"
#include "Enum.hxx"
#include "Component.hxx"
#include "InControlView.hxx"

using CLAM::Enum;
using CLAM::Component;
using CLAM::FDFilterGen;
using CLAM::EFDFilterGenControls;
using CLAM::TData;
#include "CBL.hxx"

namespace CLAMGUI
{
	template <>
	class ProcObjView< FDFilterGen > 
		: public View
	{
		typedef CBL::Functor1<  TData >             PublishTDataCb;
		typedef CBL::Functor1< const std::string& >       PublishStringCb;
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
				GetGain = 0,
				GetHighCutoff,
				GetLowCutoff,
				GetPassBandSlope,
				GetStopBandSlope,
				GetName,
				SetGain,
				SetHighCutoff,
				SetLowCutoff,
				SetPassBandSlope,
				SetStopBandSlope
			} tEnum;

		};
		
	public:

		ProcObjView()
			: View(), mObservedObj( NULL ), RenderGain( NULL ), RenderLowCutoff( NULL ),
			  RenderHighCutoff( NULL ), RenderPassBandSlope( NULL ), RenderStopBandSlope( NULL ),
			  RenderName( NULL )
			{
			}

		virtual ~ProcObjView()
			{
			}
		
		virtual const char* GetClassName()
			{
				return "FDFilterGenView";
			}
		
		virtual void UpdateModel();
		
		virtual void NotifyUpdate()
			{
				if ( HasServicesLocked() && HasModelLocked() )
					{
						mGainCtlView.NotifyUpdate();
						mLCCtlView.NotifyUpdate();
						mHCCtlView.NotifyUpdate();
						mPBSCtlView.NotifyUpdate();
						mSBSCtlView.NotifyUpdate();
					}
			}
		
		virtual void Refresh();
		
		virtual void Release();
		
		virtual void Configure( Aspect& cfg );
		
		virtual void BindTo( FDFilterGen* obj )
			{
				CLAM_ASSERT( obj!=NULL, "Object was null!!!" );
				
				mObservedObj = obj;

				mGainCtlView.BindTo( obj, EFDFilterGenControls::gain );
				mLCCtlView.BindTo( obj, EFDFilterGenControls::lowcutoff );
				mHCCtlView.BindTo( obj, EFDFilterGenControls::highcutoff );
				mPBSCtlView.BindTo( obj, EFDFilterGenControls::passbandslope );
				mSBSCtlView.BindTo( obj, EFDFilterGenControls::stopbandslope );
				
				LockModel();
			}
		
	private:
		
		FDFilterGen*           mObservedObj;

		
		// Output
		PublishTDataCb*        RenderGain;
		PublishTDataCb*        RenderLowCutoff;
		PublishTDataCb*        RenderHighCutoff;
		PublishTDataCb*        RenderPassBandSlope;
		PublishTDataCb*        RenderStopBandSlope;
		PublishStringCb*       RenderName;

		// Subviews
		
		InControlView          mGainCtlView; // Gain Control
		InControlView          mLCCtlView; // LowCutoff Control
		InControlView          mHCCtlView; // HighCutoff Control
		InControlView          mPBSCtlView; // PassBandSlope Control
		InControlView          mSBSCtlView; // StopBandSlope Control
		
	};
	
}

#endif // __FDFILTERGENVIEW__
