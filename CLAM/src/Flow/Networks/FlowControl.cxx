
#include "FlowControl.hxx"

#include <iostream>
#include "Network.hxx"
#include "Processing.hxx"
#include "Node.hxx"

namespace CLAM
{

FlowControl::FlowControl(  int frameSize )
	:mFrameSize(frameSize),
	 mNetwork(0),
	 mNetworkChanged(false)
{
}      

void FlowControl::AttachToNetwork( Network* network)
{
	mNetwork = network;
}

void FlowControl::ConfigurePorts(Processing& toConfigure) const
{
		Processing::InPortIterator itin; // todo : should be a typdef of PublishedInPorts
		for (itin = toConfigure.GetInPorts().Begin(); 
		     itin != toConfigure.GetInPorts().End(); 
		     itin++)
			(*itin)->SetParams(mFrameSize);
		
		Processing::OutPortIterator itout; // todo: idem
		for (itout = toConfigure.GetOutPorts().Begin(); 
		     itout != toConfigure.GetOutPorts().End(); 
		     itout++)
			(*itout)->SetParams(mFrameSize);		
}

void FlowControl::ProcessingAddedToNetwork( Processing& added )
{
	ConfigurePorts( added );
	NetworkTopologyChanged();
}

void FlowControl::ConfigureNode( NodeBase& toConfigure ) const
{
	toConfigure.Configure( mFrameSize );
}


void FlowControl::DoProcessings()
{
	Network::ProcessingsMap::iterator it;
	for ( it=mNetwork->BeginProcessings(); it!=mNetwork->EndProcessings(); it++ )
	{
		if (it->second->CanDoUsingPorts())
			it->second->Do();
	}
}


}
