#ifndef __SIGNAL__
#define __SIGNAL__

#include <utility>
#include <map>
#include <stack>

namespace CLAMGUI
{

class Slot;

class Signal
{
public:
	typedef unsigned  tSlotId;

	virtual void FreeSlot( Slot* ) = 0;

protected:
	void FreeSlotId( tSlotId );
	tSlotId AssignSlot();

	// Types
	typedef std::stack< unsigned >      tSlotIdStack;
protected:
	
	static tSlotIdStack  smFreeIdStack;
    static tSlotId       smLastSlotId;
};

}
#endif // Signal.hxx
