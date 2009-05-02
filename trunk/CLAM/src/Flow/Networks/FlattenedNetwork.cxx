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
#include "CLAMVersion.hxx"
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

		std::string version = CLAM::GetVersion();
		XMLAdapter<std::string> versionAdapter( version, "clamVersion");
		storage.Store(versionAdapter);

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

			unsigned nOutPorts = proc->GetNOutPorts();
			for (unsigned i = 0; i<nOutPorts; i++)
			{
				OutPortBase & outport = proc->GetOutPort(i);
				if (!outport.HasConnections())
					continue;
	
				std::string outPortName = name + "." + outport.GetName();
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

			unsigned nOutControls = proc->GetNOutControls();
			for (unsigned i = 0; i<nOutControls; i++)
			{
				OutControlBase & outControl = proc->GetOutControl(i);
				std::string outControlName = name+ "." + outControl.GetName();
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
		typedef std::map <std::string, std::string> NamesMap;
		NamesMap namesMap;
		if (!_setPasteMode) Clear();
		XMLAdapter<std::string> strAdapter( _name, "id");
		storage.Load(strAdapter);
		_processingsGeometries.clear();

		while(1)
		{
			ProcessingDefinitionAdapter procDefinition;
			XMLComponentAdapter xmlAdapter(procDefinition, "processing", true);
			if (not storage.Load(xmlAdapter)) break;

			const std::string & definitionName = procDefinition.GetName();
			CLAM::Processing * processing = procDefinition.GetProcessing();
			std::string finalName = definitionName;
			if (_setPasteMode)
			{
				finalName = GetUnusedName(definitionName, true);
				namesMap.insert(std::make_pair(definitionName,finalName));
			}
			AddProcessing(finalName, processing); 
			// if exists canvas geometries, restore them
			if (procDefinition.GetPosition()!="" && procDefinition.GetSize()!="")
			{
				Geometry geometry;
				StringPairToInts(procDefinition.GetPosition(),geometry.x,geometry.y);
				StringPairToInts(procDefinition.GetSize(),geometry.width,geometry.height);
				_processingsGeometries.insert(ProcessingsGeometriesMap::value_type(finalName,geometry));
			}
		}

		while(1)
		{
			ConnectionDefinitionAdapter connectionDefinition;
			XMLComponentAdapter xmlAdapter(connectionDefinition, "port_connection", true);
			if (not storage.Load(xmlAdapter)) break;
			std::string fullOut = connectionDefinition.GetOutName();
			std::string fullIn = connectionDefinition.GetInName();
			if (_setPasteMode)
			{
				fullOut = namesMap.find(GetProcessingIdentifier(fullOut))->second
					+"."+GetConnectorIdentifier(fullOut);
				fullIn = namesMap.find(GetProcessingIdentifier(fullIn))->second
					+"."+GetConnectorIdentifier(fullIn);
			}
			if (not ConnectPorts( fullOut, fullIn ))
				throw XmlStorageErr(std::string("Unable to connect ports '")+fullOut+"->"+fullIn+".");
		}

		while(1)
		{
			ConnectionDefinitionAdapter connectionDefinition;
			XMLComponentAdapter xmlAdapter(connectionDefinition, "control_connection", true);
			if (!storage.Load(xmlAdapter)) break;
			std::string fullOut = connectionDefinition.GetOutName();
			std::string fullIn = connectionDefinition.GetInName();
			if (_setPasteMode)
			{
				fullOut = namesMap.find(GetProcessingIdentifier(fullOut))->second
					+"."+GetConnectorIdentifier(fullOut);
				fullIn = namesMap.find(GetProcessingIdentifier(fullIn))->second
					+"."+GetConnectorIdentifier(fullIn);
			}
			if (not ConnectControls( fullOut, fullIn ))
				throw XmlStorageErr(std::string("Unable to connect controls '")+fullOut+"->"+fullIn+".");
		}
		_setPasteMode=false;
//		OrderSinksAndSources(_processingsGeometries);
	}

	bool FlattenedNetwork::UpdateSelections (const NamesList & processingsNamesList)
	{
		NamesList::const_iterator namesIterator;
		if (!_selectedProcessings.empty() || processingsNamesList.empty())
		{
			_selectedProcessings.clear();
			return true;
		}
		for (namesIterator=processingsNamesList.begin();namesIterator!=processingsNamesList.end();namesIterator++)
			_selectedProcessings.insert(*namesIterator);
		return false;
	}

	bool FlattenedNetwork::HasSelectionAndContains(const std::string & name) const
	{
		NamesSet::const_iterator itFindSelected = _selectedProcessings.find(name);
		if (!_selectedProcessings.empty() && itFindSelected==_selectedProcessings.end())
			return false;
		return true;
	}


	const FlattenedNetwork::Geometry FlattenedNetwork::findProcessingGeometry(Processing* processing) const
	{
		//TODO: use the map find!!!!
		for (ProcessingsGeometriesMap::const_iterator it=_processingsGeometries.begin();it!=_processingsGeometries.end();it++)
		{
			if ( &GetProcessing(it->first) == processing )
				return it->second;
		}
		Geometry nullGeometry={0,10000,0,0};
		return nullGeometry;
	}

	const Network::AudioSinks FlattenedNetwork::getOrderedSinks() const
	{
		std::list <ProcessingAndGeometry> sinksAndGeometries;
		AudioSinks orderedSinks;
		for (ProcessingsMap::const_iterator it=_processings.begin(); it!=_processings.end(); it++)
		{
			Processing * proc = it->second;
			const std::string className = proc->GetClassName();
			if (className!="AudioSink")
				continue;
			ProcessingAndGeometry item;
			item.processing = proc;
			item.geometry = findProcessingGeometry(proc);
			sinksAndGeometries.push_back(item);
			sinksAndGeometries.sort(compareGeometriesUpperYThan);
		}

		for (std::list<ProcessingAndGeometry>::const_iterator it=sinksAndGeometries.begin();
			it!=sinksAndGeometries.end();it++)
		{
			AudioSink* sink = dynamic_cast<AudioSink*>(it->processing);
			CLAM_ASSERT(sink, "Expected an AudioSink");
			orderedSinks.push_back( sink );
		}

		return orderedSinks;
	}

	const Network::AudioSources FlattenedNetwork::getOrderedSources() const
	{
		std::list <ProcessingAndGeometry> sourcesAndGeometries;
		AudioSources orderedSources;
		for (ProcessingsMap::const_iterator it=_processings.begin(); it!=_processings.end(); it++)
		{
			Processing * proc = it->second;
			const std::string className = proc->GetClassName();
			if (className!="AudioSource")
				continue;
			ProcessingAndGeometry item;
			item.processing = proc;
			item.geometry =findProcessingGeometry(proc);
		
			sourcesAndGeometries.push_back(item);
			sourcesAndGeometries.sort(compareGeometriesUpperYThan);
		}

		for (std::list<ProcessingAndGeometry>::const_iterator it=sourcesAndGeometries.begin();
			it!=sourcesAndGeometries.end();it++)
		{
			AudioSource* source = dynamic_cast<AudioSource*>(it->processing);
			CLAM_ASSERT(source, "Expected an AudioSource");
			orderedSources.push_back( source );
		}

		return orderedSources;
	}

	const Network::ControlSinks FlattenedNetwork::getOrderedControlSinks() const
	{
		std::list <ProcessingAndGeometry> controlSinksAndGeometries;
		ControlSinks orderedControlSinks;
		for (ProcessingsMap::const_iterator it=_processings.begin(); it!=_processings.end(); it++)
		{
			Processing * proc = it->second;
			const std::string className = proc->GetClassName();
			if (className!="ControlSink")
				continue;
			ProcessingAndGeometry item;
			item.processing = proc;
			item.geometry = findProcessingGeometry(proc);
			controlSinksAndGeometries.push_back(item);
			controlSinksAndGeometries.sort(compareGeometriesUpperXThan);
		}

		for (std::list<ProcessingAndGeometry>::const_iterator it=controlSinksAndGeometries.begin();
			it!=controlSinksAndGeometries.end();it++)
		{
			ControlSink* controlSink = dynamic_cast<ControlSink*>(it->processing);
			CLAM_ASSERT(controlSink, "Expected an AudioSink");
			orderedControlSinks.push_back( controlSink );
		}

		return orderedControlSinks;
	}
	
	const Network::ControlSources FlattenedNetwork::getOrderedControlSources() const
	{
		std::list <ProcessingAndGeometry> controlSourcesAndGeometries;
		ControlSources orderedControlSources;
		for (ProcessingsMap::const_iterator it=_processings.begin(); it!=_processings.end(); it++)
		{
			Processing * proc = it->second;
			const std::string className = proc->GetClassName();
			if (className!="ControlSource")
				continue;
			ProcessingAndGeometry item;
			item.processing = proc;
			item.geometry = findProcessingGeometry(proc);
			controlSourcesAndGeometries.push_back(item);
			controlSourcesAndGeometries.sort(compareGeometriesUpperXThan);
		}

		for (std::list<ProcessingAndGeometry>::const_iterator it=controlSourcesAndGeometries.begin();
			it!=controlSourcesAndGeometries.end();it++)
		{
			ControlSource* controlSource = dynamic_cast<ControlSource*>(it->processing);
			CLAM_ASSERT(controlSource, "Expected an AudioSink");
			orderedControlSources.push_back( controlSource );
		}

		return orderedControlSources;
	}

	bool FlattenedNetwork::SetProcessingsGeometries (const ProcessingsGeometriesMap & processingsGeometries)
	{
		_processingsGeometries.clear(); 
		if (processingsGeometries.empty())
			return true;
		_processingsGeometries=processingsGeometries;
		return false;
	}

	const FlattenedNetwork::ProcessingsGeometriesMap FlattenedNetwork::GetAndClearGeometries()
	{
		const ProcessingsGeometriesMap copyProcessingsGeometry(_processingsGeometries);
		_processingsGeometries.clear();
		return copyProcessingsGeometry;
	}

	const bool FlattenedNetwork::compareGeometriesUpperYThan (ProcessingAndGeometry & arg1, ProcessingAndGeometry & arg2)
	{
		return arg1.geometry.y < arg2.geometry.y ;
	}

	const bool FlattenedNetwork::compareGeometriesUpperXThan (ProcessingAndGeometry & arg1, ProcessingAndGeometry & arg2)
	{
		return arg1.geometry.x < arg2.geometry.x ;
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

	std::string FlattenedNetwork::GetUnusedName( const std::string& prefix, const bool cutOnLastSeparator, const std::string separator ) const
	{
		std::string name;
		std::string newPrefix=prefix;
		if (cutOnLastSeparator==true)
		{
			int lastSeparatorPos=prefix.rfind(separator);
			if (lastSeparatorPos!=-1)
				newPrefix=prefix.substr(0,lastSeparatorPos);
		}

		for ( int i = 0; i<9999999; i++ ) 
		{
			std::stringstream tmp; 
			tmp << i;
			name = i? newPrefix + separator + tmp.str() : newPrefix; 
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
		OutControlBase & outcontrol = GetOutControlByCompleteName(producer);
		InControlBase & incontrol = GetInControlByCompleteName(consumer);

		if ( outcontrol.IsConnectedTo(incontrol) ) 
			return false;

		if ( !outcontrol.IsLinkable(incontrol) ) //they have different type
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
		OutControlBase & outcontrol = GetOutControlByCompleteName(producer);
		InControlBase & incontrol = GetInControlByCompleteName(consumer);

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
		return proc.GetInPort( GetConnectorIdentifier(name) );
	}

	OutPortBase & FlattenedNetwork::GetOutPortByCompleteName( const std::string & name ) const
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetOutPort( GetConnectorIdentifier(name) );
	}

	InControlBase & FlattenedNetwork::GetInControlByCompleteName( const std::string & name ) const
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetInControl( GetConnectorIdentifier(name) );
	}

	OutControlBase & FlattenedNetwork::GetOutControlByCompleteName( const std::string & name ) const
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetOutControl( GetConnectorIdentifier(name) );
	}

	bool FlattenedNetwork::IsStopped() const
	{
		if (! _player) return true;
		return _player->IsStopped();
	}

	bool FlattenedNetwork::IsPlaying() const
	{
		if (! _player) return false;
		return _player->IsPlaying();
	}

	bool FlattenedNetwork::IsPaused() const
	{
		if (! _player) return false;
		return _player->IsPaused();
	}

	void FlattenedNetwork::Start()
	{
		ProcessingsMap::iterator it;
		for (it=BeginProcessings(); it!=EndProcessings(); it++)
		{
			if (it->second->IsRunning()) 
{
			continue;
}
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
	void FlattenedNetwork::Pause()
	{
		if (_player) _player->Pause();
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
		OutControlBase & out = GetOutControlByCompleteName( producer );
		NamesList consumers;

		std::list<InControlBase*>::iterator it;
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
			unsigned nInPorts = proc->GetNInPorts();
			for (unsigned i = 0; i<nInPorts; i++)
			{
				InPortBase & inPort = proc->GetInPort(i);
				if (not inPort.GetVisuallyConnectedOutPort())
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
			unsigned nInPorts = proc->GetNInPorts();
			for (unsigned i = 0; i<nInPorts; i++)
			{
				InPortBase & inPort = proc->GetInPort(i);
				if (not inPort.GetVisuallyConnectedOutPort())
					result+= it->first + "." + inPort.GetName() + "\n";
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
