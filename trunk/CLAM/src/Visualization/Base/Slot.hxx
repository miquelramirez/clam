#ifndef __SLOT__
#define __SLOT__

namespace CLAMGUI
{

class Signal;

class Slot
{
	typedef unsigned tSlotId;
public:
	Slot();
	Slot( tSlotId id, Signal* connectedSignal );
	Slot& operator=( Slot& s );
	Slot( Slot& s );

	tSlotId GetID() const
	{
		return mID;
	}

	~Slot();

private:
	bool    mMustFreeSignal;
	tSlotId mID;
	Signal* mConnectedSignal;
};

}

#endif // Slot.hxx
