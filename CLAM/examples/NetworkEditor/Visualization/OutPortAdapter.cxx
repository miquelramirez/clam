
#include "OutPortAdapter.hxx"
#include "OutPort.hxx"

namespace CLAMVM
{

OutPortAdapter::OutPortAdapter()
	: mObserved(0)
{
}

OutPortAdapter::~OutPortAdapter()
{
}

bool OutPortAdapter::Publish()
{
	if (!mObserved)
		return false;

	AcquireName.Emit(mObserved->GetName());
}

bool OutPortAdapter::BindTo( const CLAM::OutPort& obj)
{
	mObserved = dynamic_cast< const CLAM::OutPort* > (&obj);
	
	if ( !mObserved )
	{ 
		return false;
	}
	return true;
}

} // namespace CLAMVM

