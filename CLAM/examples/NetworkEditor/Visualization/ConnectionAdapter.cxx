
#include "ConnectionAdapter.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "Network.hxx"

namespace CLAMVM
{

ConnectionAdapter::ConnectionAdapter()
	: mOutObserved(0), mInObserved(0)
{
}

ConnectionAdapter::~ConnectionAdapter()
{
}

bool ConnectionAdapter::Publish()
{
	if ((!mOutObserved)  || (!mInObserved) || (!mNetworkObserved))
		return false;
	
	std::string outName("");
	std::string inName("");

	CLAM::Network::ProcessingsMap::const_iterator it;
	for(it=mNetworkObserved->BeginProcessings(); it!=mNetworkObserved->EndProcessings(); it++)
	{
		if( (it->second) == (mOutObserved->GetProcessing()) )
		{
			outName += it->first;
		}

		else if( (it->second) == (mInObserved->GetProcessing()) )
		{
			inName += it->first;
		}
	}
			
	outName += ".";
	outName += mOutObserved->GetName();


	inName += ".";
	inName += mInObserved->GetName();
	
	AcquireNames.Emit( outName, inName );
}

bool ConnectionAdapter::BindTo( const CLAM::OutPort& out, 
				const CLAM::InPort& in, 
				const CLAM::Network & net )
{
	mOutObserved = dynamic_cast< const CLAM::OutPort* > (&out);
	mInObserved = dynamic_cast< const CLAM::InPort* > (&in);
	mNetworkObserved = dynamic_cast< const CLAM::Network *> (&net);
	
	if ((!mOutObserved)  || (!mInObserved) || (!mNetworkObserved))
		return false;
	return true;
}

bool ConnectionAdapter::ConnectsInPort( CLAM::InPort & in)
{
	return (&in == mInObserved);
}


} // namespace CLAMVM
