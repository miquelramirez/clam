#ifndef _OutPortTmpl_hxx_
#define _OutPortTmpl_hxx_

#include "OutPort.hxx"
#include "InPortTmpl.hxx"
#include "Node.hxx"
#include "ProcessingData.hxx"
#include "WriteStreamRegion.hxx"
#include "Processing.hxx"

#include <string>

namespace CLAM
{

template<class T>
class OutPortTmpl : public OutPort
{
	WriteStreamRegion *mpRegion;
	Node<T> *mpNode;
	Array<T> mData;
public:
	
	inline OutPortTmpl(const std::string &n, Processing *o, int length, int hop = 0);
	inline ~OutPortTmpl();
	inline T &GetData();
	inline void LeaveData();
	void Attach(ProcessingData& data);
	void Attach( NodeBase&);
	inline void Attach(T& data);
	inline void Attach(Node<T> &n);
	inline void Attach(OutPortTmpl<T> &p); // For composites
	inline void Accept(DataVisitor&);

	ProcessingData* GetProcessingData();
	NodeBase* GetNode();
	bool IsAttached();
	bool IsReadyForWriting();
	void Unattach();
	bool IsConnectableTo(InPort & );
};

// Implementation
//-------------------------------------------------------------------------------


template<class T>
inline OutPortTmpl<T>::OutPortTmpl(const std::string &n,
                            Processing *o,
                            int length,
                            int hop)
	: OutPort(n,o,length),
	  mpRegion(0),
	  mpNode(0)
{
	o->PublishOutPort(this);
}

template<class T>
inline OutPortTmpl<T>::~OutPortTmpl()
{
	if (mpRegion)
		delete mpRegion;
}

template<class T>
inline void OutPortTmpl<T>::Attach(ProcessingData& data)
{
	try { 
		Attach(dynamic_cast<T&>(data));
	}
	// the exception catched should be std::bad_cast instead of std::exception. 
	// to fix when VC6 is no longer supported
	catch (std::exception){
		CLAM_ASSERT(false,"You are trying to attach a processing data that is not suitable for this port");
	}
}	

template<class T>
inline void OutPortTmpl<T>::Attach(T& data)
{
	mData.SetPtr(&data,1);
}	

template<class T>
inline void OutPortTmpl<T>::Attach(Node<T>& node)
{
	mData.SetPtr(0);
	mpNode = &node;
	mpRegion = node.NewWriter(this, Hop(), Length());
}

template<class T>
inline void OutPortTmpl<T>::Attach(OutPortTmpl<T>& port)
{
	if (port.mpNode)
		Attach(*port.mpNode);
	else
		Attach(*port.mData);
}	

template<class T>
inline T &OutPortTmpl<T>::GetData()	
{ 
	CLAM_ASSERT(mData.Size() || (mpNode && mpRegion),
	            "OutPortTmpl::GetData(): No data atached to the port.");
	if (mpNode)
		mpNode->GetAndActivate(mpRegion,mData);
	return mData[0];
}

template<class T>
inline void OutPortTmpl<T>::Attach( NodeBase& node)
{
	try {
		Attach( dynamic_cast< Node<T>& >(node) );
	}
	// the exception catched should be std::bad_cast instead of std::exception. 
	// to fix when VC6 is no longer supported
	catch (std::exception) {
		CLAM_ASSERT(false,"You are trying to attach a node that is not suitable for this port");
	}
}

template<class T>
inline void OutPortTmpl<T>::LeaveData()	
{
	if (mpNode)
		mpNode->LeaveAndAdvance(mpRegion);
}

template<class T>
inline void OutPortTmpl<T>::Accept(DataVisitor& v)
{
	int i;
	for (i=0; i<mData.Size(); i++)
		v.Visit(mData[i]);
}

template<class T>
inline ProcessingData* OutPortTmpl<T>::GetProcessingData()
{
	if ( mData.Size()>0 )
	{
		return &(mData[0]);
	}
	return 0;
}

template<class T>
inline bool OutPortTmpl<T>::IsAttached()
{
	return  mData.Size()>0 || mpNode ;
}

template<class T>
inline bool OutPortTmpl<T>::IsReadyForWriting()
{
	if( !GetNode() ) //only is "ready" when  Port attached to Node
		return false;
	
	return mpNode->CanActivateRegion(*mpRegion);
}


template<class T>
inline void OutPortTmpl<T>::Unattach()
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
	mData.SetPtr(NULL);
}

	
template<class T>
inline NodeBase* OutPortTmpl<T>::GetNode()
{
	return mpNode;
}

template <class T>
inline bool OutPortTmpl<T>::IsConnectableTo(InPort & in)
{
	return ((dynamic_cast< InPortTmpl<T>* >(&in)) != 0);
}


} // namespace CLAM

//\todo revise
// Port specializations
#include "AudioOutPortTmpl.hxx"

#endif
