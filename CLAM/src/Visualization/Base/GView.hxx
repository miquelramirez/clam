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

#ifndef __GVIEW__
#define __GVIEW__

#include "CBL.hxx"
#include "Err.hxx"

#include "Enum.hxx"
#include "Component.hxx"
#include "Storage.hxx"

#include "ViewConfiguration.hxx"

using CLAM::Err;
using CLAM::Enum;
using CLAM::Component;
using CLAM::Storage;

namespace CLAMGUI
{
	template < unsigned ServiceSelector >
	struct ServiceCallback
	{
		typedef void Type;
		
	};

	class View
	{
	public:
		class GenericServices
			: public Enum
		{
		public:
			static tEnumValue sEnumValues[];
			static tValue     sDefault;
			
			GenericServices()
				: Enum( sEnumValues, sDefault )
				{
				}
			
			GenericServices( tValue v )
				: Enum( sEnumValues, v )
				{
			}
			
			GenericServices( string s )
				: Enum( sEnumValues, s )
				{
				}
			
			~GenericServices()
				{
				}
			
			Component* Species() const
				{
					return new GenericServices;
				}
			
			typedef enum 
			{ 
				XMLDump = 0,
			} tEnum;
		};

	public:

		View() 
		  :	mServicesLocked( false ), 
			mModelObjLocked ( false)
		{
		}

		const char* GetClassName() const
		{
			return "BaseView";
		}

		virtual void Refresh() = 0;

		virtual void UpdateModel()
		{
		// :TBD: this should or should not be in the Base view interface
		}

		virtual void Release() = 0;

		virtual void Configure( Aspect& cfg ) = 0;

		virtual void NotifyUpdate()
		{
		// :TBD: this should or should not be in the Base view interface
		}

		// Services and model locking

		inline void LockServices()
		{
			mServicesLocked = true;
		}

		inline void LockModel()
		{
			mModelObjLocked = true;
		}

		inline void UnlockServices()
		{
			mServicesLocked = false;
		}

		inline void UnlockModel()
		{
			mModelObjLocked = false;
		}

		inline bool HasModelLocked() const
		{
			return mModelObjLocked;
		}

		inline bool HasServicesLocked() const
		{
			return mServicesLocked;
		}

	private:

		bool mServicesLocked;
		bool mModelObjLocked;

	};

	// Exception class for Bad signal signatures

	class ErrBadSignal
	: public Err
	{

	public:

		ErrBadSignal( const char* msg )
		: Err( msg )
		{
		}

	};

	
	template<>
		struct ServiceCallback<View::GenericServices::XMLDump>
	{
		typedef CBL::Functor0R< Storage& > Type;
	};


	// Helper functions
	// :TBD: This functions either should be part of the View interface
	//       or have a class of its own

	typedef CBL::FunctorBase       GenCb; // GenCb stands for "Generic Callback"
	typedef CBL::Functor1<View*>   HookCb; // Hook Cb for view expansion

	// :TODO: to document the ErrBadSignal exception throwing
	template < typename FunctorType >
	bool Acknowledge( FunctorType*& slot, GenCb* signal )
	{
		bool allOk = false;

		if ( signal != NULL )
		{
		slot = dynamic_cast< FunctorType* > ( signal );
		if ( slot == NULL )
		{
		  throw ( ErrBadSignal( "The signal had not the expected signature for the service requested" ) );
		}
		allOk = true;
		}

		return allOk;
	}

	// :TODO: to document that this function can throw an ErrAssertionFailed at runtime
	//        in a release build
	template < typename ViewType>
	void DelegateRequest( ViewType& delegate, const Enum& service, GenCb* signal )
	{
		typedef typename ViewType::Services   Services;

		Services s( service.GetString() );

		Aspect a;

		a.AddRequest( s, signal );

		delegate.Configure( a );
	}

	// :TODO: to document that this function can throw an ErrBadSignal
	inline bool ExpandRequest( View& expandee, GenCb* signal )
	{
		bool allOK = false;
		HookCb* expansion_hook = dynamic_cast< HookCb* >( signal );

		if ( expansion_hook == NULL )
		{
			throw( ErrBadSignal( "Not a suitable hook for requested expansion" ) );
		}
		else
		{
			(*expansion_hook)( &expandee );
			
			return allOK;
		}
	}


}

#endif // GView.hxx




