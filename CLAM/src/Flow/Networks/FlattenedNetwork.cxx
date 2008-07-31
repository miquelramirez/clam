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


#include "FlattenedNetwork.hxx"
#include "NaiveFlowControl.hxx"
#include "NetworkPlayer.hxx"
#include <algorithm>
#include "ProcessingDefinitionAdapter.hxx"
#include "ConnectionDefinitionAdapter.hxx"
#include "ProcessingFactory.hxx"
#include "XmlStorageErr.hxx"
#ifdef USE_LADSPA //TODO alway include it. move conditional code in LFactory.hxx
#	include "ProcessingFactory.hxx"
#endif

namespace CLAM
{	
	FlattenedNetwork::FlattenedNetwork() :
		_name("Unnamed Network"),
		_flowControl(new NaiveFlowControl),
		_player(0),
		_setPasteMode(false)
	{}
	
	FlattenedNetwork::~FlattenedNetwork()
	{
		//std::cerr <<" *\t\t\t~NETWORK"<<std::endl;
		Clear();
		if (_flowControl) delete _flowControl;
		if (_player) delete _player;
	}

	void FlattenedNetwork::StoreOn( Storage & storage) const
	{
		XMLAdapter<std::string> strAdapter( _name, "id");
		storage.Store(strAdapter);

		ProcessingsMap::const_iterator it;
		for(it=BeginProcessings();it!=EndProcessings();it++)
		{
			Processing * proc = it->second;
			const std::string & name = it->first;
			if (!HasSelectionAndContains(name))
				continue;
			std::string processingPosition;
			std::string processingSize;
			// if exists canvas geometries, store them
			if (!_processingsGeometries.empty())
			{
				Geometry & geometry=_processingsGeometries.find(name)->second;
				processingPosition=IntsToString (geometry.x, geometry.y);
				processingSize=IntsToString (geometry.width, geometry.height);
			}
			ProcessingDefinitionAdapter procDefinition(proc, name, processingPosition, processingSize);
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

			if (!HasSelectionAndContains(name))
				continue;

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
					if (!HasSelectionAndContains(GetProcessingIdentifier(*namesIterator)))
						continue;
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

			if (!HasSelectionAndContains(name))
				continue;

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
					if (!HasSelectionAndContains(GetProcessingIdentifier(*namesIterator)))
						continue;
					ConnectionDefinitionAdapter connectionDefinition( outControlName, *namesIterator );
					XMLComponentAdapter xmlAdapter(connectionDefinition, "control_connection", true);
					storage.Store(xmlAdapter);
				}
			}
		}
		_selectedProcessings.clear();
		_processingsGeometries.clear();
	}

	void FlattenedNetwork::LoadFrom( Storage & storage)
	{
		typedef std::map <std::string, std::string> changeProcNames;
		changeProcNames newProcNames;
		if (!_setPasteMode) Clear();
		XMLAdapter<std::string> strAdapter( _name, "id");
		storage.Load(strAdapter);
		_processingsGeometries.clear();

		while(1)
		{
			ProcessingDefinitionAdapter procDefinition;
			XMLComponentAdapter xmlAdapter(procDefinition, "processing", true);
			if(storage.Load(xmlAdapter) == false) break;
			std::string name=procDefinition.GetName();
			
			if (!_setPasteMode)
				AddProcessing(name, procDefinition.GetProcessing()); 
			else
			{
				CLAM::Processing * processing =procDefinition.GetProcessing();
				std::string key=processing->GetClassName();
				std::string newName= AddProcessing(key);
				CLAM::Processing & newProcessing = GetProcessing(newName);
				newProcessing.Configure(processing->GetConfig());
				newProcNames.insert(changeProcNames::value_type(name,newName));
				name=newName;
			}
			// if exists canvas geometries, restore them
			if (procDefinition.GetPosition()!="" && procDefinition.GetSize()!="")
			{
				Geometry processingGeometry;
				StringPairToInts(procDefinition.GetPosition(),processingGeometry.x,processingGeometry.y);
				StringPairToInts(procDefinition.GetSize(),processingGeometry.width,processingGeometry.height);
				_processingsGeometries.insert(ProcessingsGeometriesMap::value_type(name,processingGeometry));
			}
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
				if (!_setPasteMode)
					ConnectPorts( fullOut, fullIn );
				else
				{
					const std::string newNameOut = newProcNames.find(GetProcessingIdentifier(fullOut))->second
						+"."+GetConnectorIdentifier(fullOut);
					const std::string newNameIn = newProcNames.find(GetProcessingIdentifier(fullIn))->second
						+"."+GetConnectorIdentifier(fullIn);
					ConnectPorts( newNameOut, newNameIn );
				}
				
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
				if (!_setPasteMode)
					ConnectControls( fullOut, fullIn );
				else
				{
					const std::string newNameOut = newProcNames.find(GetProcessingIdentifier(fullOut))->second
						+"."+GetConnectorIdentifier(fullOut);
					const std::string newNameIn = newProcNames.find(GetProcessingIdentifier(fullIn))->second
						+"."+GetConnectorIdentifier(fullIn);
					ConnectControls( newNameOut, newNameIn );
				}
			}
			catch (Err & e) { throw XmlStorageErr(e.what()); }
		}
		_setPasteMode=false;
	}

	bool FlattenedNetwork::UpdateSelections (const NamesList & processingsNamesList)
	{
		NamesList::const_iterator namesIterator;
		if (!_selectedProcessings.empty() || processingsNamesList.empty())
		{
			_selectedProcessings.clear();
			return 1;
		}
		for (namesIterator=processingsNamesList.begin();namesIterator!=processingsNamesList.end();namesIterator++)
			_selectedProcessings.insert(*namesIterator);
		return 0;
		
	}

	bool FlattenedNetwork::HasSelectionAndContains(const std::string & name) const
	{
		NamesSet::const_iterator itFindSelected = _selectedProcessings.find(name);
		if (!_selectedProcessings.empty() && itFindSelected==_selectedProcessings.end())
			return 0;
		return 1;
	}

	bool FlattenedNetwork::SetProcessingsGeometries (const ProcessingsGeometriesMap & processingsGeometries)
	{
		_processingsGeometries.clear();
		if (processingsGeometries.empty())
			return 1;
		_processingsGeometries=processingsGeometries;
		return 0;
	}


	const FlattenedNetwork::ProcessingsGeometriesMap FlattenedNetwork::GetAndClearGeometries()
	{
		const ProcessingsGeometriesMap copyProcessingsGeometry(_processingsGeometries);
		_processingsGeometries.clear();
		return copyProcessingsGeometry;
	}

