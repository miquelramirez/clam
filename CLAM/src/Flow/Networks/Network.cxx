/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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
		Clear();
		if (mFlowControl)
			delete mFlowControl;
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
			PublishedOutPorts::Iterator itOutPort;
			for (itOutPort=proc->GetOutPorts().Begin(); 
			     itOutPort!=proc->GetOutPorts().End(); 
			     itOutPort++)
			{
				if (!(*itOutPort)->GetNode())
					break;
	
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

		// third iteration to store ports. 
		// XR: maybe it should be all in one iteration but this way
		// the xml file is clearer.

		for(it=BeginProcessings();it!=EndProcessings();it++)
		{
			std::string name(it->first);
			Processing * proc = it->second;
			PublishedOutControls::Iterator itOutControl;
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

		// second iteration to load ports. 
		// XR: maybe it should be all in one iteration but this way
		// the xml file is clearer.


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

		Processing * proc = ProcessingFactory::GetInstance().Create( key );

		// returns false if the key was repeated.
		if (!mProcessings.insert( ProcessingsMap::value_type( name, proc ) ).second )
			CLAM_ASSERT(false, "Network::AddProcessing() Trying to add a processing with a repeated name (key)" );

		mFlowControl->ProcessingAddedToNetwork(*proc);
	}


	void Network::RemoveProcessing ( const std::string & name)
	{
		CLAM_ASSERT( mFlowControl, 
			     "Network::RemoveProcessing() - Network should have an attached flow control at this state.");

		ProcessingsMap::const_iterator i = mProcessings.find( name );
		if(i==mProcessings.end())
			CLAM_ASSERT(false, "Network::RemoveProcessing() Trying to remove a processing that is not included in the network" );
		
		Processing * proc = i->second;
		mProcessings.erase( name );

		PublishedInPorts::Iterator itInPort;
		for(itInPort=proc->GetInPorts().Begin(); 
		    itInPort!=proc->GetInPorts().End();
		    itInPort++)
		{
			(*itInPort)->Unattach();
		}

		PublishedOutPorts::Iterator itOutPort;
		for(itOutPort=proc->GetOutPorts().Begin(); 
		    itOutPort!=proc->GetOutPorts().End();
		    itOutPort++)
		{
			(*itOutPort)->Unattach();
		}

		mFlowControl->ProcessingRemovedFromNetwork(*proc);
		delete proc;
		
	}

	bool Network::HasProcessing( const std::string & name ) const
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


	bool Network::ConnectPorts( const std::string & producer, const std::string & consumer )
	{
		AssertFlowControlNotNull();
		mFlowControl->NetworkTopologyChanged();

		OutPort & outport = GetOutPortByCompleteName(producer);
		InPort & inport = GetInPortByCompleteName(consumer);

		if ( outport.IsConnectedTo(inport) ) 
			return false;
			
		if ( !outport.IsConnectableTo(inport) ) //they have different type
			return false;

		if( inport.IsAttached())
			return false;

		inport.Attach(GetNodeAttachedTo(outport));
		
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

		OutPort & outport = GetOutPortByCompleteName(producer);
		InPort & inport = GetInPortByCompleteName(consumer);

		if ( !outport.IsConnectedTo(inport) ) 
			return false;

		inport.Unattach();
		

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
			PublishedInPorts::Iterator iteratorInPorts;

			//unattach all the inports of each processing
			for(iteratorInPorts=proc->GetInPorts().Begin();
			    iteratorInPorts!=proc->GetInPorts().End();
			    iteratorInPorts++)
				(*iteratorInPorts)->Unattach();
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

	std::string Network::GetLastIdentifier( const std::string& str ) const
	{
		return str.substr( PositionOfLastIdentifier(str)+1 );
	}

	std::string Network::GetProcessingIdentifier( const std::string& str ) const
	{
		std::size_t length = PositionOfLastIdentifier(str)  - PositionOfProcessingIdentifier(str);
		return str.substr( PositionOfProcessingIdentifier(str), length);
	}

	InPort & Network::GetInPortByCompleteName( const std::string & name ) const
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetInPorts().Get( GetLastIdentifier(name) );
	}

	OutPort & Network::GetOutPortByCompleteName( const std::string & name ) const
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetOutPorts().Get( GetLastIdentifier(name) );
	}

	InControl & Network::GetInControlByCompleteName( const std::string & name ) const
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetInControls().Get( GetLastIdentifier(name) );
	}

	OutControl & Network::GetOutControlByCompleteName( const std::string & name ) const
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetOutControls().Get( GetLastIdentifier(name) );
	}

	NodeBase & Network::GetNodeAttachedTo( OutPort & out )
	{
			if (!out.GetNode())
			{
				NodeBase * node = CreateAudioNodeWithDefaultStreamBuffer();
				out.Attach(*node);
				mNodes.push_back(node);
				mFlowControl->NetworkTopologyChanged();
			}
			return *out.GetNode();
	}

	NodeBase* Network::CreateAudioNodeWithDefaultStreamBuffer()
	{
		//@todo
		typedef CircularStreamImpl<TData> DefaultStreamBuffer;
		NodeBase * node = new NodeTmpl<Audio, DefaultStreamBuffer>;
		mNodesToConfigure.push_back( node );
		return node;
		//return new AudioNodeTmpl;
	}
	
	void Network::ConfigureAllNodes()
	{
		AssertFlowControlNotNull();

		Nodes::iterator it = mNodesToConfigure.begin();
		while ( it!=mNodesToConfigure.end() )
			mFlowControl->ConfigureNode(**it++);
		
		mNodesToConfigure.clear();
	}

	void Network::Start()
	{
		ProcessingsMap::iterator it;
		for (it=BeginProcessings(); it!=EndProcessings(); it++)
			if (it->second->GetExecState() == Processing::Ready)
				it->second->Start();		

		ConfigureAllNodes(); // todo: provisional till refactoring of Nodes configuration finished.

	}
	void Network::Stop()
	{
		ProcessingsMap::iterator it;
		for (it=BeginProcessings(); it!=EndProcessings(); it++)
			if (it->second->GetExecState() == Processing::Running)
				it->second->Stop();
	
	}
	void Network::DoProcessings()
	{
		AssertFlowControlNotNull();
		mFlowControl->DoProcessings();
	}


	void Network::Clear()
	{
		Stop();
		// TODO: nodes destruction is really inestable at this moment, in supervised-mode-2004 it will be solved.
//		Nodes::iterator itNodes;
//		for(itNodes=BeginNodes();itNodes!=EndNodes();itNodes++)
//			delete *itNodes;
		mNodes.clear();
		
		ProcessingsMap::iterator it;
		for( it=mProcessings.begin(); it!=mProcessings.end(); it++ )
		{
			RemoveProcessing( it->first );
		}
		mProcessings.clear();	
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

	Network::Nodes::iterator Network::BeginNodes()
	{
		return mNodes.begin();
	}

	Network::Nodes::iterator Network::EndNodes()
	{
		return mNodes.end();
	}

	Network::Nodes::const_iterator Network::BeginNodes() const
	{
		return mNodes.begin();
	}

	Network::Nodes::const_iterator Network::EndNodes() const
	{
		return mNodes.end();
	}

	Network::NamesList  Network::GetInPortsConnectedTo( const std::string & producer ) const
	{		
		OutPort & out = GetOutPortByCompleteName( producer );
//		CLAM_ASSERT( out.GetNode(), "Trying to access a node from an outport without connections");
		NamesList consumers;

		if(!out.GetNode())
			return consumers;

		NodeBase::ReaderIterator it;
		for(it=out.GetNode()->BeginReaders();
		    it!=out.GetNode()->EndReaders();
		    it++)
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

	Network::InPortsList Network::GetInPortsConnectedTo( OutPort & producer ) const
	{		
		CLAM_ASSERT( producer.GetNode(), "Trying to access a node from an outport without connections");
		InPortsList consumers;

		NodeBase::ReaderIterator it;
		for(it=producer.GetNode()->BeginReaders(); it!=producer.GetNode()->EndReaders(); it++)
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
