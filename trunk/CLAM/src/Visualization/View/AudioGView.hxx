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

#ifndef __AUDIOVIEW__
#define __AUDIOVIEW__

#include "GView.hxx"
#include "ProcDataView.hxx"
#include "Audio.hxx"
#include "ViewConfiguration.hxx"
#include "CBL.hxx"
#include "ErrGUI.hxx"
#include "Component.hxx"
#include "Enum.hxx"


using CLAM::Audio;
using CLAM::DataArray;
using CLAM::TData;
using CLAM::Enum;
using CLAM::Component;

namespace CLAMGUI
{

	template <>
	class ProcDataView< Audio > 
		: public View
	{
		typedef CBL::Functor1< const DataArray& >        FloatArrayCb;
		typedef CBL::Functor1< int >                     IntCb;
		typedef CBL::Functor1< TData >                   TDataCb;

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
				GetBuffer = 0,
				GetSampleRate,
				GetDuration,
			} tEnum;
		};

	public:

		ProcDataView()
			: View(), mObservedObj( NULL ), RenderFloatBuffer( NULL ),
		RenderAudioDuration( NULL )
		{
		}

		virtual ~ProcDataView()
		{
		}

		const char* GetClassName() const
		{
			return "AudioView";
		}

		virtual void Refresh();

		virtual void Release();

		virtual void Configure( Aspect& v );

		virtual void BindTo( Audio* obj )
		{

			CLAM_ASSERT( obj!= NULL, "Object was null!!!" );

			mObservedObj = obj;
			LockModel();
		}

	private:

		Audio*            mObservedObj;
		Aspect            mCfg;
		FloatArrayCb*     RenderFloatBuffer;
		IntCb*            RenderSamplingRate;
		TDataCb*          RenderAudioDuration;

	};

}

#endif // AudioGView.hxx
