#ifndef _AudioInPortTmpl_hxx_
#define _AudioInPortTmpl_hxx_


#include "Audio.hxx"
#include "InPort.hxx"
#include "Node.hxx"

namespace CLAM
{

template<>
class InPortTmpl<Audio> : public InPort
{
	ReadStreamRegion *mpRegion;
	Node<Audio> *mpNode;
	Audio *mpData;
	Audio mWrapper;
public:
	inline InPortTmpl(std::string n, Processing *o, int length, int hop = 0, bool inplace=false);
	inline Audio &GetData();
	inline void LeaveData();
	void Attach(ProcessingData& data);
	inline void Attach(Audio& data);
	void Attach( NodeBase& node);
	inline void Attach(Node<Audio> &n);
	inline void Attach(InPortTmpl<Audio> &n);
	inline void Accept(DataVisitor&);
	ProcessingData* GetProcessingData();
	NodeBase* GetNode();
	bool IsAttached();
	bool IsReadyForReading();
	void Unattach();
};

//-----------------------------------------------------------------------------------------------
// Template method implementations

InPortTmpl<Audio>::InPortTmpl(std::string n,
							  Processing *o,
							  int length,
							  int hop,
							  bool inplace)
	: InPort(n,o,length,hop,inplace),
	  mpRegion(0),
	  mpNode(0),
	  mpData(0)
{
	o->PublishInPort(this);
}

Audio &InPortTmpl<Audio>::GetData()	
{ 
	CLAM_ASSERT(mpData || (mpNode && mpRegion),
				"InPortTmpl::GetData(): No data atached to the port.");
	if (mpNode) {
		mpNode->GetAndActivate(mpRegion,mWrapper);
		mpData = &mWrapper;
	}
	return *mpData;
}

void InPortTmpl<Audio>::LeaveData()	
{
	if (mpNode)
		mpNode->LeaveAndAdvance(mpRegion);
}

inline void InPortTmpl<Audio>::Attach(ProcessingData& data)
{
	try{
		Attach(dynamic_cast<Audio&> (data));
	}
	// the exception catched should be std::bad_cast instead of std::exception. 
	// to fix when VC6 is no longer supported
	catch (std::exception){
		CLAM_ASSERT(false,"You are trying to attach a processing data that is not an Audio to an Audio port");
	}
}

inline void InPortTmpl<Audio>::Attach(Audio& data)
{
	mpNode = 0;
	mpData = &data;
}	

inline void InPortTmpl<Audio>::Attach( NodeBase& node)
{
	try {
		Attach( dynamic_cast< Node<Audio>& >(node) );
	}
	// the exception catched should be std::bad_cast instead of std::exception. 
	// to fix when VC6 is no longer supported
	catch (std::exception) {
		CLAM_ASSERT(false,"You are trying to attach a node that is not suitable for this port");
	}
}

inline void InPortTmpl<Audio>::Attach(Node<Audio>& node)
{
	CLAM_ASSERT( mpNode == 0, "InPortTmpl::Attach. Trying to attach an inport already connected"
		     "Try to call Unattach before connect another time the port" );
	CLAM_ASSERT( mpRegion == 0, "InPortTmpl::Attach. Trying to attach an inport already connected"
		     "Try to call Unattach before connect another time the port" );
	mpData = 0;
	mpNode = &node;
	mpRegion = node.NewReader(this, Hop(), Length());
}	

inline void InPortTmpl<Audio>::Attach(InPortTmpl<Audio>& port)
{
	if (port.mpNode)
		Attach(*port.mpNode);
	else
		Attach(*port.mpData);
}	
inline bool InPortTmpl<Audio>::IsAttached()
{
	return mpData || mpNode;
}

inline bool InPortTmpl<Audio>::IsReadyForReading()
{
	if( !GetNode() ) //only is "ready" when  Port attached to Node
		return false;
	
	return mpNode->CanActivateRegion( *mpRegion );
}


inline void InPortTmpl<Audio>::Unattach()
{
	if( !IsAttached() )
		return;

	if(mpNode)
	{
		mpNode->RemoveInPortConnection( this, mpRegion );
		delete mpRegion;
		mpNode = 0;
		mpRegion = 0;
	}
	mpData = 0;

}

void InPortTmpl<Audio>::Accept(DataVisitor& v)
{
	v.Visit(*mpData);
}

inline ProcessingData* InPortTmpl<Audio>::GetProcessingData()
{
	if (IsAttached())
	{
		return mpData; 
	}
	return 0;
}

inline NodeBase* InPortTmpl<Audio>::GetNode()
{
	return mpNode;
}


} // namespace CLAM

#endif
