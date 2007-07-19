/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __OutPort_hxx__
#define __OutPort_hxx__

#include "WritingRegion.hxx"
#include <list>
#include <string>
#include "InPort.hxx"
#include "InPortPublisher.hxx"

namespace CLAM
{

class Processing;

class OutPortBase
{
public:
	typedef std::list<InPortBase*> InPortsList;
	OutPortBase( const std::string & name = "unnamed out port", Processing * proc = 0 );
	virtual ~OutPortBase();
	const std::string & GetName();
	Processing * GetProcessing();
	InPortsList::iterator BeginVisuallyConnectedInPorts();
	InPortsList::iterator EndVisuallyConnectedInPorts();
	
	virtual void ConnectToIn(InPortBase& in) = 0;
	virtual void DisconnectFromIn(InPortBase & in) = 0;
	virtual void DisconnectFromAll()=0;
	virtual bool IsVisuallyConnectedTo(InPortBase & in) = 0;
	virtual bool IsConnectableTo(InPortBase & ) = 0;
	virtual bool CanProduce()=0;
	virtual int GetSize()=0;
	virtual void SetSize(int newSize)=0;
	virtual int GetHop()=0;
	virtual void SetHop(int newHop)=0;
	bool HasConnections(){ return mVisuallyConnectedPorts.size()!=0; }
	virtual void CenterEvenRegions()=0;
	void SetPublisher( OutPortBase& publisher); 
	void UnsetPublisher(); 
	virtual void UnpublishOutPort() =0;
	virtual const std::type_info & GetTypeId() const = 0;
protected:
	InPortsList mVisuallyConnectedPorts;	
	std::string mName;
	Processing * mProcessing;
	OutPortBase* mPublisher;
};


template<typename Token>
class OutPort : public OutPortBase
{
	typedef OutPort<Token> ProperOutPort;
	typedef InPort<Token> ProperInPort;
	typedef WritingRegion<Token> ProperWritingRegion;
public:
	OutPort( const std::string & name = "unnamed out port", Processing * proc = 0 );
	virtual ~OutPort();

	void DisconnectFromAll();
	void ConnectToIn( InPortBase& in);
	void ConnectToConcreteIn(InPort<Token>& in);
	void DisconnectFromIn( InPortBase& in);
	void DisconnectFromConcreteIn(InPort<Token>& in);
	bool IsConnectableTo(InPortBase & in);
	bool IsVisuallyConnectedTo(InPortBase & in);
	bool IsPhysicallyConnectedToIn(InPort<Token>& ); 
	InPortPublisher<Token>* GetPublisherContaining(InPort<Token>&);

	Token & GetData(int offset=0);	
	void SetSize( int newSize );
	int GetSize();
	int GetHop();
	void SetHop( int hop );	
	void Produce();
	bool CanProduce();
	void CenterEvenRegions();
	
	static Token & GetLastWrittenData( OutPortBase &, int offset = 0);
	void UnpublishOutPort() {} 
	virtual const std::type_info & GetTypeId() const
        {
                return typeid(Token);
        };

protected:	
	bool TryConnectToPublisher( InPortBase & in );
	bool TryConnectToConcreteIn( InPortBase & in );

	bool TryDisconnectFromPublisher( InPortBase & in );
	bool TryDisconnectFromConcreteIn( InPortBase & in );

