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

#ifndef __SPECTRUMGVIEW__
#define __SPECTRUMGVIEW__

#include "ErrGUI.hxx"
#include "CBL.hxx"
#include "ProcDataView.hxx"
#include "Spectrum.hxx"
#include "Component.hxx"
#include "Enum.hxx"
#include "GlobalEnums.hxx"

using CLAM::DataArray;
using CLAM::Spectrum;
using CLAM::TData;
using CLAM::Component;
using CLAM::Enum;
using CLAM::EScale;

namespace CLAMGUI
{

	template <>
	class ProcDataView<Spectrum> 
		: public View
	{
		typedef CBL::Functor1< const DataArray& >                 ArrayCb;
		typedef CBL::Functor1< TData >                            IntCb;
		typedef CBL::Functor2< unsigned int, EScale >             ScaleCb;
		
	public:
		
		// Services definition
		
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
				GetMag = 0,
				GetPhase,
				GetSpectralRange,
				GetSpectralResolution,
				GetScale
			} tEnum;
		};
		
	public:
		
		ProcDataView()
			: View(), mObservedObj( NULL ), RenderMagnitude( NULL ), RenderPhase( NULL ),
			  RenderSpectralRange( NULL ), RenderSpectralResolution( NULL ), RenderScale( NULL )
			{
				Release();
			}
		
		virtual ~ProcDataView()
			{
			}
		
		virtual const char* GetClassName()
			{
				return "SpectrumView";
			}
		
		virtual void Refresh();
		
		virtual void Release();
		
		virtual void Configure( Aspect& v );
		
		virtual void BindTo( Spectrum* obj )
			{
				CLAM_ASSERT( obj!=NULL, "Object was null!!!");
				
				mObservedObj = obj;
				LockModel();
			}
		
	private:
		
		Spectrum*           mObservedObj;
		ArrayCb*            RenderMagnitude;
		ArrayCb*            RenderPhase;
		IntCb*              RenderSpectralRange;
		IntCb*              RenderSpectralResolution;
		ScaleCb*            RenderScale;

};

}

#endif // SpectrumGView.hxx
