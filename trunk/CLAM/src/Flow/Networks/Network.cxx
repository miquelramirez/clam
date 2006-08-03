/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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


#include "Network.hxx"
#include "FlowControl.hxx"
#include <algorithm>
#include "ProcessingDefinitionAdapter.hxx"
#include "ConnectionDefinitionAdapter.hxx"
#include "Factory.hxx"


namespace CLAM
{	
	typedef Factory<CLAM::Processing> ProcessingFactory;

	Network::Network() :
		mName("Unnamed Network"),
		mFlowControl(0)
	{}
	
	Network::~Network()
	{
		//std::cerr <<" *\t\t\t~NETWORK"<<std::endl;
		Clear();
		if (mFlowControl) delete mFlowControl;
	}

	void Network::StoreOn( Storage & storage) const
	{
		// Storing an standard library string as an attribute
		std::string name = mName;
		XMLAdapter<std::string> strAdapter( name, "id");
		storage.Store(strAdapter);

		ProcessingsMap::const_iterator it;
		for(it=BeginProcessings();it!=EndProcessings();it++)
		{
			Processing * proc = it->second;
			std::string name(it->first);
			ProcessingDefinitionAdapter procDefinitionAdapter(proc, name);
			XMLComponentAdapter procComponentAdapter(procDefinitionAdapter, "processing", true);
			storage.Store(procComponentAdapter);
		}

		// second iteration to store ports. 
		// XR: maybe it should be all in one iteration but this way
		// the xml file is clearer.

		for(it=BeginProcessings();it!=EndProcessings();it++)
		{
			std::string name(it->first);
			Processing * proc = it->second;
			OutPortRegistry::Iterator itOutPort;
			for (itOutPort=proc->GetOutPorts().Begin(); 
			     itOutPort!=proc->GetOutPorts().End(); 
			     itOutPort++)
			{
				if (!(*itOutPort)->HasConnections())
					continue;
	
				std::string outPortName(name);
				outPortName += ".";
				outPortName += (*itOutPort)->GetName();

				NamesList namesInPorts = GetInPortsConnectedTo(outPortName);
				NamesList::iterator namesIterator;
				for(namesIterator=namesInPorts.begin();
				    namesIterator!=namesInPorts.end();
				    namesIterator++)
				{
					ConnectionDefinitionAdapter connectionDefinitionAdapter( outPortName, *namesIterator );
					XMLComponentAdapter connectionComponentAdapter(connectionDefinitionAdapter, "port_connection", true);
					storage.Store(connectionComponentAdapter);
				}
			}
		}

		for(it=BeginProcessings();it!=EndProcessings();it++)
		{
			std::string name(it->first);
			Processing * proc = it->second;
			OutControlRegistry::Iterator itOutControl;
			for (itOutControl=proc->GetOutControls().Begin(); 
			     itOutControl!=proc->GetOutControls().End(); 
			     itOutControl++)
			{
				std::string outControlName(name);
				outControlName += ".";
				outControlName += (*itOutControl)->GetName();

				NamesList namesInControls = GetInControlsConnectedTo(outControlName);
				NamesList::iterator namesIterator;
				for(namesIterator=namesInControls.begin();
				    namesIterator!=namesInControls.end();
				    namesIterator++)
				{
					ConnectionDefinitionAdapter connectionDefinitionAdapter( outControlName, *namesIterator );
					XMLComponentAdapter connectionComponentAdapter(connectionDefinitionAdapter, "control_connection", true);
					storage.Store(connectionComponentAdapter);
				}
			}
		}


	}

	void Network::LoadFrom( Storage & storage)
	{
		XMLAdapter<std::string> strAdapter( mName, "id");
		storage.Load(strAdapter);

		while(1)
		{
			ProcessingDefinitionAdapter procDefinitionAdapter;
			XMLComponentAdapter procComponentAdapter(procDefinitionAdapter, "processing", true);
			if(storage.Load(procComponentAdapter) == false)
				break;
			
			AddProcessing(procDefinitionAdapter.GetName(), procDefinitionAdapter.GetProcessing()); 
		}

		while(1)
		{
			ConnectionDefinitionAdapter connectionDefinitionAdapter;
			XMLComponentAdapter connectionComponentAdapter(connectionDefinitionAdapter, "port_connection", true);
			if(storage.Load(connectionComponentAdapter)==false)
				break;
			ConnectPorts( connectionDefinitionAdapter.GetOutName(), connectionDefinitionAdapter.GetInName() );			
		}

		while(1)
		{
			ConnectionDefinitionAdapter connectionDefinitionAdapter;
			XMLComponentAdapter connectionComponentAdapter(connectionDefinitionAdapter, "control_connection", true);
			if(storage.Load(connectionComponentAdapter)==false)
				break;
			ConnectControls( connectionDefinitionAdapter.GetOutName(), connectionDefinitionAdapter.GetInName() );			
		}

	}

