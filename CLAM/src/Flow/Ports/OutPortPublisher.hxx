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

#ifndef __OutPortPublisher_hxx__
#define __OutPortPublisher_hxx__

#include "OutPort.hxx"

namespace CLAM
{

template< typename Token >
class OutPortPublisher : public OutPortBase
{
	typedef OutPort<Token> ProperOutPort;
public:
	OutPortPublisher( const std::string & name = "unnamed out port publisher", Processing * proc = 0 )
		: OutPortBase( name, proc ), mPublishedOutPort(0)
	{
		
	}

	virtual ~OutPortPublisher()
	{
		//disconnect published port
		if (mPublishedOutPort) 
		{
			mPublishedOutPort->UnsetPublisher();
			mPublishedOutPort->DisconnectFromAll();
			mPublishedOutPort = 0;
		}
		//disconnect visually connected inports
		for (InPortsList::iterator it = BeginVisuallyConnectedInPorts();
			it != EndVisuallyConnectedInPorts();
			it++ )
		(*it)->SetVisuallyConnectedOutPort(0);
		
	}

	void DisconnectFromAll()
	{
		CLAM_DEBUG_ASSERT( mPublishedOutPort != 0, "OutPortPublisher - no out port published" );
		mPublishedOutPort->DisconnectFromAll();
		mVisuallyConnectedPorts.clear();
	}
	void ConnectToIn( InPortBase& in)
	{
		CLAM_DEBUG_ASSERT( mPublishedOutPort != 0, "OutPortPublisher - no out port published" );
		mPublishedOutPort->ConnectToIn( in );
		in.SetVisuallyConnectedOutPort( this );
		mVisuallyConnectedPorts.push_back(&in);
	}
	
	void PublishOutPort( OutPortBase & out )
	{
		try
		{
			ConcretePublishOutPort( dynamic_cast<ProperOutPort&>(out) );
		} catch (...) // could be std::bad_cast ?
		{
			CLAM_ASSERT( false,
			"OutPortPublisher<Token>::PublishOutPort coudn't connect to outPort "
			"because was not templatized by the same Token type as OutPortPublisher" );
		}

	}
	void UnpublishOutPort()
	{
		mPublishedOutPort = 0;
	}
	void ConcretePublishOutPort( ProperOutPort & out )
	{
		mPublishedOutPort = &out;
		out.SetPublisher( *this );
	}
	
	void DisconnectFromIn( InPortBase& in)
	{
		CLAM_ASSERT(mPublishedOutPort, "OutPortPublisher::DisconnectFromIn() A published port is missing. "
				"Consider using the method PublishOutPort( OutPortBase& out) ");
		mPublishedOutPort->DisconnectFromIn( in );
		mVisuallyConnectedPorts.remove(&in);
	}
	
	bool IsConnectableTo(InPortBase & in)
	{
		CLAM_ASSERT(mPublishedOutPort, "OutPortPublisher::IsConnectableTo() A published port is missing. "
				"Consider using the method PublishOutPort( OutPortBase& out) ");
		return mPublishedOutPort->IsConnectableTo( in );
	}
	
	bool IsVisuallyConnectedTo(InPortBase & in)
	{
		CLAM_ASSERT(mPublishedOutPort, "OutPortPublisher:IsVisuallyConnectedTo() A published port is missing. "
				"Consider using the method PublishOutPort( OutPortBase& out) ");
		return mPublishedOutPort->IsVisuallyConnectedTo( in );
	}
	
	Token & GetData(int offset=0)
	{
		CLAM_ASSERT(mPublishedOutPort, "OutPortPublisher::GetData() A published port is missing. "
				"Consider using the method PublishOutPort( OutPortBase& out) ");
		return mPublishedOutPort->GetData( offset );
	}
	
	int GetSize()
	{
		CLAM_ASSERT(mPublishedOutPort, "OutPortPublisher::GetSize() A published port is missing. "
				"Consider using the method PublishOutPort( OutPortBase& out) ");
		return mPublishedOutPort->GetSize();
	}
	
	void SetSize(int newSize)
	{
		CLAM_ASSERT(mPublishedOutPort, "OutPortPublisher::SetSize() A published port is missing. "
				"Consider using the method PublishOutPort( OutPortBase& out) ");
		mPublishedOutPort->SetSize( newSize );
	}
	
	int GetHop()
	{
		CLAM_ASSERT(mPublishedOutPort, "OutPortPublisher::GetHop() A published port is missing. "
				"Consider using the method PublishOutPort( OutPortBase& out) ");
		return mPublishedOutPort->GetHop();
	}
	
	void SetHop(int newHop)
	{
		CLAM_ASSERT(mPublishedOutPort, "OutPortPublisher::SetHop() A published port is missing. "
				"Consider using the method PublishOutPort( OutPortBase& out) ");
		mPublishedOutPort->SetHop( newHop );
	}

	
	bool CanProduce()
	{
		CLAM_ASSERT(mPublishedOutPort, "OutPortPublisher::CanProduce() A published port is missing. "
				"Consider using the method PublishOutPort( OutPortBase& out) ");
		return mPublishedOutPort->CanProduce();
	}	
	
	void CenterEvenRegions()
	{
		CLAM_ASSERT(mPublishedOutPort, "OutPortPublisher::CenterEvenRegions() A published port is missing. "
				"Consider using the method PublishOutPort( OutPortBase& out) ");
		mPublishedOutPort->CenterEvenRegions();
	}
	Token & GetLastWrittenData( int offset = 0 )
	{
		return OutPort<Token>::GetLastWrittenData( *mPublishedOutPort, offset );
	}

	static Token & GetLastWrittenData( OutPortBase &, int offset = 0);
	virtual const std::type_info & GetTypeId() const 
	{
		return typeid(Token);
	};

	
protected:
	ProperOutPort * mPublishedOutPort;
};

template<class Token>
Token & OutPortPublisher<Token>::GetLastWrittenData( OutPortBase & out, int offset )
{
	try
	{
		OutPortPublisher<Token>& concreteOut = dynamic_cast< OutPortPublisher<Token>& >(out);
		return concreteOut.GetLastWrittenData( offset );
	}
	catch(...)
	{
		CLAM_ASSERT( false, "OutPortPublisher<Token>::DumpDataWithLastToken - Passed an outport of wrong type" );
	}
	return *(Token *)NULL;
		
}




} // namespace CLAM

#endif // __OutPortPublisher_hxx__

