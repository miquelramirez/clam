
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
	bool mNetworkChanged;

public:
	FlowControl( int frameSize = 512 );
	virtual ~FlowControl(){}
	virtual void AttachToNetwork( Network* );
	
	//methods relative to state of FlowControl & Network
	virtual void ProcessingAddedToNetwork( Processing& added );

	void ConfigureNode( NodeBase& toConfigure ) const;
	
	// to implement in each type of flowcontrol
	virtual void DoProcessings() = 0;

	void NetworkTopologyChanged() { mNetworkChanged=true;}

protected:
	bool HasNetworkTopologyChanged() const { return mNetworkChanged; }
	void ConfigurePorts(Processing &toConfigure) const;
	
	int mFrameSize;
	Network * mNetwork;

};

}

#endif
