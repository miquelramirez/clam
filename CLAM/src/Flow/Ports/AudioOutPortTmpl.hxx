#ifndef _AudioOutPortTmpl_hxx_
#define _AudioOutPortTmpl_hxx_

#include "Audio.hxx"
#include "AudioInPortTmpl.hxx"
#include "OutPort.hxx"
#include "Node.hxx"

namespace CLAM
{

template<>
class OutPortTmpl<Audio> : public OutPort
{
	WriteStreamRegion *mpRegion;
	Node<Audio> *mpNode;
	Audio *mpData;
	Audio mWrapper;
public:

	inline OutPortTmpl(std::string n, Processing *o, int length, int hop = 0);
	inline ~OutPortTmpl();
	inline Audio &GetData();
	inline void LeaveData();
	void Attach(ProcessingData& data);
	void Attach( NodeBase& node);
	inline void Attach(Audio& data);
	inline void Attach(OutPortTmpl<Audio> &n);
	inline void Attach(Node<Audio> &n);
	inline void Accept(DataVisitor&);
	NodeBase* GetNode();
	
	ProcessingData* GetProcessingData();
	bool IsAttached();
	bool IsReadyForWriting();
	void Unattach();
	bool IsConnectableTo(InPort & );

};


//-----------------------------------------------------------------------------------------------
// Template method implementations
OutPortTmpl<Audio>::OutPortTmpl(std::string n,
				Processing *o,
				int length,
				int hop)
	: OutPort(n,o,length,hop),
	  mpRegion(0),
	  mpNode(0),
	  mpData(0)
{
	o->PublishOutPort(this);
}

inline OutPortTmpl<Audio>::~OutPortTmpl()
{
	if (mpRegion)
		delete mpRegion;
}


inline void OutPortTmpl<Audio>::Attach(ProcessingData& data)
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

inline void OutPortTmpl<Audio>::Attach( NodeBase& node)
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

void OutPortTmpl<Audio>::Attach(Audio& data)
{
	mpData = &data;
	mpNode = 0;
}	

void OutPortTmpl<Audio>::Attach(Node<Audio>& node)
{
	mpData = 0;
	mpNode = &node;
	mpRegion = node.NewWriter(this, Hop(), Length());
}	

void OutPortTmpl<Audio>::Attach(OutPortTmpl<Audio>& port)
{
	if (port.mpNode)
		Attach(*port.mpNode);
	else
		Attach(*port.mpData);
}	

Audio &OutPortTmpl<Audio>::GetData()	
{ 
	CLAM_ASSERT(mpData || (mpNode && mpRegion),
				"OutPortTmpl::GetData(): No data atached to the port.");
	if (mpNode) {
		mpNode->GetAndActivate(mpRegion,mWrapper);
		mpData = &mWrapper;
	}
	return *mpData;
}

void OutPortTmpl<Audio>::LeaveData()	
{
	if (mpNode)
		mpNode->LeaveAndAdvance(mpRegion);
}

void OutPortTmpl<Audio>::Accept(DataVisitor& v)
{
	v.Visit(*mpData);
}

inline ProcessingData* OutPortTmpl<Audio>::GetProcessingData()
{
	if (IsAttached())
	{
		return mpData; 
	}
	return 0;
}

	
inline NodeBase* OutPortTmpl<Audio>::GetNode()
{
	return mpNode;
}

inline bool OutPortTmpl<Audio>::IsAttached()
{
	return  mpData || mpNode ;
}

inline bool OutPortTmpl<Audio>::IsReadyForWriting()
{
	if( !GetNode() ) //only is "ready" when  Port attached to Node
		return false;

	return mpNode->CanActivateRegion(*mpRegion);
}

inline void OutPortTmpl<Audio>::Unattach()
{
	if( !IsAttached() )
		return;

	if(mpNode)
	{
		mpNode->UnattachAll();
		delete mpRegion;
		delete mpNode;
		mpRegion = 0;
		mpNode = 0;
	}	
	mpData = 0;
}


inline bool OutPortTmpl<Audio>::IsConnectableTo(InPort & in)
{
	return ((dynamic_cast< InPortTmpl<Audio>* >(&in)) != 0);
}


} // namespace CLAM

#endif