	/** Gets the ownership of the FlowControl passed. So it will be deleted by the destructor */
	void Network::AddFlowControl(FlowControl* flowControl)
	{

		if (mFlowControl) delete mFlowControl;
		mFlowControl = flowControl;
		mFlowControl->AttachToNetwork(this);
	}

	Processing& Network::GetProcessing( const std::string & name ) const
	{
		CLAM_ASSERT( HasProcessing(name), "No Processing with the given name");

		ProcessingsMap::const_iterator it = mProcessings.find( name );
		return *it->second;
	}

	void Network::AssertFlowControlNotNull() const
	{
		CLAM_ASSERT( 
			mFlowControl, 
			"the Network should have a FlowControl. Use Network::AddFlowControl(FlowControl*)");
	}

	void Network::AddProcessing( const std::string & name, Processing* proc)
	{
		AssertFlowControlNotNull();

		// returns false if the key was repeated.
		if (!mProcessings.insert( ProcessingsMap::value_type( name, proc ) ).second )
			CLAM_ASSERT(false, "Network::AddProcessing() Trying to add a processing with a repeated name (key)" );

		mFlowControl->ProcessingAddedToNetwork(*proc);
	}

	void Network::AddProcessing( const std::string & name, const std::string & key)
	{
		AssertFlowControlNotNull();

		Processing * proc = ProcessingFactory::GetInstance().CreateSafe( key );

		// returns false if the key was repeated.
		if (!mProcessings.insert( ProcessingsMap::value_type( name, proc ) ).second )
			CLAM_ASSERT(false, "Network::AddProcessing() Trying to add a processing with a repeated name (key)" );

		mFlowControl->ProcessingAddedToNetwork(*proc);
	}

	std::string Network::AddProcessing( const std::string & key) // returns the name that was used so the same one can be used when calling CreateProcessingController (hack)
	{
		AssertFlowControlNotNull();

		std::string name = GetUnusedName( key ); // this won't be needed in the future
		
		Processing * proc = ProcessingFactory::GetInstance().CreateSafe( key );

		// returns false if the key was repeated.
		if (!mProcessings.insert( ProcessingsMap::value_type( name , proc ) ).second )
			CLAM_ASSERT(false, "Network::AddProcessing() Trying to add a processing with a repeated name (key)" );

		mFlowControl->ProcessingAddedToNetwork(*proc);

		return name;
	}

	std::string Network::GetUnusedName( const std::string& prefix )
	{
		std::string name;

		for ( int i = 0; i<9999999; i++ ) // 9999999 is just an arbitrary large value
		{
			std::stringstream tmp; 
			tmp << i;
			name = prefix + "_" + tmp.str(); // pseudo code ofcourse
			if (!this->HasProcessing( name ) ) return name;
		}
		CLAM_ASSERT(false, "All valid id's for given prefix are exhausted");
		return "";
	}


	void Network::RemoveProcessing ( const std::string & name)
	{
		CLAM_ASSERT( mFlowControl, 
			     "Network::RemoveProcessing() - Network should have an attached flow control at this state.");

		ProcessingsMap::const_iterator i = mProcessings.find( name );
		if(i==mProcessings.end())
		{
			std::string msg("Network::RemoveProcessing() Trying to remove a processing that is not included in the network:");
			msg += name;
			CLAM_ASSERT(false, msg.c_str() );
		}
		Processing * proc = i->second;
		mProcessings.erase( name );

		InPortRegistry::Iterator itInPort;
		for(itInPort=proc->GetInPorts().Begin(); itInPort!=proc->GetInPorts().End(); itInPort++)
		{
			if((*itInPort)->GetAttachedOutPort())
				(*itInPort)->Disconnect();
		}

		OutPortRegistry::Iterator itOutPort;
		for(itOutPort=proc->GetOutPorts().Begin(); itOutPort!=proc->GetOutPorts().End(); itOutPort++)
		{
			if((*itOutPort)->HasConnections())
				(*itOutPort)->DisconnectFromAll();
		}

		mFlowControl->ProcessingRemovedFromNetwork(*proc);
		delete proc;		
	}

