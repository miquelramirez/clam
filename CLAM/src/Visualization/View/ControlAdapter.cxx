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
				mEmitter.mControl.AddLink( &adaptee );

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
