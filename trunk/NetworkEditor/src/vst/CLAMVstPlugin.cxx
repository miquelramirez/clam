#ifndef __CLAMTest_H
#include "CLAMVstPlugin.hpp"
#endif

#include <CLAM/XMLStorage.hxx>
#include <iostream>
#include <sstream>
#include <cmath>

#include "SimpleOscillator.hxx" //TODO remove
#include "Oscillator.hxx" //TODO remove
//#define NO_NETWORK_FILE

using namespace CLAM;

//char* xmlfile="wire.xml"; //2
//char* xmlfile="externSMSmess.xml"; //1
char* xmlfile="../../example-data/simpleModulator.clamnetwork"; 
//char* xmlfile="inputMultiplier.xml"; //2


//---------------------------------------------------------------------
CLAMTest::CLAMTest (audioMasterCallback audioMaster)
	: AudioEffectX (audioMaster, 1, GetNumberOfParameters(xmlfile) )	// 1 program, 1 parameter only
{
	mNet=new Network();
	mClamBufferSize=512;
	mExternBufferSize=mClamBufferSize;

	try
	{

#ifdef NO_NETWORK_FILE
		FillNetwork(); //Testing interest
#else 
		XmlStorage::Restore( *mNet, xmlfile );
	}
	catch ( std::exception& err)
	{
		std::cerr << "CLAMTest WARNING: error opening file <"
			<< xmlfile << "> . Plugin not loaded" << std::endl
		        << "exception.what() " << err.what() << std::endl;
		FillNetwork();
	}
#endif	
	GetNetwork().Start();

	ProcessInputPorts();
	ProcessOutputPorts();
	ProcessInputControls();

	UpdatePortFrameAndHopSize();

	setNumInputs (2);			// stereo in
	setNumOutputs (2);			// stereo out
	setUniqueID ('CLAM');			// identify
	//canMono ();				// makes sense to feed both inputs with the same signal
	strcpy (programName, "Default");	// default program name
}

//---------------------------------------------------------------------
CLAMTest::~CLAMTest ()
{
	GetNetwork().Stop();
	delete mNet;
}

