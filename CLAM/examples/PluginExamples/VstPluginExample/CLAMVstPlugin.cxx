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
	CLAM::VstNetworkExporter * effect = new CLAM::VstNetworkExporter(
		audioMaster,
		"CLAM effect",
		"Example of CLAM based VST plugin",
		"CLAM Project",
		1000
	);
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
	ExternControlInfo & controlInfo = mInControlList[index];
	float realval = value * ( controlInfo.max - controlInfo.min ) - fabs( controlInfo.min );

	controlInfo.lastvalue = realval;
	controlInfo.processing->Do(realval);
}

float VstNetworkExporter::getParameter (long index)
{
	return mInControlList[index].lastvalue;
}

void VstNetworkExporter::getParameterName (long index, char *label)
{
	strncpy ( label, mInControlList[index].name.c_str(), kVstMaxParamStrLen ); 
}

void VstNetworkExporter::getParameterDisplay (long index, char *text)
{
	float2string ( mInControlList[index].lastvalue, text, kVstMaxParamStrLen);
}

void VstNetworkExporter::getParameterLabel(long index, char *label)
{
	ControlSourceConfig& conf=const_cast<ControlSourceConfig&>(
		dynamic_cast<const ControlSourceConfig&>(
			mInControlList[index].processing->GetConfig() ));
	strncpy (label, conf.GetUnitName().c_str(), kVstMaxParamStrLen);
}

//---------------------------------------------------------------------
VstNetworkExporter::VstNetworkExporter (
		audioMasterCallback audioMaster,
		const std::string & effectName,
		const std::string & productString,
		const std::string & vendor,
		int version
	)
	: AudioEffectX (audioMaster, 1 /*nPrograms*/, GetNumberOfParameters(embededNetwork) )
	, _effectName(effectName)
	, _productString(productString)
	, _vendor(vendor)
	, _version(version)
{
	std::cout << "== Constructor" << std::endl;
	mClamBufferSize=512;
	mExternBufferSize=mClamBufferSize;

	try
	{
		std::istringstream xmlfile(embededNetwork);
		XmlStorage::Restore( _network, xmlfile );
	}
	catch ( std::exception& err)
	{
		std::cerr << "VstNetworkExporter WARNING: error parsing embeded network. "
				"Plugin not loaded" << std::endl
			<< "exception.what() " << err.what() << std::endl;
		FillNetwork();
	}
	_network.Start();
	LocateConnections();

	setNumInputs (mReceiverList.size());
	setNumOutputs (mSenderList.size());
	setUniqueID (CCONST('C','L','A','M'));
	// default program name
	strncpy (programName, "Default", kVstMaxProgNameLen);
}

//---------------------------------------------------------------------
VstNetworkExporter::~VstNetworkExporter ()
{
	_network.Stop();
}

//---------------------------------------------------------------------
int VstNetworkExporter::GetNumberOfParameters( const char* networkXmlContent )
{
	Network net;
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

	int count=0;
	for (Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
	{
		std::string controlSourceClass = "ControlSource";
		if (it->second->GetClassName() != controlSourceClass) continue;
		count++;
		std::cout << "Found ControlSource: " << it->first << std::endl;
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

	_network.Do();
}

////////////////////////////////////////////////////////////////////////////////////
void VstNetworkExporter::FillNetwork()
{
	CLAM::SimpleOscillator();
	CLAM::Oscillator();
	_network.SetName("VST net");
	_network.AddProcessing("AudioSink", "AudioSink");
	_network.AddProcessing("AudioSource", "AudioSource");
	_network.AddProcessing("Oscillator", "SimpleOscillator");
	_network.AddProcessing("Modulator", "Oscillator");
	_network.AddProcessing("ControlSource_0", "ControlSource");
	_network.ConnectPorts("AudioSource.AudioOut","Modulator.Input Phase Modulation");
	_network.ConnectPorts("Modulator.Audio Output","AudioSink.AudioIn");
	_network.ConnectPorts("Oscillator.Audio Output","Modulator.Input Frequency Modulation");
	_network.ConnectControls("ControlSource_0.output","Modulator.Pitch");
}

void VstNetworkExporter::LocateConnections()
{
	CLAM_ASSERT( mReceiverList.empty(), "VstNetworkExporter::LocateConnections() : there are already registered input ports");
	CLAM_ASSERT( mSenderList.empty(), "VstNetworkExporter::LocateConnections() : there are already registered output ports");
	CLAM_ASSERT( mInControlList.empty(), "VstNetworkExporter::LocateConnections() : there are already registered controls");

	for (Network::ProcessingsMap::const_iterator it=_network.BeginProcessings(); it!=_network.EndProcessings(); it++)
	{
		CLAM::Processing * processing = it->second;
		std::string className = processing->GetClassName();
		if (className == "AudioSource")
		{
			std::cout << "AudioSource: " << it->first << std::endl;
			DataInfo<AudioSource> info;
			info.name=it->first;
			info.processing=(AudioSource*)processing;
			info.processing->SetFrameAndHopSize( mExternBufferSize );
			mReceiverList.push_back(info);
		}
		if (className == "AudioSink")
		{
			std::cout << "AudioSink: " << it->first << std::endl;
			DataInfo<AudioSink> info;
			info.name=it->first;
			info.processing=(AudioSink*)processing;
			info.processing->SetFrameAndHopSize( mExternBufferSize );
			mSenderList.push_back(info);
		}

		if (className == "ControlSource")
		{
			std::cout << "ControlSource: " << it->first << std::endl;
			const ControlSourceConfig & conf= dynamic_cast<const ControlSourceConfig&>(it->second->GetConfig() );

			ExternControlInfo info;
			info.name=it->first;
			info.processing=(ControlSource*)processing;
			info.min=conf.GetMinValue();
			info.max=conf.GetMaxValue();
			info.lastvalue=info.max;
			mInControlList.push_back(info);
		}
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


