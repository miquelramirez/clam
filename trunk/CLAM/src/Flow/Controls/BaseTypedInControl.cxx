#include <CLAM/BaseTypedInControl.hxx>
#include <CLAM/Processing.hxx>

namespace CLAM{
	BaseTypedInControl::BaseTypedInControl(const std::string &name, Processing * proc)
		: mName(name)
		, mProcessing(proc)
	{
		if (proc) proc->RegisterTypedInControl(this);
	}
	
	BaseTypedInControl::~BaseTypedInControl()
	{
		if (mProcessing)
			mProcessing->GetTypedInControls().ProcessingInterface_Unregister(this);
	}
}
