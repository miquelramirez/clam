#include "NetworkLADSPAPlugin.hxx"

namespace CLAM
{

NetworkLADSPAPlugin::NetworkLADSPAPlugin()
{
	mNet=new Network();

	mClamBufferSize=512;
	mExternBufferSize=mClamBufferSize;
	GetNetwork().SetName("Testing name");

	std::cerr << " constructor" << std::endl;

	char* xmlfile=getenv("CLAM_NETWORK_PLUGIN_PATH");
	if (xmlfile==NULL)
	{
		std::cerr << "CLAM::NetworkLADSPAPlugin WARNING: no network file specified. Plugin not loaded" << std::endl;
		std::cerr << "                    --> Do \"export CLAM_NETWORK_PLUGIN_PATH=/..path../file.xml\"" << std::endl;
		return;
	}
	
	try
	{
		XmlStorage::Restore( GetNetwork(), xmlfile);	//"genwire.xml");	
	}
	catch ( XmlStorageErr err)
	{
		std::cerr << "CLAM::NetworkLADSPAPlugin WARNING: error opening file <" << xmlfile << "> . Plugin not loaded" <<std::endl;
		return;
	}
	
	ProcessInputPorts();
	ProcessOutputPorts();
	ProcessInputControls();
	ProcessOutputControls();
}

NetworkLADSPAPlugin::~NetworkLADSPAPlugin()
{
	delete mNet;

	std::cerr << " DELETED" << std::endl;

}

void NetworkLADSPAPlugin::Activate()
{
	GetNetwork().Start();
}

void NetworkLADSPAPlugin::Deactivate()
{
	GetNetwork().Stop();
}

void NetworkLADSPAPlugin::ProcessInputPorts()
{
	CLAM_ASSERT( mReceiverList.empty(), "NetworkLADSPAPlugin::ProcessInputPorts() : there are already registered input ports");

	LADSPAInfo<ExternGenerator> info;

	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=GetNetwork().BeginProcessings(); it!=GetNetwork().EndProcessings(); it++)
	{
		if (std::string("ExternGenerator")==std::string(it->second->GetClassName()))
		{
			//Store Processing name
			info.name=it->first;
			
			//Get Processing address
			info.processing=(ExternGenerator*)it->second;
			info.processing->SetFrameAndHopSize( mExternBufferSize );

			//Add the info 
			mReceiverList.push_back(info);
		}
	}
}

void NetworkLADSPAPlugin::ProcessOutputPorts()
{
	CLAM_ASSERT( mSenderList.empty(), "NetworkLADSPAPlugin::ProcessInputPorts() : there are already registered output ports");

	LADSPAInfo<ExternSink> info;

	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=GetNetwork().BeginProcessings(); it!=GetNetwork().EndProcessings(); it++)
	{
		if (std::string("ExternSink")==std::string(it->second->GetClassName()))
		{
			//Store Processing name
			info.name=it->first;
			
			//Get Processing address
			info.processing=(ExternSink*)it->second;
			info.processing->SetFrameAndHopSize( mExternBufferSize );

			//Add the info 
			mSenderList.push_back(info);
		}
	}
}

void NetworkLADSPAPlugin::ProcessInputControls()
{
	CLAM_ASSERT( mInControlList.empty(), "NetworkLADSPAPlugin::ProcessInputControls() : there are already registered controls");

	LADSPAInfo<ExternInControl> info;

	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=GetNetwork().BeginProcessings(); it!=GetNetwork().EndProcessings(); it++)
	{
		if (std::string("ExternInControl")==std::string(it->second->GetClassName()))
		{
			//Store Processing name
			info.name=it->first;
			
			//Get Processing address
			info.processing=(ExternInControl*)it->second;

			//Add the info 
			mInControlList.push_back(info);
		}
	}
}

void NetworkLADSPAPlugin::ProcessOutputControls()
{
	CLAM_ASSERT( mOutControlList.empty(), "NetworkLADSPAPlugin::ProcessOutputControls() : there are already registered controls");

	LADSPAInfo<ExternOutControl> info;

	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=GetNetwork().BeginProcessings(); it!=GetNetwork().EndProcessings(); it++)
	{
		if (std::string("ExternOutControl")==std::string(it->second->GetClassName()))
		{
			//Store Processing name
			info.name=it->first;
			
			//Get Processing address
			info.processing=(ExternOutControl*)it->second;

			//Add the info 
			mOutControlList.push_back(info);
		}
	}
}


