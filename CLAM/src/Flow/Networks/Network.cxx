/*
 * Copyright (c) 2001-2007 MUSIC TECHNOLOGY GROUP (MTG)
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
#include "NaiveFlowControl.hxx"
#include "NetworkPlayer.hxx"
#include <algorithm>
#include "ProcessingDefinitionAdapter.hxx"
#include "ConnectionDefinitionAdapter.hxx"
#include "Factory.hxx"
#include "XmlStorageErr.hxx"
#ifdef USE_LADSPA //TODO alway include it. move conditional code in LFactory.hxx
#	include "LadspaFactory.hxx"
#endif

namespace CLAM
{	
	typedef Factory<CLAM::Processing> ProcessingFactory;

	Network::Network() :
		mName("Unnamed Network"),
		mFlowControl(new NaiveFlowControl),
		mPlayer(0)
	{}
	
	Network::~Network()
	{
		//std::cerr <<" *\t\t\t~NETWORK"<<std::endl;
		Clear();
		if (mFlowControl) delete mFlowControl;
		if (mPlayer) delete mPlayer;
	}

	void Network::StoreOn( Storage & storage) const
	{
		XMLAdapter<std::string> strAdapter( mName, "id");
		storage.Store(strAdapter);

		ProcessingsMap::const_iterator it;
		for(it=BeginProcessings();it!=EndProcessings();it++)
		{
			Processing * proc = it->second;
			ProcessingDefinitionAdapter procDefinition(proc, it->first);
			XMLComponentAdapter xmlAdapter(procDefinition, "processing", true);
			storage.Store(xmlAdapter);
		}

		// second iteration to store ports. 
		// XR: maybe it should be all in one iteration but this way
		// the xml file is clearer.

		for(it=BeginProcessings();it!=EndProcessings();it++)
		{
			const std::string & name = it->first;
			Processing * proc = it->second;
			OutPortRegistry::Iterator itOutPort;
			for (itOutPort=proc->GetOutPorts().Begin(); 
			     itOutPort!=proc->GetOutPorts().End(); 
			     itOutPort++)
			{
				if (!(*itOutPort)->HasConnections())
					continue;
	
				std::string outPortName = name + "." + (*itOutPort)->GetName();
				NamesList namesInPorts = GetInPortsConnectedTo(outPortName);
				NamesList::iterator namesIterator;
				for(namesIterator=namesInPorts.begin();
				    namesIterator!=namesInPorts.end();
				    namesIterator++)
				{
					ConnectionDefinitionAdapter connectionDefinition( outPortName, *namesIterator );
					XMLComponentAdapter xmlAdapter(connectionDefinition, "port_connection", true);
					storage.Store(xmlAdapter);
				}
			}
		}

		for(it=BeginProcessings();it!=EndProcessings();it++)
		{
			const std::string & name = it->first;
			Processing * proc = it->second;
			OutControlRegistry::Iterator itOutControl;
			for (itOutControl=proc->GetOutControls().Begin(); 
			     itOutControl!=proc->GetOutControls().End(); 
			     itOutControl++)
			{
				std::string outControlName = name+ "." + (*itOutControl)->GetName();
				NamesList namesInControls = GetInControlsConnectedTo(outControlName);
				NamesList::iterator namesIterator;
				for(namesIterator=namesInControls.begin();
				    namesIterator!=namesInControls.end();
				    namesIterator++)
				{
					ConnectionDefinitionAdapter connectionDefinition( outControlName, *namesIterator );
					XMLComponentAdapter xmlAdapter(connectionDefinition, "control_connection", true);
					storage.Store(xmlAdapter);
				}
			}
		}
	}

	void Network::LoadFrom( Storage & storage)
	{
		Clear();
		XMLAdapter<std::string> strAdapter( mName, "id");
		storage.Load(strAdapter);

		while(1)
		{
			ProcessingDefinitionAdapter procDefinition;
			XMLComponentAdapter xmlAdapter(procDefinition, "processing", true);
			if(storage.Load(xmlAdapter) == false) break;
			
			AddProcessing(procDefinition.GetName(), procDefinition.GetProcessing()); 
		}

		while(1)
		{
			ConnectionDefinitionAdapter connectionDefinition;
			XMLComponentAdapter xmlAdapter(connectionDefinition, "port_connection", true);
			if (!storage.Load(xmlAdapter)) break;
			const std::string & fullOut = connectionDefinition.GetOutName();
			const std::string & fullIn = connectionDefinition.GetInName();
			try
			{
				ConnectPorts( fullOut, fullIn );
			}
			catch (Err & e) { throw XmlStorageErr(e.what()); }
		}

		while(1)
		{
			ConnectionDefinitionAdapter connectionDefinition;
			XMLComponentAdapter xmlAdapter(connectionDefinition, "control_connection", true);
			if (!storage.Load(xmlAdapter)) break;
			const std::string & fullOut = connectionDefinition.GetOutName();
			const std::string & fullIn = connectionDefinition.GetInName();
			try 
			{
				ConnectControls( fullOut, fullIn );
			}
			catch (Err & e) { throw XmlStorageErr(e.what()); }
		}

	}

	void Network::AddFlowControl(FlowControl* flowControl)
	{
		if (mFlowControl) delete mFlowControl;
		mFlowControl = flowControl;
		mFlowControl->AttachToNetwork(this);
	}
	void Network::SetPlayer(NetworkPlayer* player)
	{
		if (mPlayer) delete mPlayer;
		mPlayer = player;
		mPlayer->SetNetworkBackLink(*this);	
	}

	Processing& Network::GetProcessing( const std::string & name ) const
	{
		CLAM_ASSERT( HasProcessing(name), 
			("No processing in the network has the name '"+name+"'.").c_str());

		ProcessingsMap::const_iterator it = mProcessings.find( name );
		return *it->second;
	}

	void Network::AddProcessing( const std::string & name, Processing* proc)
	{
		if (!IsStopped()) Stop();

		if (!mProcessings.insert( ProcessingsMap::value_type( name, proc ) ).second )
			CLAM_ASSERT(false, "Network::AddProcessing() Trying to add a processing with a repeated name (key)" );

		mFlowControl->ProcessingAddedToNetwork(*proc);
	}

	void Network::AddProcessing( const std::string & name, const std::string & factoryKey )
	{
		Processing * proc=0;
#ifdef USE_LADSPA //TODO move conditional code inside LadspaFactory
		try
		{
			proc = ProcessingFactory::GetInstance().CreateSafe( factoryKey  );
		}
		catch (ErrFactory&)
		{
			proc = LadspaFactory::GetInstance().CreateSafe( factoryKey );
		}
#else 
		proc = ProcessingFactory::GetInstance().CreateSafe( factoryKey  );
#endif
		AddProcessing(name, proc);
	}

	// returns the name that was used so the same one can be used when calling CreateProcessingController (hack)
	std::string Network::AddProcessing( const std::string & factoryKey )
	{
		std::string name = GetUnusedName( factoryKey  ); 
		AddProcessing(name, factoryKey );
		return name;
	}

	std::string Network::GetUnusedName( const std::string& prefix ) const
	{
		std::string name;

		for ( int i = 0; i<9999999; i++ ) 
		{
			std::stringstream tmp; 
			tmp << i;
			name = prefix + "_" + tmp.str(); 
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
		if ( !IsStopped() ) Stop(); 
		Processing * proc = i->second;
		mProcessings.erase( name );

		mFlowControl->ProcessingRemovedFromNetwork(*proc);
		delete proc;		
	}

	bool Network::HasProcessing( const std::string& name ) const
	{
		ProcessingsMap::const_iterator i = mProcessings.find( name );
		return i!=mProcessings.end();
	}
	
	bool Network::ConfigureProcessing( const std::string & name, const ProcessingConfig & newConfig )	
	{
		ProcessingsMap::iterator it = mProcessings.find( name );
		CLAM_ASSERT(it!=mProcessings.end(),"Wrong processing name to configure in a network");
		Processing * proc = it->second;
		if ( !IsStopped() ) Stop();
		bool ok = proc->Configure( newConfig );
		mFlowControl->ProcessingConfigured(*proc);
		return ok;
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
		mFlowControl->NetworkTopologyChanged();

		OutPortBase & outport = GetOutPortByCompleteName(producer);
		InPortBase & inport = GetInPortByCompleteName(consumer);

		if ( outport.IsVisuallyConnectedTo(inport) ) 
			return false;
			
		if ( !outport.IsConnectableTo(inport) ) //they have different type
			return false;

		if( inport.GetVisuallyConnectedOutPort())
			return false;

		if (!IsStopped()) Stop();

		outport.ConnectToIn( inport );
		return true;
	}

	bool Network::ConnectControls( const std::string & producer, const std::string & consumer )
	{
		OutControl & outcontrol = GetOutControlByCompleteName(producer);
		InControl & incontrol = GetInControlByCompleteName(consumer);

		if ( outcontrol.IsConnectedTo(incontrol) ) 
			return false;

		if (!IsStopped()) Stop();

		outcontrol.AddLink( incontrol );
		return true;
	}


	bool Network::DisconnectPorts( const std::string & producer, const std::string & consumer)
	{
		mFlowControl->NetworkTopologyChanged();

		OutPortBase & outport = GetOutPortByCompleteName(producer);
		InPortBase & inport = GetInPortByCompleteName(consumer);

		if ( !outport.IsVisuallyConnectedTo(inport))
			return false;

		if (!IsStopped()) Stop();

		outport.DisconnectFromIn( inport );
		return true;
	}

	bool Network::DisconnectControls( const std::string & producer, const std::string & consumer)
	{
		OutControl & outcontrol = GetOutControlByCompleteName(producer);
		InControl & incontrol = GetInControlByCompleteName(consumer);

		if ( !outcontrol.IsConnectedTo( incontrol )) 
			return false;

		if (!IsStopped()) Stop();

		outcontrol.RemoveLink( incontrol );
		return true;
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

	bool Network::IsStopped() const
	{
		if (! mPlayer) return true;
		return mPlayer->IsStopped();
	}

	void Network::Start()
	{
		ProcessingsMap::iterator it;
		for (it=BeginProcessings(); it!=EndProcessings(); it++)
		{
			if (it->second->IsRunning()) continue;
			if (it->second->IsConfigured())
			{
				it->second->Start();
			}
			else
			{	
				std::cerr << "Warning: could not start processing for not being Configured: '" << it->first<< "' of class " << it->second->GetClassName() << std::endl;
			}
		}
		if (mPlayer) mPlayer->Start();
	}
	
	void Network::Stop()
	{
		if (mPlayer) mPlayer->Stop();
		ProcessingsMap::iterator it;
		for (it=BeginProcessings(); it!=EndProcessings(); it++)
			if (it->second->IsRunning())
				it->second->Stop();
	}
	
	void Network::Do()
	{
		mFlowControl->Do();
	}

	void Network::Clear()
	{
		if ( !IsStopped() ) Stop(); 
		
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
		for(it=out.BeginVisuallyConnectedInPorts(); it!=out.EndVisuallyConnectedInPorts(); it++)
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
		for(it=producer.BeginVisuallyConnectedInPorts(); it!=producer.EndVisuallyConnectedInPorts(); it++)
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

	bool Network::RenameProcessing( const std::string & oldName, const std::string & newName )
	{
		if (oldName==newName) return true;
		if( mProcessings.find( newName ) != mProcessings.end() ) // newName is being used
			return false;
		ProcessingsMap::iterator it = mProcessings.find( oldName );
		Processing * proc = it->second;
		mProcessings.erase( it );
		mProcessings.insert( ProcessingsMap::value_type( newName, proc ) );
		return true;
	}

	bool Network::IsReady() const
	{
		if (IsEmpty()) return false;
		if (HasMisconfiguredProcessings()) return false;
		if (HasUnconnectedInPorts()) return false;
		return true;
	}

	bool Network::IsEmpty() const
	{
		return mProcessings.empty();
	}

	bool Network::HasMisconfiguredProcessings() const
	{
		ProcessingsMap::const_iterator it;
		for(it=BeginProcessings(); it!=EndProcessings(); it++)
			if(!it->second->IsConfigured())
				return true;
		return false;
	}

	bool Network::HasUnconnectedInPorts() const
	{
		return true;
	}

	bool Network::HasSyncSource() const
	{
		ProcessingsMap::const_iterator it;
		for(it=BeginProcessings(); it!=EndProcessings(); it++)
			if(it->second->IsSyncSource())
				return true;
		return false;
	}

	// TODO: Test GetConfigurationErrors
	std::string Network::GetConfigurationErrors() const
	{
		std::ostringstream errorMessage;
		ProcessingsMap::const_iterator it;
		for(it=BeginProcessings(); it!=EndProcessings(); it++)
		{
			if(it->second->IsConfigured()) continue;
			errorMessage << "* Processing '" <<  it->first  << "' is misconfigured:\n";
			errorMessage << it->second->GetConfigErrorMessage() << std::endl;
		}

		return errorMessage.str();
	}

}

