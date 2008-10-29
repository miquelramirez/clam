#include <CLAM/BaseTypedOutControl.hxx>
#include <CLAM/Processing.hxx>

namespace CLAM{
	BaseTypedOutControl::BaseTypedOutControl(const std::string &name, Processing * proc, bool publish)
		: mName(name)
		, mProcessing(proc)
	{
		if (proc and publish) proc->RegisterOutControl(this);
	}
	
	BaseTypedOutControl::~BaseTypedOutControl()
	{
		while (!mLinks.empty())
			RemoveLink(*mLinks.front());
		if (mProcessing)
			mProcessing->GetOutControls().ProcessingInterface_Unregister(this);
	}
	BaseTypedOutControl::Peers::iterator BaseTypedOutControl::BeginInControlsConnected()
	{
		return mLinks.begin();
	}

	BaseTypedOutControl::Peers::iterator BaseTypedOutControl::EndInControlsConnected()
	{
		return mLinks.end();
	}

}
