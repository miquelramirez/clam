#include "CLAMRemoteController.hxx"

unsigned int numports=4;

namespace CLAM
{

CLAMRemoteController::CLAMRemoteController()
{
	CreateStructure();
	
	ProcessPorts();
}

CLAMRemoteController::~CLAMRemoteController()
{
	std::cerr << " DELETED" << std::endl;
	for (LADSPAInControlList::iterator it=mInControlList.begin(); it!=mInControlList.end(); it++)
		delete it->processing;
	for (std::vector<OSCSender*>::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
		delete *it;
	std::cerr << " DELETED OK!" << std::endl;
}

void CLAMRemoteController::CreateStructure()
{
	std::cerr << " constructor" << std::endl;

	std::string xmlfile="/home/xoliver/CLAM/build/Examples/CLAMRemoteController/remoteList.xml";

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

	LADSPAInfo<ExternInControl> info;
	mLastValues=new TData[mRemoteControlList.GetList().size()];
	int i=0;
	for ( std::list<RemoteControlPair>::iterator it=mRemoteControlList.GetList().begin(); it!=mRemoteControlList.GetList().end(); it++)
	{
		//Create ExternInControl
		info.name=it->GetName();
		info.processing=new ExternInControl( it->GetControlConfig() );
		mInControlList.push_back(info);

		//Create OSCSender
		mSenderList.push_back( new OSCSender( it->GetSenderConfig() ) );		
		mLastValues[i++]=0.0;
	}
}

void CLAMRemoteController::ProcessPorts()
{
	//Create the two input channels
	mPortList[0].name="LeftIn";
	mPortList[0].processing=NULL;
	mPortList[1].name="RightIn";
	mPortList[1].processing=NULL;

	//Create the two output channels
	mPortList[2].name="LeftOut";
	mPortList[2].processing=NULL;
	mPortList[3].name="RightOut";
	mPortList[3].processing=NULL;
}

void CLAMRemoteController::FillPortInfo( LADSPA_PortDescriptor* descriptors, char** names, LADSPA_PortRangeHint* rangehints )
{
	int currentport=0;

	//Manage InPorts
	descriptors[currentport] = (LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO);
	names[currentport] = dupstr( mPortList[currentport].name.c_str() );
	rangehints[currentport].HintDescriptor = 0;
	currentport++;

	descriptors[currentport] = (LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO);
	names[currentport] = dupstr( mPortList[currentport].name.c_str() );
	rangehints[currentport].HintDescriptor = 0;
	currentport++;

	//Manage OutPorts
	descriptors[currentport] = (LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO);
	names[currentport] = dupstr( mPortList[currentport].name.c_str() );
	rangehints[currentport].HintDescriptor = 0;
	currentport++;

	descriptors[currentport] = (LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO);
	names[currentport] = dupstr( mPortList[currentport].name.c_str() );
	rangehints[currentport].HintDescriptor = 0;
	currentport++;

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
}

void CLAMRemoteController::Run( unsigned long nsamples )
{
	ProcessInControlValues();
	
	CopyInPortsToOutputPorts(nsamples);
}

void CLAMRemoteController::CopyInPortsToOutputPorts( unsigned long nsamples )
{
	for (unsigned long i=0; i<nsamples; i++)
	{
		//Simply copy input buffers to output ones
		mPortList[2].dataBuffer[i]=mPortList[0].dataBuffer[i];
		mPortList[3].dataBuffer[i]=mPortList[1].dataBuffer[i];
	}
}

void CLAMRemoteController::ProcessInControlValues()
{
	int i=0;
	for (LADSPAInControlList::iterator it=mInControlList.begin(); it!=mInControlList.end(); it++)
	{
		if ( (TData) *(it->dataBuffer) != mLastValues[i] )
		{
			mSenderList.at(i)->GetInControl("input").DoControl( (TData) *(it->dataBuffer) );
			mLastValues[i]= (TData) *(it->dataBuffer);
		}
		i++;
	}
}

void CLAMRemoteController::ConnectTo(unsigned long port, LADSPA_Data * data)
{
	if ( port<numports )
		mPortList[port].dataBuffer=data;
	else //Input controls
		mInControlList.at( port-numports ).dataBuffer=data;
}

} //end namespace CLAM