/*	// TODO: use individual geometries loadings/storings??:
	const FlattenedNetwork::Geometry FlattenedNetwork::GetAndEraseGeometry(std::string name)
	{
		const ProcessingsGeometriesMap::iterator itGeometry =_processingsGeometries.find(name);
		Geometry geometry=itGeometry->second;
		if (itGeometry == _processingsGeometries.end())
		{
			geometry.width=0;
			geometry.height=0;
			return geometry;
		}
		_processingsGeometries.erase(name); // if exists, erase geometry from map
		return geometry;
	}*/

	void FlattenedNetwork::StringPairToInts(const std::string & geometryInString, int & a, int & b)
	{
		a=atoi(geometryInString.substr(0,geometryInString.find(",")).c_str());
		b=atoi(geometryInString.substr(geometryInString.find(",")+1,geometryInString.length()).c_str());
	}

	const std::string FlattenedNetwork::IntsToString (const int & a, const int & b) const
	{
		std::ostringstream stream;
		stream<<a<<","<<b;
		return stream.str();
	}

	// flow and player related methods
	void FlattenedNetwork::AddFlowControl(FlowControl* flowControl)
	{
		if (_flowControl) delete _flowControl;
		_flowControl = flowControl;
		_flowControl->AttachToNetwork((CLAM::Network*)this);
	}
	void FlattenedNetwork::SetPlayer(NetworkPlayer* player)
	{
		if (_player) delete _player;
		_player = player;
		_player->SetNetworkBackLink(*(CLAM::Network*)this);
		_player->Init();
	}
	unsigned FlattenedNetwork::BackendBufferSize()
	{
		if (!_player) return 512;
		return _player->BackendBufferSize();
	}
	unsigned FlattenedNetwork::BackendSampleRate()
	{
		if (!_player) return 44100;
		return _player->BackendSampleRate();
	}


	Processing& FlattenedNetwork::GetProcessing( const std::string & name ) const
	{
		CLAM_ASSERT( HasProcessing(name), 
			("No processing in the network has the name '"+name+"'.").c_str());

		ProcessingsMap::const_iterator it = _processings.find( name );
		return *it->second;
	}

	void FlattenedNetwork::AddProcessing( const std::string & name, Processing* proc)
	{
		if (!IsStopped()) Stop();

		if (!_processings.insert( ProcessingsMap::value_type( name, proc ) ).second )
			CLAM_ASSERT(false, "FlattenedNetwork::AddProcessing() Trying to add a processing with a repeated name (key)" );
		proc->SetNetworkBackLink((CLAM::Network*)this);
		proc->Configure(proc->GetConfig()); //TODO inefficient. but solves the problem 
		// of some processings needing the network for configuring its ports.
		_flowControl->ProcessingAddedToNetwork(*proc);
	}

	void FlattenedNetwork::AddProcessing( const std::string & name, const std::string & factoryKey )
	{
		Processing * proc=0;
		proc = ProcessingFactory::GetInstance().CreateSafe( factoryKey  );
		AddProcessing(name, proc);
	}

	// returns the name that was used so the same one can be used when calling CreateProcessingController (hack)
	std::string FlattenedNetwork::AddProcessing( const std::string & factoryKey )
	{
		std::string name = GetUnusedName( factoryKey  ); 
		AddProcessing(name, factoryKey );
		return name;
	}

	std::string FlattenedNetwork::GetUnusedName( const std::string& prefix ) const
	{
		std::string name;

		for ( int i = 0; i<9999999; i++ ) 
		{
			std::stringstream tmp; 
			tmp << i;
			name = i? prefix + "_" + tmp.str() : prefix; 
			if (!this->HasProcessing( name ) ) return name;
		}
		CLAM_ASSERT(false, "All valid id's for given prefix are exhausted");
		return "";
	}

	void FlattenedNetwork::RemoveProcessing ( const std::string & name)
	{
		CLAM_ASSERT( _flowControl, 
			     "FlattenedNetwork::RemoveProcessing() - Network should have an attached flow control at this state.");

		ProcessingsMap::const_iterator i = _processings.find( name );
		if(i==_processings.end())
		{
			std::string msg("FlattenedNetwork::RemoveProcessing() Trying to remove a processing that is not included in the network:");
			msg += name;
			CLAM_ASSERT(false, msg.c_str() );
		}
		if ( !IsStopped() ) Stop(); 
		Processing * proc = i->second;
		_processings.erase( name );

		_flowControl->ProcessingRemovedFromNetwork(*proc);
		delete proc;		
	}

	bool FlattenedNetwork::HasProcessing( const std::string& name ) const
	{
		ProcessingsMap::const_iterator i = _processings.find( name );
		return i!=_processings.end();
	}
	
	bool FlattenedNetwork::ConfigureProcessing( const std::string & name, const ProcessingConfig & newConfig )	
	{
		ProcessingsMap::iterator it = _processings.find( name );
		CLAM_ASSERT(it!=_processings.end(),"Wrong processing name to configure in a network");
		Processing * proc = it->second;
		if ( !IsStopped() ) Stop();
		bool ok = proc->Configure( newConfig );
		_flowControl->ProcessingConfigured(*proc);
		return ok;
	}

	void FlattenedNetwork::ReconfigureAllProcessings()
	{
		ProcessingsMap::iterator it;
		for( it=_processings.begin(); it!=_processings.end(); it++)
		{
			Processing* proc = it->second;
			proc->Configure( proc->GetConfig() );
		}
	}

	bool FlattenedNetwork::ConnectPorts( const std::string & producer, const std::string & consumer )
	{
		_flowControl->NetworkTopologyChanged();

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

	bool FlattenedNetwork::ConnectControls( const std::string & producer, const std::string & consumer )
	{
		OutControl & outcontrol = GetOutControlByCompleteName(producer);
		InControl & incontrol = GetInControlByCompleteName(consumer);

		if ( outcontrol.IsConnectedTo(incontrol) ) 
			return false;

		if (!IsStopped()) Stop();

		outcontrol.AddLink( incontrol );
		return true;
	}


	bool FlattenedNetwork::DisconnectPorts( const std::string & producer, const std::string & consumer)
	{
		_flowControl->NetworkTopologyChanged();

		OutPortBase & outport = GetOutPortByCompleteName(producer);
		InPortBase & inport = GetInPortByCompleteName(consumer);

		if ( !outport.IsVisuallyConnectedTo(inport))
			return false;

		if (!IsStopped()) Stop();

		outport.DisconnectFromIn( inport );
		return true;
	}

	bool FlattenedNetwork::DisconnectControls( const std::string & producer, const std::string & consumer)
	{
		OutControl & outcontrol = GetOutControlByCompleteName(producer);
		InControl & incontrol = GetInControlByCompleteName(consumer);

		if ( !outcontrol.IsConnectedTo( incontrol )) 
			return false;

		if (!IsStopped()) Stop();

		outcontrol.RemoveLink( incontrol );
		return true;
	}

	std::string FlattenedNetwork::GetConnectorIdentifier( const std::string& str ) const
	{
		return str.substr( PositionOfLastIdentifier(str)+1 );
	}

	std::string FlattenedNetwork::GetProcessingIdentifier( const std::string& str ) const
	{
		std::size_t length = PositionOfLastIdentifier(str)  - PositionOfProcessingIdentifier(str);
		return str.substr( PositionOfProcessingIdentifier(str), length);
	}

	InPortBase & FlattenedNetwork::GetInPortByCompleteName( const std::string & name ) const
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetInPorts().Get( GetConnectorIdentifier(name) );
	}

	OutPortBase & FlattenedNetwork::GetOutPortByCompleteName( const std::string & name ) const
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetOutPorts().Get( GetConnectorIdentifier(name) );
	}

	InControl & FlattenedNetwork::GetInControlByCompleteName( const std::string & name ) const
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetInControls().Get( GetConnectorIdentifier(name) );
	}

	OutControl & FlattenedNetwork::GetOutControlByCompleteName( const std::string & name ) const
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetOutControls().Get( GetConnectorIdentifier(name) );
	}

	bool FlattenedNetwork::IsStopped() const
	{
		if (! _player) return true;
		return _player->IsStopped();
	}

	void FlattenedNetwork::Start()
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
		if (_player) _player->Start();
	}
	
	void FlattenedNetwork::Stop()
	{
		if (_player) _player->Stop();
		ProcessingsMap::iterator it;
		for (it=BeginProcessings(); it!=EndProcessings(); it++)
			if (it->second->IsRunning())
				it->second->Stop();
	}
	
	void FlattenedNetwork::Do()
	{
		_flowControl->Do();
	}

	void FlattenedNetwork::Clear()
	{
		if ( !IsStopped() ) Stop(); 
		
		while( !_processings.empty() )
		{
			//std::cerr << "REMOVING <"<<_processings.begin()->first<<">"<<std::endl;
			RemoveProcessing( _processings.begin()->first );
		}
	}

	FlattenedNetwork::ProcessingsMap::iterator FlattenedNetwork::BeginProcessings()
	{
		return _processings.begin();
	}

	FlattenedNetwork::ProcessingsMap::iterator FlattenedNetwork::EndProcessings()
	{
		return _processings.end();
	}
	FlattenedNetwork::ProcessingsMap::const_iterator FlattenedNetwork::BeginProcessings() const
	{
		return _processings.begin();
	}

	FlattenedNetwork::ProcessingsMap::const_iterator FlattenedNetwork::EndProcessings() const
	{
		return _processings.end();
	}

	FlattenedNetwork::NamesList  FlattenedNetwork::GetInPortsConnectedTo( const std::string & producer ) const
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

	FlattenedNetwork::NamesList  FlattenedNetwork::GetInControlsConnectedTo( const std::string & producer ) const
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

	FlattenedNetwork::InPortsList FlattenedNetwork::GetInPortsConnectedTo( OutPortBase & producer ) const
	{		
		InPortsList consumers;
		OutPortBase::InPortsList::iterator it;
		for(it=producer.BeginVisuallyConnectedInPorts(); it!=producer.EndVisuallyConnectedInPorts(); it++)
			consumers.push_back(*it);
		return consumers;
	}

	const std::string &  FlattenedNetwork::GetNetworkId(const Processing * proc) const
	{
		ProcessingsMap::const_iterator it;
		for(it=BeginProcessings(); it!=EndProcessings(); it++)
			if(it->second == proc )
				return it->first;

		CLAM_ASSERT(false, "Trying to get a network id from a processing not present in it");
		throw 0; // To avoid warning message
	}

	bool FlattenedNetwork::RenameProcessing( const std::string & oldName, const std::string & newName )
	{
		if (oldName==newName) return true;
		if( _processings.find( newName ) != _processings.end() ) // newName is being used
			return false;
		ProcessingsMap::iterator it = _processings.find( oldName );
		Processing * proc = it->second;
		_processings.erase( it );
		_processings.insert( ProcessingsMap::value_type( newName, proc ) );
		return true;
	}

	bool FlattenedNetwork::IsReady() const
	{
		if (IsEmpty()) return false;
		if (HasMisconfiguredProcessings()) return false;
		if (HasUnconnectedInPorts()) return false;
		return true;
	}

	bool FlattenedNetwork::IsEmpty() const
	{
		return _processings.empty();
	}

	bool FlattenedNetwork::HasMisconfiguredProcessings() const
	{
		ProcessingsMap::const_iterator it;
		for(it=BeginProcessings(); it!=EndProcessings(); it++)
			if(!it->second->IsConfigured())
				return true;
		return false;
	}

	bool FlattenedNetwork::HasUnconnectedInPorts() const
	{
		for (ProcessingsMap::const_iterator it=BeginProcessings(); it!=EndProcessings(); it++)
		{
			Processing * proc = it->second;
			InPortRegistry::Iterator itInPort;
			for (itInPort=proc->GetInPorts().Begin(); 
			     itInPort!=proc->GetInPorts().End(); 
			     itInPort++)
			{
				if (not (*itInPort)->GetVisuallyConnectedOutPort())
					return true;
			}
			
		}
		return false;
	}
	std::string FlattenedNetwork::GetUnconnectedInPorts() const
	{
		std::string result;
		for (ProcessingsMap::const_iterator it=BeginProcessings(); it!=EndProcessings(); it++)
		{
			Processing * proc = it->second;
			InPortRegistry::Iterator itInPort;
			for (itInPort=proc->GetInPorts().Begin(); 
			     itInPort!=proc->GetInPorts().End(); 
			     itInPort++)
			{
				if (not (*itInPort)->GetVisuallyConnectedOutPort())
					result+= it->first+"."+(*itInPort)->GetName()+"\n";
			}
			
		}
		return result;
	}

	bool FlattenedNetwork::HasSyncSource() const
	{
		ProcessingsMap::const_iterator it;
		for(it=BeginProcessings(); it!=EndProcessings(); it++)
			if(it->second->IsSyncSource())
				return true;
		return false;
	}

	// TODO: Test GetConfigurationErrors
	std::string FlattenedNetwork::GetConfigurationErrors() const
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

