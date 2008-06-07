#include <CLAM/TypedInControl.hxx>

namespace CLAM{
	BaseTypedInControl::BaseTypedInControl(const std::string &name, Processing * proc)
		: mName(name)
		, mProcessing(proc)
	{
		if (proc) proc->RegisterTypedInControl(this);
	}	
}
