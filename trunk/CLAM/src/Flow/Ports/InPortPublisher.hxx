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

#ifndef __InPortPublisher_hxx__
#define __InPortPublisher_hxx__

#include "InPort.hxx"

namespace CLAM
{

template<typename Token>
class InPortPublisher : public InPortBase
{
	typedef InPort<Token> ProperInPort;	
	
	/** Made private since Size and Hop have no meaning for an InPortPublisher  */
	int GetSize()	{ return 0; }
	/** Made private since Size and Hop have no meaning for an InPortPublisher  */
	void SetSize(int newSize) {}
	/** Made private since Size and Hop have no meaning for an InPortPublisher  */
	int GetHop() { return 0; }
	/** Made private since Size and Hop have no meaning for an InPortPublisher  */
	void SetHop(int newHop)	{}
	
public:
	typedef std::list< ProperInPort * > ProperInPortsList;

	InPortPublisher( const std::string & name = "unnamed in port", Processing * proc = 0 )
		: InPortBase( name, proc )
	{
	}
	
	virtual ~InPortPublisher()
	{	
		if ( GetVisuallyConnectedOutPort() )
			Disconnect();
	}

	void PublishInPort( InPortBase & in )
	{
		try
		{
			ConcretePublishInPort( dynamic_cast<ProperInPort&>(in) );
		} catch (...) // could be std::bad_cast ?
		{
			CLAM_ASSERT( false,
			"InPortPublisher<Token>::PublishInPort coudn't connect to outPort "
			"because was not templatized by the same Token type as InPortPublisher" );
		}

	}
	//why not pass InPortBase? still not needed to call from the "generic" interface
	void UnPublishInPort( ProperInPort& in )
	{
		mPublishedInPortsList.remove(&in);
	}

	void ConcretePublishInPort( ProperInPort & in )
	{
		mPublishedInPortsList.push_back( &in );
	}
	
	
	
	bool CanConsume()
	{
		typename ProperInPortsList::iterator it;
		for(it=mPublishedInPortsList.begin(); it!=mPublishedInPortsList.end(); it++)
			if(!(*it)->CanConsume())
				return false;
		return true;	
	}
	

	typename ProperInPortsList::iterator BeginPublishedInPortsList()
	{
		return mPublishedInPortsList.begin();
	}
	
	typename ProperInPortsList::iterator EndPublishedInPortsList()
	{
		return mPublishedInPortsList.end();
	}

	/** Do nothing, since a publisher itself don't have any region */
	void UnAttachRegion() 
	{
		SetVisuallyConnectedOutPort( 0 );
		typename ProperInPortsList::iterator it;
		for(it=mPublishedInPortsList.begin(); it!=mPublishedInPortsList.end(); it++)
		{
			(*it)->UnAttachRegion();
		}
	}
	
	bool IsPublisherOf( InPortBase& in)
	{	
		// BIG TODO: go in-depth (search for publisher-publiser-inport)
		typename ProperInPortsList::iterator it;
		for(it=mPublishedInPortsList.begin(); it!=mPublishedInPortsList.end(); it++)
		{
			if( *it == &in)
				return true;
		}
		return false;
	}
	virtual const std::type_info & GetTypeId() const 
	{
		return typeid(Token);
	};
 protected:

	ProperInPortsList mPublishedInPortsList;
};

} // namespace CLAM

#endif // __InPortPublisher_hxx__

