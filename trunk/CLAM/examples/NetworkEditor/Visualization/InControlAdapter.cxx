
#include "InControlAdapter.hxx"
#include "InControl.hxx"

namespace CLAMVM
{

InControlAdapter::InControlAdapter()
	: mObserved(0)
{
}

InControlAdapter::~InControlAdapter()
{
}

bool InControlAdapter::Publish()
{
	if (!mObserved)
		return false;

	AcquireName.Emit(mObserved->GetName());
}

bool InControlAdapter::BindTo( const CLAM::InControl& obj)
{
	mObserved = dynamic_cast< const CLAM::InControl* > (&obj);
	
	if ( !mObserved )
	{ 
		return false;
	}
	return true;
}

} // namespace CLAMVM
