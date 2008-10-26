#include <CLAM/BaseTypedOutControl.hxx>
#include <CLAM/Processing.hxx>

namespace CLAM{
	BaseTypedOutControl::BaseTypedOutControl(const std::string &name, Processing * proc)
		: mName(name)
		, mProcessing(proc)
	{
		if (proc) proc->RegisterTypedOutControl(this);
	}
	
	BaseTypedOutControl::~BaseTypedOutControl()
	{
		while (!mLinks.empty())
			RemoveLink(*mLinks.front());
//		if (mProcessing)
//			mProcessing->GetOutControls().ProcessingInterface_Unregister(this);
	}
}
