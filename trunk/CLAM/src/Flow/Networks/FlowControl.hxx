
#ifndef _FlowControl_hxx_
#define _FlowControl_hxx_


#include <list>

namespace CLAM
{

class Network;
class NodeBase;
class Processing;

class FlowControl
{
	bool _networkChanged;

public:
	FlowControl( int frameSize = 0 );
	virtual ~FlowControl(){}
	virtual void AttachToNetwork( Network* );
	
	//methods relative to state of FlowControl & Network
	virtual void ProcessingAddedToNetwork( Processing& added );

	void ConfigureNode( NodeBase& toConfigure ) const;
	
	// to implement in each type of flowcontrol
	virtual void DoProcessings() = 0;

	void NetworkTopologyChanged() { _networkChanged=true;}

protected:
	bool HasNetworkTopologyChanged() const { return _networkChanged; }
	void ConfigurePorts(Processing &toConfigure) const;
	
	int _frameSize;
	Network * _network;

};

}

#endif
