
#include "OutControlAdapter.hxx"
#include "OutControl.hxx"

namespace CLAMVM
{

OutControlAdapter::OutControlAdapter()
	: mObserved(0)
{
}

OutControlAdapter::~OutControlAdapter()
{
}

bool OutControlAdapter::Publish()
{
	if (!mObserved)
		return false;

	AcquireName.Emit(mObserved->GetName());
}

bool OutControlAdapter::BindTo( const CLAM::OutControl& obj)
{
	mObserved = dynamic_cast< const CLAM::OutControl* > (&obj);
	
	if ( !mObserved )
	{ 
		return false;
	}
	return true;
}

} // namespace CLAMVM

