
#include "InPortAdapter.hxx"
#include "InPort.hxx"

namespace CLAMVM
{

InPortAdapter::InPortAdapter()
	: mObserved(0)
{
}

InPortAdapter::~InPortAdapter()
{
}

bool InPortAdapter::Publish()
{
	if (!mObserved)
		return false;

	AcquireName.Emit(mObserved->GetName());
}

bool InPortAdapter::BindTo( const CLAM::InPort& obj)
{
	mObserved = dynamic_cast< const CLAM::InPort* > (&obj);
	
	if ( !mObserved )
	{ 
		return false;
	}
	return true;
}

} // namespace CLAMVM
