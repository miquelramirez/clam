#include "Slot.hxx"
#include "Signal.hxx"

namespace CLAMGUI
{

	Slot::Slot()
		: mMustFreeSignal(false), mConnectedSignal( NULL )
	{		
	}

	Slot::~Slot()
	{
		if ( mMustFreeSignal )
			mConnectedSignal->FreeSlot( this );
	}

	Slot::Slot( tSlotId id, Signal* connSig )
		: mMustFreeSignal( true ), mID( id ), mConnectedSignal( connSig )
	{
	}

	Slot::Slot( Slot& s )
	{
		mMustFreeSignal = s.mMustFreeSignal;
		mID = s.mID;
		mConnectedSignal = s.mConnectedSignal;
		s.mMustFreeSignal = false;
	}

	Slot& Slot::operator=( Slot& s )
	{
		mMustFreeSignal = s.mMustFreeSignal;
		mID = s.mID;
		mConnectedSignal = s.mConnectedSignal;
		s.mMustFreeSignal = false;

		return *this;
	}

}
