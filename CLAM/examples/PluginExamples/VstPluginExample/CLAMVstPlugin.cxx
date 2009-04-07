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

CLAM_EMBEDDED_FILE(embededNetwork,"wire.xml")

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	std::cout << "== createEffectInstance" << std::endl;
	static CLAM::VstNetworkExporter exporter(
		embededNetwork,
		"CLAM effect",
		"Example of CLAM based VST plugin",
		"CLAM Project",
		1000
	);
	return exporter.createEffect(audioMaster);
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

void VstNetworkExporter::setParameter (VstInt32 index, float value)
{
	ExternControlInfo & controlInfo = mInControlList[index];
	float realval = controlInfo.min + value * ( controlInfo.max - controlInfo.min ) ;

	controlInfo.lastvalue = realval;
	controlInfo.processing->Do(realval);
	std::cout << "== setParameter " << value << " -> " << realval << std::endl;
}

float VstNetworkExporter::getParameter (VstInt32 index)
{
	std::cout << "== getParameter " << mInControlList[index].lastvalue << std::endl;
	return mInControlList[index].lastvalue;
}

void VstNetworkExporter::getParameterName (VstInt32 index, char *label)
{
	strncpy ( label, mInControlList[index].name.c_str(), kVstMaxParamStrLen ); 
	std::cout << "== getParameterName " << label << std::endl;
}

void VstNetworkExporter::getParameterDisplay (VstInt32 index, char *text)
{
	float2string ( mInControlList[index].lastvalue, text, kVstMaxParamStrLen);
	std::cout << "== getParameterDisplay " << text << std::endl;
}

void VstNetworkExporter::getParameterLabel(VstInt32 index, char *label)
{
	ControlSourceConfig& conf=const_cast<ControlSourceConfig&>(
		dynamic_cast<const ControlSourceConfig&>(
			mInControlList[index].processing->GetConfig() ));
	strncpy (label, conf.GetUnitName().c_str(), kVstMaxParamStrLen);
}

//---------------------------------------------------------------------
VstNetworkExporter::VstNetworkExporter (
		const std::string & networkContent,
		const std::string & effectName,
		const std::string & productString,
		const std::string & vendor,
		int version
	)
	: AudioEffectX (0, 1 /*nPrograms*/, GetNumberOfParameters(networkContent) )
	, _effectName(effectName)
	, _productString(productString)
	, _vendor(vendor)
	, _version(version)
{
	
}

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
		// TODO: error
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
int VstNetworkExporter::GetNumberOfParameters( const std::string & networkXmlContent )
{
	Network net;
	try
	{
		std::istringstream file(networkXmlContent);
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

void VstNetworkExporter::process (float **inputs, float **outputs, VstInt32 sampleFrames)
{
	std::cout << "." << std::flush;
	if (sampleFrames!=mExternBufferSize)
	{
		std::cerr << "Switching to "<< sampleFrames <<std::endl;
		mExternBufferSize=sampleFrames;
		UpdatePortFrameAndHopSize();
	}

	for (unsigned i=0; i<mReceiverList.size(); i++)
		mReceiverList[i].processing->SetExternalBuffer( inputs[i], sampleFrames );

	for (unsigned i=0; i<mSenderList.size(); i++)
		mSenderList[i].processing->SetExternalBuffer( outputs[i], sampleFrames );

	_network.Do();
}

////////////////////////////////////////////////////////////////////////////////////
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
	for (unsigned i=0; i<mReceiverList.size(); i++)
		mReceiverList[i].processing->SetFrameAndHopSize(mExternBufferSize);
		
	for (unsigned i=0; i<mSenderList.size(); i++)
		mSenderList[i].processing->SetFrameAndHopSize(mExternBufferSize);
}

} // CLAM