	bool Network::HasProcessing( const std::string& name ) const
	{
		ProcessingsMap::const_iterator i = mProcessings.find( name );
		return i!=mProcessings.end();
	}
	
	void Network::ConfigureProcessing( const std::string & name, const ProcessingConfig & newConfig )	
	{
		AssertFlowControlNotNull();
		ProcessingsMap::iterator it = mProcessings.find( name );
		Processing * proc = it->second;
		proc->Configure( newConfig );
		mFlowControl->ProcessingConfigured(*proc);
	}

	void Network::ReconfigureAllProcessings()
	{
		ProcessingsMap::iterator it;
		for( it=mProcessings.begin(); it!=mProcessings.end(); it++)
		{
			Processing* proc = it->second;
			proc->Configure( proc->GetConfig() );
		}
	}

	bool Network::ConnectPorts( const std::string & producer, const std::string & consumer )
	{
		AssertFlowControlNotNull();
		mFlowControl->NetworkTopologyChanged();

		OutPortBase & outport = GetOutPortByCompleteName(producer);
		InPortBase & inport = GetInPortByCompleteName(consumer);

		if ( outport.IsDirectlyConnectedTo(inport) ) 
			return false;
			
		if ( !outport.IsConnectableTo(inport) ) //they have different type
			return false;

		if( inport.GetAttachedOutPort())
			return false;

		outport.ConnectToIn( inport );
		return true;
	}

	bool Network::ConnectControls( const std::string & producer, const std::string & consumer )
	{
		OutControl & outcontrol = GetOutControlByCompleteName(producer);
		InControl & incontrol = GetInControlByCompleteName(consumer);

		if ( outcontrol.IsConnectedTo(incontrol) ) 
			return false;
			

		outcontrol.AddLink( &incontrol );
		return true;
	}


	bool Network::DisconnectPorts( const std::string & producer, const std::string & consumer)
	{
		AssertFlowControlNotNull();
		mFlowControl->NetworkTopologyChanged();

		OutPortBase & outport = GetOutPortByCompleteName(producer);
		InPortBase & inport = GetInPortByCompleteName(consumer);

		if ( !outport.IsDirectlyConnectedTo(inport))
			return false;

		outport.DisconnectFromIn( inport );
		return true;
	}

	bool Network::DisconnectControls( const std::string & producer, const std::string & consumer)
	{

		OutControl & outcontrol = GetOutControlByCompleteName(producer);
		InControl & incontrol = GetInControlByCompleteName(consumer);

		if ( !outcontrol.IsConnectedTo( incontrol )) 
			return false;

		outcontrol.RemoveLink( &incontrol );
		return true;
	}

	void Network::DisconnectAllPorts()
	{
		AssertFlowControlNotNull();
		mFlowControl->NetworkTopologyChanged();

		ProcessingsMap::iterator it;
		// pass trough all the processing
		for( it=mProcessings.begin(); it!=mProcessings.end(); it++)
		{
			Processing* proc = it->second;
			InPortRegistry::Iterator iteratorInPorts;

			//unattach all the inports of each processing
			for(iteratorInPorts=proc->GetInPorts().Begin();
			    iteratorInPorts!=proc->GetInPorts().End();
			    iteratorInPorts++)
				(*iteratorInPorts)->Disconnect();
		}
	}


	char Network::NamesIdentifiersSeparator()
	{ 	
		return '.'; 	
	}

	std::size_t Network::PositionOfLastIdentifier( const std::string & str ) 
	{
		std::size_t result = str.find_last_of( NamesIdentifiersSeparator() );
		CLAM_ASSERT( result!=std::string::npos, "Malformed port/control name. It should be ProcessingName.[Port/Control]Name");
		return result;
	}
	std::size_t Network::PositionOfProcessingIdentifier( const std::string& str )
	{
		std::size_t endPos = PositionOfLastIdentifier(str)-1;
		std::size_t	last_ofResult = str.find_last_of( NamesIdentifiersSeparator(), endPos );
		return last_ofResult == std::string::npos ? 0 : last_ofResult+1;
	}

	std::string Network::GetConnectorIdentifier( const std::string& str ) const
	{
		return str.substr( PositionOfLastIdentifier(str)+1 );
	}

	std::string Network::GetProcessingIdentifier( const std::string& str ) const
	{
		std::size_t length = PositionOfLastIdentifier(str)  - PositionOfProcessingIdentifier(str);
		return str.substr( PositionOfProcessingIdentifier(str), length);
	}

