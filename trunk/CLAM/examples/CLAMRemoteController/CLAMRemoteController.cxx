#include "CLAMRemoteController.hxx"

namespace CLAM
{

CLAMRemoteController::CLAMRemoteController()
{
	CreateNetwork();
	
	ProcessInputPorts();
	ProcessOutputPorts();
	ProcessInputControls();
	ProcessOutputControls();
}

CLAMRemoteController::~CLAMRemoteController()
{
	delete mNet;

	std::cerr << " DELETED" << std::endl;
}

void CLAMRemoteController::CreateNetwork()
{
	mNet=new Network();

	mClamBufferSize=512;
	mExternBufferSize=512;
	GetNetwork().AddFlowControl( new PushFlowControl( mClamBufferSize ) );
	GetNetwork().SetName("Testing name");

	std::cerr << " constructor" << std::endl;

	std::string xmlfile="remoteList.xml";

	try
	{
		XmlStorage::Restore( mRemoteControlList, xmlfile);
	}
	catch ( XmlStorageErr err)
	{
		std::cerr << "CLAM::CLAMRemoteController WARNING: error opening configuration file <"
			<< xmlfile << "> . Plugin not loaded" <<std::endl;
		return;
	}

	for ( std::list<RemoteControlPair>::iterator it=mRemoteControlList.GetList().begin(); it!=mRemoteControlList.GetList().end(); it++)
	{
		std::cerr << " LISTING!! : " << it->GetName() << std::endl;
		//Create ExternInControl
		mNet->AddProcessing( it->GetName() , new ExternInControl( it->GetControlConfig() ) );

		//Create OSCSender
		std::string oscname=it->GetName();
		oscname.append("_osc");
		mNet->AddProcessing( oscname , new OSCSender( it->GetSenderConfig() ) );

		//Link them
		oscname.append(".input");
		std::string controlname=it->GetName();
		controlname.append(".output");
		mNet->ConnectControls( controlname, oscname );
	}

	//GetNetwork().AddProcessing( "LeftIn", new ExternGenerator() );
	//GetNetwork().AddProcessing( "RightIn", new ExternGenerator() );
	//GetNetwork().AddProcessing( "LeftOut", new ExternSink() );
	//GetNetwork().AddProcessing( "RightOut", new ExternSink() );
	//GetNetwork().ConnectPorts( "LeftIn.AudioOut", "LeftOut.AudioIn");
	//GetNetwork().ConnectPorts( "RightIn.AudioOut", "RightOut.AudioIn");

}

void CLAMRemoteController::Activate()
{
	GetNetwork().Start();
}

void CLAMRemoteController::Deactivate()
{
	GetNetwork().Stop();
}

void CLAMRemoteController::ProcessInputPorts()
{
	CLAM_ASSERT( mReceiverList.empty(), "CLAMRemoteController::ProcessInputPorts() : there are already registered input ports");

	LADSPAInfo<ExternGenerator> info;

	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=GetNetwork().BeginProcessings(); it!=GetNetwork().EndProcessings(); it++)
	{
		if (std::string("ExternGenerator")==std::string(it->second->GetClassName()))
		{
			std::cerr << " GENERATOR FOUND!" << std::endl;

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

void CLAMRemoteController::ProcessOutputPorts()
{
	CLAM_ASSERT( mSenderList.empty(), "CLAMRemoteController::ProcessInputPorts() : there are already registered output ports");

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

void CLAMRemoteController::ProcessInputControls()
{
	CLAM_ASSERT( mInControlList.empty(), "CLAMRemoteController::ProcessInputControls() : there are already registered controls");

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

void CLAMRemoteController::ProcessOutputControls()
{
	CLAM_ASSERT( mOutControlList.empty(), "CLAMRemoteController::ProcessOutputControls() : there are already registered controls");

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


void CLAMRemoteController::UpdatePortFrameAndHopSize()
{
	//ExternGenerators
	for (LADSPAInPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
		it->processing->SetFrameAndHopSize( mExternBufferSize );

	//ExternSinks
	for (LADSPAOutPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
		it->processing->SetFrameAndHopSize( mExternBufferSize );
}

void CLAMRemoteController::FillPortInfo( LADSPA_PortDescriptor* descriptors, char** names, LADSPA_PortRangeHint* rangehints )
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

void CLAMRemoteController::Run( unsigned long nsamples )
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

void CLAMRemoteController::ProcessInControlValues()
{
	for (LADSPAInControlList::iterator it=mInControlList.begin(); it!=mInControlList.end(); it++)
		it->processing->Do( (float) *(it->dataBuffer) );
}

void CLAMRemoteController::ProcessOutControlValues()
{
	for (LADSPAOutControlList::iterator it=mOutControlList.begin(); it!=mOutControlList.end(); it++)
		*(it->dataBuffer)=it->processing->GetControlValue();
}


void CLAMRemoteController::CopyLadspaBuffersToGenerators(const unsigned long nframes)
{
	for (LADSPAInPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
		it->processing->Do( (TData*) it->dataBuffer, nframes );
}

void CLAMRemoteController::CopySinksToLadspaBuffers(const unsigned long nframes)
{
	for (LADSPAOutPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
		it->processing->Do( (TData*) it->dataBuffer, nframes);	
}

void CLAMRemoteController::ConnectTo(unsigned long port, LADSPA_Data * data)
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
