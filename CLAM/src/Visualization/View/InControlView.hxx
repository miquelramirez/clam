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

#ifndef __INCONTROLVIEW__
#define __INCONTROLVIEW__

#include <string>
#include "GView.hxx"
#include "Component.hxx"
#include "Enum.hxx"
#include "ViewConfiguration.hxx"
#include "Processing.hxx"
#include "ErrGUI.hxx"
#include "DataTypes.hxx"


using CLAM::Processing;
using CLAM::Enum;
using CLAM::TData;
using CLAM::Component;
#include "CBL.hxx"
using std::string;

namespace CLAMGUI
{

	class InControlView
		: public View
	{

		typedef CBL::FunctorBase             GenCb;
		typedef CBL::Functor0R< TData >      GetSignalCb;

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

			typedef enum { Apply_Control = 0 } tEnum;

		};

	public:

		InControlView() :
		  View(), mSignalGetter( NULL ), mMustDoUpdate( false ), mControlID( -1 ), 
		  mObservedObj( NULL ), mLastValue( 0 )
	          {
		  }
		
		virtual ~InControlView()
			{
			}
		
		void Configure( Aspect& cfg );
		
		virtual const char* GetClassName()
			{
				return "InControlView";
			}
		
		virtual void BindTo( Processing* obj, unsigned int control );
		
		virtual void Refresh();
		
		virtual void UpdateModel();
		
		inline bool IsConfigured() const 
			{
				return HasModelLocked() && HasServicesLocked();
			}
		
		virtual void Release();
		
		virtual void NotifyUpdate();
		

		
	private:
		
		GetSignalCb*        mSignalGetter;
		bool                mMustDoUpdate;
		int                 mControlID;
		Processing*         mObservedObj;
		TData               mLastValue;
		
	};

}


#endif // InControlView.hxx


