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
		if (mProcessing)
			mProcessing->GetTypedOutControls().ProcessingInterface_Unregister(this);
	}
}
