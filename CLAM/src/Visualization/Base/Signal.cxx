#include "Signal.hxx"

namespace CLAMGUI
{
	Signal::tSlotId        Signal::smLastSlotId = 0;
	Signal::tSlotIdStack   Signal::smFreeIdStack;

	Signal::tSlotId Signal::AssignSlot()
	{
		tSlotId id;
		if ( smFreeIdStack.empty() )
			{
				id = smLastSlotId;
				smLastSlotId++;
				return id;
			}
		else
			{
				id = smFreeIdStack.top();
				smFreeIdStack.pop();
				return id;
			}
	}

	void Signal::FreeSlotId( Signal::tSlotId freedSlotId )
	{
		smFreeIdStack.push( freedSlotId );
	}
}
