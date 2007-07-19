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

#ifndef __InPort_hxx__
#define __InPort_hxx__

#include "WritingRegion.hxx"
#include "ReadingRegion.hxx"
#include <string>

namespace CLAM
{
	
class OutPortBase; 
class Processing;

class InPortBase
{
public:
	InPortBase( const std::string & name = "unnamed in port", Processing * proc = 0 );
	virtual ~InPortBase();
	OutPortBase * GetVisuallyConnectedOutPort(); 
	void SetVisuallyConnectedOutPort( OutPortBase* );
	const std::string & GetName();
	bool HasProcessing();
	Processing * GetProcessing();
	virtual bool CanConsume()=0;
	virtual int GetSize()=0;
	virtual void SetSize(int newSize)=0;
	virtual int GetHop()=0;
	virtual void SetHop(int newHop)=0;
	virtual void UnAttachRegion()=0;
	void Disconnect();	
	virtual bool IsPublisherOf( InPortBase& ) { return false; }
	virtual const std::type_info& GetTypeId() const = 0;
protected:
	OutPortBase * mVisuallyConnectedOutPort;
	std::string mName;
	Processing * mProcessing;
};


template<typename Token>
class InPort : public InPortBase
{
	typedef WritingRegion<Token> ProperWritingRegion;
	typedef typename ProperWritingRegion::ProperReadingRegion ProperReadingRegion;

public:
	InPort( const std::string & name = "unnamed in port", Processing * proc = 0 );
	virtual ~InPort();

	// XR: BIG TODO: make this method const!
	/*const*/ Token & GetData(int offset=0);
	
	
	void SetSize( int newSize );
	int GetSize();
	int GetHop();
	void SetHop( int hop );
	void Consume();
	bool CanConsume();

	/**
	 *  This method is intended to be used only for the OutPort. A user shouldn't call it directly. 
	 *  Instead , use ConnectToIn method in OutPortBase.
	 */
	void AttachRegionToOutPort( OutPortBase * out, ProperWritingRegion & writer );
	/**
	 *  This method is intended to be used only for the OutPort. A user shouldn't call it directly. 
	 *  Instead , use DisconnectFromIn method in OutPortBase.
	 */
	void UnAttachRegion();
	virtual const std::type_info & GetTypeId() const 
	{
		return typeid(Token);
	};
protected:

	ProperReadingRegion mRegion;
};


/////// Implementation ////////

template<class Token>
InPort<Token>::InPort( const std::string & name, Processing * proc )
	: InPortBase( name,proc )
{
}

template<class Token> 
InPort<Token>::~InPort()
{
	if(mVisuallyConnectedOutPort)
		Disconnect();
}

template<class Token>
// XR BIG TODO: make this method const!
/*const*/ Token & InPort<Token>::GetData( int offset )
{
	return mRegion[offset];
}

template<class Token>
void InPort<Token>::SetSize( int newSize )
{
	mRegion.Size( newSize );
}

template<class Token>
int InPort<Token>::GetSize()
{
	return mRegion.Size();
}

template<class Token>
int InPort<Token>::GetHop()
{
	return mRegion.Hop();
}

template<class Token>
void InPort<Token>::SetHop( int hop )
{
	mRegion.Hop(hop);
}

template<class Token>
void InPort<Token>::Consume() 
{
	mRegion.Consume();
}

template<class Token>
bool InPort<Token>::CanConsume()
{
	return mRegion.CanConsume();
}

template<class Token>
void InPort<Token>::AttachRegionToOutPort( OutPortBase * out, ProperWritingRegion & writer )
{
	writer.LinkRegions( mRegion );
	mVisuallyConnectedOutPort = out;
}

template<class Token>
void InPort<Token>::UnAttachRegion()
{
	CLAM_DEBUG_ASSERT( mVisuallyConnectedOutPort, "InPort<T>::UnAttachRegion() - InPort is not connected" );
	mRegion.ProducerRegion()->RemoveRegion( mRegion );
	mVisuallyConnectedOutPort = 0;
}

} // namespace CLAM

#endif // __InPort_hxx__