	InPortBase & Network::GetInPortByCompleteName( const std::string & name ) const
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetInPorts().Get( GetConnectorIdentifier(name) );
	}

	OutPortBase & Network::GetOutPortByCompleteName( const std::string & name ) const
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetOutPorts().Get( GetConnectorIdentifier(name) );
	}

	InControl & Network::GetInControlByCompleteName( const std::string & name ) const
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetInControls().Get( GetConnectorIdentifier(name) );
	}

	OutControl & Network::GetOutControlByCompleteName( const std::string & name ) const
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetOutControls().Get( GetConnectorIdentifier(name) );
	}

	void Network::Start()
	{
		ProcessingsMap::iterator it;
		for (it=BeginProcessings(); it!=EndProcessings(); it++)
			if (it->second->GetExecState() == Processing::Ready)
				it->second->Start();		
	}
	
	void Network::Stop()
	{
		ProcessingsMap::iterator it;
		for (it=BeginProcessings(); it!=EndProcessings(); it++)
			if (it->second->GetExecState() == Processing::Running)
				it->second->Stop();
	}
	
	void Network::Do()
	{
		AssertFlowControlNotNull();
		mFlowControl->Do();
	}

	void Network::Clear()
	{
		Stop();
		
		while( !mProcessings.empty() )
		{
			//std::cerr << "REMOVING <"<<mProcessings.begin()->first<<">"<<std::endl;
			RemoveProcessing( mProcessings.begin()->first );
		}
	}

	Network::ProcessingsMap::iterator Network::BeginProcessings()
	{
		return mProcessings.begin();
	}

	Network::ProcessingsMap::iterator Network::EndProcessings()
	{
		return mProcessings.end();
	}
	Network::ProcessingsMap::const_iterator Network::BeginProcessings() const
	{
		return mProcessings.begin();
	}

	Network::ProcessingsMap::const_iterator Network::EndProcessings() const
	{
		return mProcessings.end();
	}

	Network::NamesList  Network::GetInPortsConnectedTo( const std::string & producer ) const
	{		
		OutPortBase & out = GetOutPortByCompleteName( producer );
		NamesList consumers;

		if(!out.HasConnections())
			return consumers;

		OutPortBase::InPortsList::iterator it;
		for(it=out.BeginConnectedInPorts(); it!=out.EndConnectedInPorts(); it++)
		{
			std::string completeName(GetNetworkId((*it)->GetProcessing()));
			completeName += ".";
			completeName += (*it)->GetName();
			consumers.push_back(completeName);
		}
		return consumers;
	}

	Network::NamesList  Network::GetInControlsConnectedTo( const std::string & producer ) const
	{		
		OutControl & out = GetOutControlByCompleteName( producer );
		NamesList consumers;

		std::list<InControl*>::iterator it;
		for(it=out.BeginInControlsConnected();
		    it!=out.EndInControlsConnected();
		    it++)
		{
			std::string completeName(GetNetworkId((*it)->GetProcessing()));
			completeName += ".";
			completeName += (*it)->GetName();
			consumers.push_back(completeName);
		}
		return consumers;
	}

	Network::InPortsList Network::GetInPortsConnectedTo( OutPortBase & producer ) const
	{		
		InPortsList consumers;
		OutPortBase::InPortsList::iterator it;
		for(it=producer.BeginConnectedInPorts(); it!=producer.EndConnectedInPorts(); it++)
			consumers.push_back(*it);
		return consumers;
	}

	const std::string &  Network::GetNetworkId(const Processing * proc) const
	{
		ProcessingsMap::const_iterator it;
		for(it=BeginProcessings(); it!=EndProcessings(); it++)
			if(it->second == proc )
				return it->first;

		CLAM_ASSERT(false, "Trying to get a network id from a processing not present in it");
		throw 0; // To avoid warning message
	}

	void Network::ChangeKeyMap( const std::string & oldName, const std::string & newName )
	{
		if( mProcessings.find( newName ) != mProcessings.end() ) // newName is being used
		{
			CLAM_ASSERT( false, "Network::ChangeKeyMap Trying to modify unexistent processing" );
		}
		ProcessingsMap::iterator it = mProcessings.find( oldName );
		Processing * proc = it->second;
		mProcessings.erase( it );
		mProcessings.insert( ProcessingsMap::value_type( newName, proc ) );
	}
}

