
#include "Network.hxx"
#include "FlowControl.hxx"
#include <algorithm>

namespace CLAM
{
	namespace HelperFunctions
	{
		void DeleteProcessing( Network::ProcessingsMap::value_type& mapElem ) {
			delete mapElem.second;
		}
	}
	// constructor / destructor

	Network::Network() :
		_name("Unnamed Network"),
		_flowControl(0)
	{}
	
	Network::~Network()
	{
		if (_flowControl)
			delete _flowControl;

		std::for_each( 	_processings.begin(), _processings.end(), HelperFunctions::DeleteProcessing );
	}

	/** Gets the ownership of the FlowControl passed. So it will be deleted by the destructor */
	void Network::AddFlowControl(FlowControl* flowControl)
	{		
		_flowControl = flowControl;
		_flowControl->AttachToNetwork(this);
	}

	Processing& Network::GetProcessing( const std::string & name )
	{
		CLAM_ASSERT( HasProcessing(name), "No Processing with the given name" );

		ProcessingsMap::const_iterator it = _processings.find( name );
		return *it->second;
	}

	void Network::AssertFlowControlNotNull() const
	{
		CLAM_ASSERT( 
			_flowControl, 
			"the Network should have a FlowControl. Use Network::AddFlowControl(FlowControl*)");
	}

	void Network::AddProcessing( const std::string & name, Processing* proc)
	{
		AssertFlowControlNotNull();

		// returns false if the key was repeated.
		if (!_processings.insert( ProcessingsMap::value_type( name, proc ) ).second )
			CLAM_ASSERT(false, "Network::AddProcessing() Trying to add a processing with a repeated name (key)" );

		_flowControl->ProcessingAddedToNetwork(*proc);
	}

	void Network::RemoveProcessing ( const std::string & name)
	{
		ProcessingsMap::const_iterator i = _processings.find( name );
		if(i==_processings.end())
			CLAM_ASSERT(false, "Network::RemoveProcessing() Trying to remove a processing that is not included in the network" );
		
		Processing * proc = i->second;
		_processings.erase( name );

		Processing::InPortIterator itInPort;
		for(itInPort=proc->GetInPorts().Begin(); 
		    itInPort!=proc->GetInPorts().End();
		    itInPort++)
		{
			(*itInPort)->Unattach();
		}

		Processing::OutPortIterator itOutPort;
		for(itOutPort=proc->GetOutPorts().Begin(); 
		    itOutPort!=proc->GetOutPorts().End();
		    itOutPort++)
		{
			(*itOutPort)->Unattach();
		}
		delete proc;
		
	}

	bool Network::HasProcessing( const std::string & name )
	{
		ProcessingsMap::const_iterator i = _processings.find( name );
		return i!=_processings.end();
	}


	bool Network::ConnectPorts( const std::string & producer, const std::string & consumer )
	{
		AssertFlowControlNotNull();
		_flowControl->NetworkTopologyChanged();

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
		_flowControl->NetworkTopologyChanged();

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
		_flowControl->NetworkTopologyChanged();

		ProcessingsMap::iterator it;
		// pass trough all the processing
		for( it=_processings.begin(); it!=_processings.end(); it++)
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

	std::string Network::GetLastIdentifier( const std::string& str )
	{
		return str.substr( PositionOfLastIdentifier(str)+1 );
	}

	std::string Network::GetProcessingIdentifier( const std::string& str )
	{
		std::size_t length = PositionOfLastIdentifier(str)  - PositionOfProcessingIdentifier(str);
		return str.substr( PositionOfProcessingIdentifier(str), length);
	}

	InPort & Network::GetInPortByCompleteName( const std::string & name )
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetInPorts().Get( GetLastIdentifier(name) );
	}

	OutPort & Network::GetOutPortByCompleteName( const std::string & name )
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetOutPorts().Get( GetLastIdentifier(name) );
	}

	InControl & Network::GetInControlByCompleteName( const std::string & name )
	{
		Processing& proc = GetProcessing( GetProcessingIdentifier(name) );
		return proc.GetInControls().Get( GetLastIdentifier(name) );
	}

	OutControl & Network::GetOutControlByCompleteName( const std::string & name )
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
				_nodes.push_back(node);
				_flowControl->NetworkTopologyChanged();
			}
			return *out.GetNode();
	}

	NodeBase* Network::CreateAudioNodeWithDefaultStreamBuffer()
	{
		//@todo
		typedef CircularStreamImpl<TData> DefaultStreamBuffer;
		NodeBase * node = new NodeTmpl<Audio, DefaultStreamBuffer>;
		_nodesToConfigure.push_back( node );
		return node;
		//return new AudioNodeTmpl;
	}
	
	void Network::ConfigureAllNodes()
	{
		AssertFlowControlNotNull();

		Nodes::iterator it = _nodesToConfigure.begin();
		while ( it!=_nodesToConfigure.end() )
			_flowControl->ConfigureNode(**it++);
		
		_nodesToConfigure.clear();
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
		_flowControl->DoProcessings();
	}

	Network::ProcessingsMap::iterator Network::BeginProcessings()
	{
		return _processings.begin();
	}

	Network::ProcessingsMap::iterator Network::EndProcessings()
	{
		return _processings.end();
	}
	Network::ProcessingsMap::const_iterator Network::BeginProcessings() const
	{
		return _processings.begin();
	}

	Network::ProcessingsMap::const_iterator Network::EndProcessings() const
	{
		return _processings.end();
	}

	Network::Nodes::iterator Network::BeginNodes()
	{
		return _nodes.begin();
	}

	Network::Nodes::iterator Network::EndNodes()
	{
		return _nodes.end();
	}

	Network::Nodes::const_iterator Network::BeginNodes() const
	{
		return _nodes.begin();
	}

	Network::Nodes::const_iterator Network::EndNodes() const
	{
		return _nodes.end();
	}


}