void NetworkLADSPAPlugin::UpdatePortFrameAndHopSize()
{
	//ExternGenerators
	for (LADSPAInPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
		it->processing->SetFrameAndHopSize( mExternBufferSize );

	//ExternSinks
	for (LADSPAOutPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
		it->processing->SetFrameAndHopSize( mExternBufferSize );
}

void NetworkLADSPAPlugin::FillPortInfo( LADSPA_PortDescriptor* descriptors, char** names, LADSPA_PortRangeHint* rangehints )
{
	int currentport=0;

	//Manage InPorts (ExternGenerators)
	for (LADSPAInPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
	{
		descriptors[currentport] = (LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO);
		names[currentport] = dupstr( it->name.c_str() );
		rangehints[currentport].HintDescriptor = 0;
		currentport++;
	}

	//Manage OutPorts (ExternSinks)
	for (LADSPAOutPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
	{
		descriptors[currentport] = (LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO);
		names[currentport] = dupstr( it->name.c_str() );
		rangehints[currentport].HintDescriptor = 0;
		currentport++;
	}

	//Manage InControls (ExternInControls)
	for (LADSPAInControlList::iterator it=mInControlList.begin(); it!=mInControlList.end(); it++)
	{
		descriptors[currentport] = (LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL);
		names[currentport] = dupstr( it->name.c_str() );

		//Obté processingConfig, i defineix cada param
		ExternInControlConfig& conf=const_cast<ExternInControlConfig&>(
						dynamic_cast<const ExternInControlConfig&>(
							it->processing->GetConfig() ));
		
		rangehints[currentport].LowerBound=(LADSPA_Data)conf.GetMinValue();
		rangehints[currentport].UpperBound=(LADSPA_Data)conf.GetMaxValue();
		rangehints[currentport].HintDescriptor = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_MIDDLE);
		currentport++;
	}
	
	//Manage OutControls (ExternOutControls)
	// (Please note that not all the LADSPA hosts make use of these kind of ports)
	for (LADSPAOutControlList::iterator it=mOutControlList.begin(); it!=mOutControlList.end(); it++)
	{
		descriptors[currentport] = (LADSPA_PORT_OUTPUT | LADSPA_PORT_CONTROL);
		names[currentport] = dupstr( it->name.c_str() );
		rangehints[currentport].LowerBound=(LADSPA_Data)0;
		rangehints[currentport].UpperBound=(LADSPA_Data)1000;
		rangehints[currentport].HintDescriptor = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE);
		currentport++;
	}

}

void NetworkLADSPAPlugin::Run( unsigned long nsamples )
{
	//Check current buffer size of ports, to make sure everything fits!
	// if it isn't so, upgrade Frame and Hop sizes (vital)
	if (nsamples!=mExternBufferSize)
	{
		mExternBufferSize=nsamples;
		UpdatePortFrameAndHopSize();
	}		
	
	ProcessInControlValues();
	
	CopyLadspaBuffersToGenerators(nsamples);

	//Do() as much as it is needed
	for (int stepcount=0; stepcount < (int(mExternBufferSize)/int(mClamBufferSize)); stepcount++)
		GetNetwork().Do();

	CopySinksToLadspaBuffers(nsamples);
	ProcessOutControlValues();
}

void NetworkLADSPAPlugin::ProcessInControlValues()
{
	for (LADSPAInControlList::iterator it=mInControlList.begin(); it!=mInControlList.end(); it++)
		it->processing->Do( (float) *(it->dataBuffer) );
}

void NetworkLADSPAPlugin::ProcessOutControlValues()
{
	for (LADSPAOutControlList::iterator it=mOutControlList.begin(); it!=mOutControlList.end(); it++)
		*(it->dataBuffer)=it->processing->GetControlValue();
}


void NetworkLADSPAPlugin::CopyLadspaBuffersToGenerators(const unsigned long nframes)
{
	for (LADSPAInPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
		it->processing->Do( (TData*) it->dataBuffer, nframes );
}

void NetworkLADSPAPlugin::CopySinksToLadspaBuffers(const unsigned long nframes)
{
	for (LADSPAOutPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
		it->processing->Do( (TData*) it->dataBuffer, nframes);	
}

void NetworkLADSPAPlugin::ConnectTo(unsigned long port, LADSPA_Data * data)
{
	if ( port <= mReceiverList.size()-1 ) //Input port
		mReceiverList.at( port ).dataBuffer=data;
	else if ( port <= mReceiverList.size() + mSenderList.size() -1) //Output port
		mSenderList.at( port-mReceiverList.size() ).dataBuffer=data;
	else if ( port <= mReceiverList.size() + mSenderList.size() + mInControlList.size() -1) //Input control
		mInControlList.at( port-mReceiverList.size()-mSenderList.size() ).dataBuffer=data;
	else //Output control
		mOutControlList.at( port-mReceiverList.size()-mSenderList.size()-mInControlList.size() ).dataBuffer=data;
}

} //end namespace CLAM