//---------------------------------------------------------------------
int CLAMTest::GetNumberOfParameters( char* file )
{
#ifdef NO_NETWORK_FILE
	return 1; 
#endif
	Network net;
	int count=0;
	
	try
	{
		XmlStorage::Restore( net, file );
	}
	catch ( XmlStorageErr err)
	{
		std::cerr << "CLAMTest WARNING: error opening file <"
			<< file << "> . Plugin not loaded" <<std::endl;
		return -2;
	}

	for (Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
	{
		std::cout << "proc: " << it->second->GetClassName() << std::endl;
		if (std::string("ControlSource")==std::string(it->second->GetClassName())) {
			count++;
			std::cout << "Found ControlSource: " << it->second->GetClassName() << std::endl;
			
		}
	}

	std::cout << "returning : " << count << std::endl;
	return count;
}
	
//---------------------------------------------------------------------
void CLAMTest::setProgramName (char *name)
{
	strcpy (programName, name);
}

//---------------------------------------------------------------------
void CLAMTest::getProgramName (char *name)
{
	strcpy (name, programName);
}

//---------------------------------------------------------------------
void CLAMTest::setParameter (long index, float value)
{
	float realval = value * ( mInControlList.at(index).max - mInControlList.at(index).min ) - fabs( mInControlList.at(index).min );

	mInControlList.at(index).lastvalue = realval;
	mInControlList.at(index).processing->Do(realval);
}

//---------------------------------------------------------------------
float CLAMTest::getParameter (long index)
{
	return mInControlList.at(index).lastvalue;
}

//---------------------------------------------------------------------
void CLAMTest::getParameterName (long index, char *label)
{
	strcpy ( label, mInControlList.at(index).name.c_str() ); 
}

//---------------------------------------------------------------------
void CLAMTest::getParameterDisplay (long index, char *text)
{
	//TODO: alguna mena de switch() per distingir entre dB, Hz, etc...
	float2string ( mInControlList.at(index).lastvalue, text);
}

//---------------------------------------------------------------------
void CLAMTest::getParameterLabel(long index, char *label)
{
	//strcpy (label, "db");
	ControlSourceConfig& conf=const_cast<ControlSourceConfig&>(
		dynamic_cast<const ControlSourceConfig&>(
			mInControlList.at(index).processing->GetConfig() ));
	strcpy (label, conf.GetUnitName().c_str() );
}

//---------------------------------------------------------------------
bool CLAMTest::getEffectName (char* name)
{
	strcpy (name, "CLAMTest-effectname");
	return true;
}

//---------------------------------------------------------------------
bool CLAMTest::getProductString (char* text)
{
	strcpy (text, "CLAMTest-productstring");
	return true;
}

//---------------------------------------------------------------------
bool CLAMTest::getVendorString (char* text)
{
	strcpy (text, "clam-devel");
	return true;
}

//---------------------------------------------------------------------
void CLAMTest::process (float **inputs, float **outputs, long sampleFrames)
{
	const bool mono = true;
	float *in1  =  inputs[0];
	float *out1 = outputs[0];
	float *in2  = 0;
	float *out2 = 0;
	if (! mono) 
	{
		in2  =  inputs[1];
		out2 = outputs[1];
	}

	if (sampleFrames!=mExternBufferSize)
	{
		std::cerr << "Switching to "<< sampleFrames <<std::endl;
		mExternBufferSize=sampleFrames;
		UpdatePortFrameAndHopSize();
	}


	//buffer2clam
	mReceiverList.at(0).processing->SetExternalBuffer( in1 , sampleFrames );
	if (! mono) mReceiverList.at(1).processing->SetExternalBuffer( in2 , sampleFrames );
	
	//clam2buffer
	mSenderList.at(0).processing->SetExternalBuffer( out1 , sampleFrames );
	if (! mono) mSenderList.at(1).processing->SetExternalBuffer( out2 , sampleFrames );

	GetNetwork().Do();
}

////////////////////////////////////////////////////////////////////////////////////
void CLAMTest::FillNetwork()
{
	CLAM::SimpleOscillator();
	CLAM::Oscillator();
	GetNetwork().SetName("VST net");
	GetNetwork().AddProcessing("AudioSink", "AudioSink");
	GetNetwork().AddProcessing("AudioSource", "AudioSource");
	GetNetwork().AddProcessing("Oscillator", "SimpleOscillator");
	GetNetwork().AddProcessing("Modulator", "Oscillator");
	GetNetwork().AddProcessing("ControlSource_0", "ControlSource");
	GetNetwork().ConnectPorts("AudioSource.AudioOut","Modulator.Input Phase Modulation");
	GetNetwork().ConnectPorts("Modulator.Audio Output","AudioSink.AudioIn");
	GetNetwork().ConnectPorts("Oscillator.Audio Output","Modulator.Input Frequency Modulation");
	GetNetwork().ConnectControls("ControlSource_0.output","Modulator.Pitch");
}

void CLAMTest::ProcessInputControls()
{
	CLAM_ASSERT( mInControlList.empty(), "CLAMTest::ProcessInputControls() : there are already registered controls");

	ExternControlInfo info;

	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=GetNetwork().BeginProcessings(); it!=GetNetwork().EndProcessings(); it++)
	{
		if (std::string("ControlSource")==std::string(it->second->GetClassName()))
		{
			ControlSourceConfig& conf=const_cast<ControlSourceConfig&>(
				dynamic_cast<const ControlSourceConfig&>(
					it->second->GetConfig() ));

			//Store Processing name
			info.name=it->first;
			
			//Get Processing address
			info.processing=(ControlSource*)it->second;

			//Store range data
			info.min=conf.GetMinValue();
			info.max=conf.GetMaxValue();
			info.lastvalue=info.max;
			
			//Add the info 
			mInControlList.push_back(info);
		}
	}
}

void CLAMTest::ProcessInputPorts()
{
	CLAM_ASSERT( mReceiverList.empty(), "CLAMTest::ProcessInputPorts() : there are already registered input ports");

	DataInfo<AudioSource> info;

	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=GetNetwork().BeginProcessings(); it!=GetNetwork().EndProcessings(); it++)
	{
		if (std::string("AudioSource")==std::string(it->second->GetClassName()))
		{
			//Store Processing name
			info.name=it->first;
			
			//Get Processing address
			info.processing=(AudioSource*)it->second;
			info.processing->SetFrameAndHopSize( mExternBufferSize );

			//Add the info 
			mReceiverList.push_back(info);
		}
	}
}

void CLAMTest::ProcessOutputPorts()
{
	CLAM_ASSERT( mSenderList.empty(), "CLAMTest::ProcessInputPorts() : there are already registered output ports");

	DataInfo<AudioSink> info;

	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=GetNetwork().BeginProcessings(); it!=GetNetwork().EndProcessings(); it++)
	{
		if (std::string("AudioSink")==std::string(it->second->GetClassName()))
		{
			//Store Processing name
			info.name=it->first;
			
			//Get Processing address
			info.processing=(AudioSink*)it->second;
			info.processing->SetFrameAndHopSize( mExternBufferSize );

			//Add the info 
			mSenderList.push_back(info);
		}
	}
}

void CLAMTest::UpdatePortFrameAndHopSize()
{
	//AudioSources
	for (VSTInPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
		it->processing->SetFrameAndHopSize( mExternBufferSize );
		
	//AudioSinks
	for (VSTOutPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
		it->processing->SetFrameAndHopSize( mExternBufferSize );
}
									
