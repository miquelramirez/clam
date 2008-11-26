#include <CLAM/OutControlBase.hxx>
#include <CLAM/Processing.hxx>

namespace CLAM{
	OutControlBase::OutControlBase(const std::string &name, Processing * proc)
		: mName(name)
		, mProcessing(proc)
	{
		if (proc) proc->RegisterOutControl(this);
	}
	
	OutControlBase::~OutControlBase()
	{
		while (!mLinks.empty())
			RemoveLink(*mLinks.front());
		if (mProcessing)
			mProcessing->GetOutControls().ProcessingInterface_Unregister(this);
	}
	OutControlBase::Peers::iterator OutControlBase::BeginInControlsConnected()
	{
		return mLinks.begin();
	}

	OutControlBase::Peers::iterator OutControlBase::EndInControlsConnected()
	{
		return mLinks.end();
	}

}
