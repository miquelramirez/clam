#include "TypedOutControl.hxx"

namespace CLAM
{
	BaseTypedOutControl::BaseTypedOutControl(const std::string &name, Processing * proc)
		: mName(name)
		, mProcessing(proc)
	{
		if (proc) proc->RegisterTypedOutControl(this);
	}

	/**
		WARNING: You should call IsLinkable before using the Link function or you'll get an assert failure if In and Out Control types are different.
	*/
	void BaseTypedOutControl::BaseTypedOutControl::Link(BaseTypedInControl& in)
	{
		bool successfullLink;
		successfullLink = DoTypedLink(in);
		CLAM_ASSERT( successfullLink,
			     "TypedOutControl<TypedControlData>::Link coudn't connect to TypedInControl "
	   		     "because was not templatized by the same TypedControlData type as TypedOutControl" );
	}

}
