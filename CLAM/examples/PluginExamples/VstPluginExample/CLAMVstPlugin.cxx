#ifndef __CLAMTest_H
#include "CLAMVstPlugin.hpp"
#endif

#include <CLAM/XMLStorage.hxx>
#include <iostream>
#include <sstream>
#include <cmath>

#include <CLAM/EmbeddedFile.hxx>
#include "SimpleOscillator.hxx" //TODO remove
#include "Oscillator.hxx" //TODO remove

CLAM_EMBEDDED_FILE(embededNetwork,"genwire.xml")

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	CLAM::VstNetworkExporter * effect = new CLAM::VstNetworkExporter(audioMaster);
	if (effect->ok()) return effect;
	delete effect;
	return 0;
}

namespace CLAM
{

// VST interface: Metadata

bool VstNetworkExporter::getEffectName (char* name)
{
	strncpy (name, _effectName.c_str(), kVstMaxEffectNameLen);
	return true;
}

bool VstNetworkExporter::getProductString (char* text)
{
	strncpy (text, _productString.c_str(), kVstMaxProductStrLen);
	return true;
}

bool VstNetworkExporter::getVendorString (char* text)
{
	strncpy (text, _vendor.c_str(), kVstMaxVendorStrLen);
	return true;
}

VstInt32 VstNetworkExporter::getVendorVersion ()
{
	return _version;
}

// VST interface: Program management

void VstNetworkExporter::setProgramName (char *name)
{
	strncpy (programName, name, kVstMaxProgNameLen);
}

void VstNetworkExporter::getProgramName (char *name)
{
	strncpy (name, programName, kVstMaxProgNameLen);
}


// VST interface: Parameter management

void VstNetworkExporter::setParameter (long index, float value)
{
	float realval = value * ( mInControlList.at(index).max - mInControlList.at(index).min ) - fabs( mInControlList.at(index).min );

	mInControlList.at(index).lastvalue = realval;
	mInControlList.at(index).processing->Do(realval);
}

float VstNetworkExporter::getParameter (long index)
{
	return mInControlList.at(index).lastvalue;
}

void VstNetworkExporter::getParameterName (long index, char *label)
{
	strncpy ( label, mInControlList.at(index).name.c_str(), kVstMaxParamStrLen ); 
}

void VstNetworkExporter::getParameterDisplay (long index, char *text)
{
	float2string ( mInControlList.at(index).lastvalue, text, kVstMaxParamStrLen);
}

void VstNetworkExporter::getParameterLabel(long index, char *label)
{
	ControlSourceConfig& conf=const_cast<ControlSourceConfig&>(
		dynamic_cast<const ControlSourceConfig&>(
			mInControlList.at(index).processing->GetConfig() ));
	strncpy (label, conf.GetUnitName().c_str(), kVstMaxParamStrLen);
}

//---------------------------------------------------------------------
VstNetworkExporter::VstNetworkExporter (audioMasterCallback audioMaster)
	: AudioEffectX (audioMaster, 1 /*nPrograms*/, GetNumberOfParameters(embededNetwork) )
	, _effectName("CLAMTest-effectname")
	, _productString("CLAMTest-productstring")
	, _vendor("clam-devel")
	, _version(1000)
{
	std::cout << "== Constructor" << std::endl;
	mNet=new Network;
	mClamBufferSize=512;
	mExternBufferSize=mClamBufferSize;

	try
	{
		std::istringstream xmlfile(embededNetwork);
		XmlStorage::Restore( *mNet, xmlfile );
	}
	catch ( std::exception& err)
	{
		std::cerr << "VstNetworkExporter WARNING: error parsing embeded network. "
				"Plugin not loaded" << std::endl
			<< "exception.what() " << err.what() << std::endl;
		FillNetwork();
	}
	GetNetwork().Start();

	ProcessInputPorts();
	ProcessOutputPorts();
	ProcessInputControls();

	UpdatePortFrameAndHopSize();

	setNumInputs (2);			// stereo in
	setNumOutputs (2);			// stereo out
	setUniqueID ('CLAM');		// identify TODO: Get one
	//canMono ();				// makes sense to feed both inputs with the same signal
	strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
}

//---------------------------------------------------------------------
VstNetworkExporter::~VstNetworkExporter ()
{
	GetNetwork().Stop();
	delete mNet;
}

//---------------------------------------------------------------------
int VstNetworkExporter::GetNumberOfParameters( const char* networkXmlContent )
{
	Network net;
	int count=0;
	
	try
	{
		std::istringstream file(embededNetwork);
		XmlStorage::Restore( net, file );
	}
	catch ( XmlStorageErr err)
	{
		std::cerr << "VstNetworkExporter WARNING: error loading embedded network. "
			"Plugin not loaded" <<std::endl;
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

void VstNetworkExporter::process (float **inputs, float **outputs, long sampleFrames)
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
void VstNetworkExporter::FillNetwork()
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

void VstNetworkExporter::ProcessInputControls()
{
	CLAM_ASSERT( mInControlList.empty(), "VstNetworkExporter::ProcessInputControls() : there are already registered controls");

	ExternControlInfo info;

	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=GetNetwork().BeginProcessings(); it!=GetNetwork().EndProcessings(); it++)
	{
		if (std::string(it->second->GetClassName()) != "ControlSource") continue;
		const ControlSourceConfig & conf= dynamic_cast<const ControlSourceConfig&>(it->second->GetConfig() );

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

void VstNetworkExporter::ProcessInputPorts()
{
	CLAM_ASSERT( mReceiverList.empty(), "VstNetworkExporter::ProcessInputPorts() : there are already registered input ports");

	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=GetNetwork().BeginProcessings(); it!=GetNetwork().EndProcessings(); it++)
	{
		if (std::string(it->second->GetClassName()) != "AudioSource") continue;
		DataInfo<AudioSource> info;
		//Store Processing name
		info.name=it->first;
		
		//Get Processing address
		info.processing=(AudioSource*)it->second;
		info.processing->SetFrameAndHopSize( mExternBufferSize );

		//Add the info 
		mReceiverList.push_back(info);
	}
}

void VstNetworkExporter::ProcessOutputPorts()
{
	CLAM_ASSERT( mSenderList.empty(), "VstNetworkExporter::ProcessInputPorts() : there are already registered output ports");

	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=GetNetwork().BeginProcessings(); it!=GetNetwork().EndProcessings(); it++)
	{
		if (std::string(it->second->GetClassName()) != "AudioSink") continue;
		//Store Processing name
		DataInfo<AudioSink> info;
		info.name=it->first;
		//Get Processing address
		info.processing=(AudioSink*)it->second;
		info.processing->SetFrameAndHopSize( mExternBufferSize );

		//Add the info 
		mSenderList.push_back(info);
	}
}

void VstNetworkExporter::UpdatePortFrameAndHopSize()
{
	//AudioSources
	for (VSTInPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
		it->processing->SetFrameAndHopSize( mExternBufferSize );
		
	//AudioSinks
	for (VSTOutPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
		it->processing->SetFrameAndHopSize( mExternBufferSize );
}

} // CLAM


