
#include "FlowControl.hxx"

#include <iostream>
#include "Network.hxx"
#include "Processing.hxx"
#include "Node.hxx"

namespace CLAM
{

FlowControl::FlowControl(  int frameSize )
	:_frameSize(frameSize),
	 _network(0),
	 _networkChanged(false)
{
}      

void FlowControl::AttachToNetwork( Network* network)
{
	_network = network;
}

void FlowControl::ConfigurePorts(Processing& toConfigure) const
{
		Processing::InPortIterator itin; // todo : should be a typdef of PublishedInPorts
		for (itin = toConfigure.GetInPorts().Begin(); 
		     itin != toConfigure.GetInPorts().End(); 
		     itin++)
			(*itin)->SetParams(_frameSize);
		
		Processing::OutPortIterator itout; // todo: idem
		for (itout = toConfigure.GetOutPorts().Begin(); 
		     itout != toConfigure.GetOutPorts().End(); 
		     itout++)
			(*itout)->SetParams(_frameSize);		
}

void FlowControl::ProcessingAddedToNetwork( Processing& added )
{
	ConfigurePorts( added );
	NetworkTopologyChanged();
}

void FlowControl::ConfigureNode( NodeBase& toConfigure ) const
{
	toConfigure.Configure( _frameSize );
}


void FlowControl::DoProcessings()
{
	Network::ProcessingsMap::iterator it;
	for ( it=_network->BeginProcessings(); it!=_network->EndProcessings(); it++ )
	{
		if (it->second->CanDoUsingPorts())
			it->second->Do();
	}
}


}
