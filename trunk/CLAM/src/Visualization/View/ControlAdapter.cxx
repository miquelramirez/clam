/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "ControlAdapter.hxx"
#include "CLAM_Math.hxx"

namespace CLAMVM
{
		ControlAdapter::tManagedEmitter::tManagedEmitter()
				: mControl("OutControl"), mValueToEmit(0), mIsDirty( false )				  
		{
		}

		void ControlAdapter::tManagedEmitter::Emit(  )
		{
				mControl.SendControl( mValueToEmit );										
				mIsDirty = false;				
		}

		ControlAdapter::ControlAdapter()
				: mEmitter(), mIsBound( false ), mMinValue( 0 ), mMaxValue( 0 )
		{
			UpdateValue.Wrap( this, &InputControlModel::DispatchControlValue );
		}

		ControlAdapter::~ControlAdapter()
		{
		}
		
		bool ControlAdapter::BindTo( InControl& adaptee )
		{
				mEmitter.mControl.AddLink( adaptee );

				mIsBound = true;

				return mIsBound;
		}
		
		bool ControlAdapter::Publish()
		{
				ValuePublished.Emit( mEmitter.mValueToEmit );
				if ( mValueRangeChanged )
				{
						ValueRangePublished.Emit( mMinValue, mMaxValue );
						mValueRangeChanged = false;
				}

				return true;
		}
		
		bool ControlAdapter::Update()
		{
				if ( ! mEmitter.mIsDirty ) return true;

				if ( fabs( mMinValue - mMaxValue) > 0 ) // range is not null
				{
						if (mMinValue > mEmitter.mValueToEmit) return false;
						if (mEmitter.mValueToEmit > mMaxValue) return false;
						mEmitter.Emit( );
				}

				// if the range is null then no check is done

				mEmitter.mControl.SendControl( mEmitter.mValueToEmit );
				mEmitter.mIsDirty = false;

				return true;
		}

		void ControlAdapter::DispatchControlValue( CLAM::TControlData value )
		{
				mEmitter.mIsDirty = true;
				mEmitter.mValueToEmit = value;
						
		}

}