	Token & GetLastWrittenData( int offset = 0 );
	ProperWritingRegion mRegion;

};

template<class Token>
OutPort<Token>::OutPort( const std::string & name, Processing * proc )
	: OutPortBase(name,proc)
{
}

template<class Token>
void OutPort<Token>::DisconnectFromAll()
{
	InPortsList::iterator it = mVisuallyConnectedPorts.begin();
	for( it=BeginVisuallyConnectedInPorts(); it!=EndVisuallyConnectedInPorts(); it++ )	
	{ 
		(*it)->UnAttachRegion();
	}
	mVisuallyConnectedPorts.clear();
}

template<class Token>
OutPort<Token>::~OutPort()
{
	DisconnectFromAll();
}

template<class Token>
bool OutPort<Token>::TryConnectToConcreteIn( InPortBase & in )
{
	try
	{
		ConnectToConcreteIn( dynamic_cast<ProperInPort&>(in) );
	}
	catch(std::bad_cast & e)
	{
		return false;
	}
	return true;
}

template<class Token>
bool OutPort<Token>::TryConnectToPublisher( InPortBase & in )
{
	InPortPublisher<Token> * publisher =  dynamic_cast< InPortPublisher<Token> *>(&in);
	if (!publisher) 
		return false;
	
	typename InPortPublisher<Token>::ProperInPortsList::iterator it;
	mVisuallyConnectedPorts.push_back( &in );
	for( it=publisher->BeginPublishedInPortsList(); it!=publisher->EndPublishedInPortsList(); it++)
		(*it)->AttachRegionToOutPort(this, mRegion );

	in.SetVisuallyConnectedOutPort(this);

	return true;
}

template<class Token>
void OutPort<Token>::ConnectToIn( InPortBase& in)
{
	bool successfullConnection;
	successfullConnection = TryConnectToConcreteIn( in ) || TryConnectToPublisher( in );
	CLAM_ASSERT( successfullConnection,
		     "OutPort<Token>::connectToIn coudn't connect to inPort "
   		     "because was not templatized by the same Token type as outPort" );
}

template<class Token>
void OutPort<Token>::ConnectToConcreteIn(InPort<Token>& in)
{
	CLAM_ASSERT( !in.GetVisuallyConnectedOutPort(), "OutPort<Token>::ConnectToConcreteIn - Trying to connect an inport "
						    "already connected to another out port" );
	CLAM_ASSERT( !IsVisuallyConnectedTo(in), "OutPort<Token>::ConnectToConcreteIn - Trying to connect an in port "
					"already connected to this out port" );
	mVisuallyConnectedPorts.push_back(&in);
	in.AttachRegionToOutPort(this, mRegion );
}

template<class Token>
void OutPort<Token>::DisconnectFromIn( InPortBase& in)
{
	bool successfullDisconnection;
	successfullDisconnection = TryDisconnectFromConcreteIn( in ) || TryDisconnectFromPublisher( in );
	CLAM_ASSERT( successfullDisconnection,
		     "OutPort<Token>::DisconnectFromIn coudn't discconnect from inPort "
   		     "because was not templatized by the same Token type as outPort" );
}

template<class Token>
bool OutPort<Token>::TryDisconnectFromConcreteIn( InPortBase & in )
{
	ProperInPort * concreteIn = dynamic_cast<ProperInPort*>(&in);
	if (!concreteIn)
		return false;
	
	DisconnectFromConcreteIn( *concreteIn );
	return true;
}

template<class Token>
bool OutPort<Token>::TryDisconnectFromPublisher( InPortBase & in )
{
	InPortPublisher<Token> *publisher = dynamic_cast<InPortPublisher<Token> *>(&in);
	if (!publisher)
		return false;
	
	typename InPortPublisher<Token>::ProperInPortsList::iterator it;
	mVisuallyConnectedPorts.remove( &in );
	for( it=publisher->BeginPublishedInPortsList(); it!=publisher->EndPublishedInPortsList(); it++)
	{
		if( (*it)->GetVisuallyConnectedOutPort())
			(*it)->UnAttachRegion();
	}
	return true;
}

template<class Token>
void OutPort<Token>::DisconnectFromConcreteIn(InPort<Token>& in)
{
	CLAM_ASSERT( IsVisuallyConnectedTo(in) || IsPhysicallyConnectedToIn(in), 
			"OutPort::DisconnectFromConcreteIn() in port is not directly neither physically connected" );
	if (IsVisuallyConnectedTo(in) )
	{
		// is directly connected
		mVisuallyConnectedPorts.remove(&in);
	}
	else // then IsPhysicallyConnected()
	{
		InPortPublisher<Token> *pub = GetPublisherContaining(in);
		CLAM_DEBUG_ASSERT(0!=pub, "in port should be published");
		pub->UnPublishInPort(in);
	}
	in.UnAttachRegion();
}

template<class Token>
Token & OutPort<Token>::GetData(int offset )
{
	return mRegion[offset];
}

template<class Token>
void OutPort<Token>::SetSize( int newSize )
{
	mRegion.Size( newSize );
}

template<class Token>
int OutPort<Token>::GetSize()
{
	return mRegion.Size();
}

template<class Token>
int OutPort<Token>::GetHop()
{
	return mRegion.Hop();
}

template<class Token>
void OutPort<Token>::SetHop( int hop )
{
	mRegion.Hop(hop);
}

template<class Token>
void OutPort<Token>::Produce()
{
	mRegion.Produce();
}

template<class Token>
bool OutPort<Token>::CanProduce()
{
	return mRegion.CanProduce();
}

template<class Token>
bool OutPort<Token>::IsConnectableTo(InPortBase & in)
{	
	return ( dynamic_cast< ProperInPort* >(&in) || 
		dynamic_cast< InPortPublisher<Token> *>(&in));
}

template<class Token>
bool OutPort<Token>::IsPhysicallyConnectedToIn(InPort<Token>& in)
{ 
	if (IsVisuallyConnectedTo(in))
		return true;
	
	return ( 0!=GetPublisherContaining(in) );
	
}

template<class Token>
InPortPublisher<Token>* OutPort<Token>::GetPublisherContaining(InPort<Token>& in)
{
	
	InPortPublisher<Token> *result=0;
	InPortsList::iterator it;
	for( it=mVisuallyConnectedPorts.begin(); it!=mVisuallyConnectedPorts.end(); it++ )
		if ( (*it)->IsPublisherOf(in) )
		{
			result=dynamic_cast<InPortPublisher<Token> *>(*it);
			CLAM_DEBUG_ASSERT( result, "OutPort::GetPublisherContaining(in) IsPublisher but dynamic_cast failed");
			return result;
		}

	return 0;
		
}

template<class Token>
bool OutPort<Token>::IsVisuallyConnectedTo(InPortBase & in)
{
	InPortsList::iterator it;
	for( it=mVisuallyConnectedPorts.begin(); it!=mVisuallyConnectedPorts.end(); it++ )
		if(*it == &in) return true;
	return false;
}
	
template<class Token>
void OutPort<Token>::CenterEvenRegions()
{
	mRegion.CenterEvenRegions();
}

template<class Token>
Token & OutPort<Token>::GetLastWrittenData( int offset )
{
	CLAM_DEBUG_ASSERT( 0 <= offset, "OutPort<Token>::GetLastWrittenData - Index under bounds" );
	CLAM_DEBUG_ASSERT( offset <= GetSize(), "OutPort<Token>::GetLastWrittenData - Index over bounds" );
	return mRegion.GetLastWrittenData( offset );
}

template<class Token>
Token & OutPort<Token>::GetLastWrittenData( OutPortBase & out, int offset )
{
	try
	{
		OutPort<Token>& concreteOut = dynamic_cast< OutPort<Token>& >(out);
		return concreteOut.GetLastWrittenData( offset );
	}
	catch(...)
	{
		CLAM_ASSERT( false, "OutPort<Token>::GetLastWrittenData - Passed an outport of wrong type" );
	}
	return *(Token *)NULL;
		
}

} // namespace CLAM

#endif // __OutPort_hxx__

