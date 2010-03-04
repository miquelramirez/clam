
#include "VstNetworkExporter.hxx"
#include <CLAM/XMLStorage.hxx>
#include <iostream>
#include <sstream>

namespace CLAM
{

VstNetworkExporter::Plugin * VstNetworkExporter::createEffect(audioMasterCallback audioMaster)
{
	return new VstNetworkExporter::Plugin(
		audioMaster,
		_embeddedNetwork,
		_uniqueId,
		_effectName,
		_productString,
		_vendor,
		_version);
}

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
	_programName = name;
}

void VstNetworkExporter::getProgramName (char *name)
{
	strncpy (name, _programName.c_str(), kVstMaxProgNameLen);
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
	ExternControlInfo & control = mInControlList[index];
	float interfaceValue = (control.lastvalue-control.min)/(control.max-control.min);
	std::cout << "== getParameter " << control.lastvalue << " -> " << interfaceValue << std::endl;
	return interfaceValue;
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
		const std::string & networkXml,
		VstInt32 uniqueId,
		const std::string & effectName,
		const std::string & productString,
		const std::string & vendor,
		VstInt32 version
	)
	: AudioEffectX (0, 1 /*nPrograms*/, GetNumberOfParameters(networkXml) )
	, _uniqueId(uniqueId)
	, _embeddedNetwork(networkXml)
	, _effectName(effectName)
	, _productString(productString)
	, _vendor(vendor)
	, _version(version)
{
	
}

VstNetworkExporter::VstNetworkExporter (
		audioMasterCallback audioMaster,
		const std::string & networkXml,
		VstInt32 uniqueId,
		const std::string & effectName,
		const std::string & productString,
		const std::string & vendor,
		int version
	)
	: AudioEffectX (audioMaster, 1 /*nPrograms*/, GetNumberOfParameters(networkXml) )
	, _uniqueId(uniqueId)
	, _embeddedNetwork(networkXml)
	, _effectName(effectName)
	, _productString(productString)
	, _vendor(vendor)
	, _version(version)
{
	mClamBufferSize=512;
	mExternBufferSize=mClamBufferSize;
	std::istringstream xmlfile(networkXml);
	try
	{
		XmlStorage::Restore( _network, xmlfile );
	}
	catch ( XmlStorageErr err)
	{
		std::cerr << "CLAM VST: Error while loading CLAM network based plugin '" << _effectName << "'." <<std::endl;
		std::cerr << err.what() << std::endl;
		std::cerr << "Plugin not loaded." << std::endl;
		return;
	}
	if (_network.HasMisconfiguredProcessings())
	{
		std::cerr << "CLAM VST: Error while configuring CLAM network based plugin '" << _effectName << "'." <<std::endl;
		std::cerr << _network.GetConfigurationErrors() << std::endl;
		std::cerr << "Plugin not loaded." << std::endl;
		return;
	}
	if (_network.HasUnconnectedInPorts())
	{
		std::cerr << "CLAM VST: Error loading CLAM network based plugin '" << _effectName << "'." <<std::endl;
		std::cerr << "Plugin not loaded because internal network inports were unconnected." <<std::endl;
		std::cerr << _network.GetUnconnectedInPorts() << std::flush;
		return;
	}
	LocateConnections();

	setNumInputs (mReceiverList.size());
	setNumOutputs (mSenderList.size());
	setUniqueID (_uniqueId);
	_programName = "Default";
	_network.Start();
}

VstNetworkExporter::~VstNetworkExporter ()
{
	_network.Stop();
}

//---------------------------------------------------------------------
int VstNetworkExporter::GetNumberOfParameters( const std::string & networkXmlContent )
{
	Network net;
	std::istringstream file(networkXmlContent);
	try
	{
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

void VstNetworkExporter::processReplacing (float **inputs, float **outputs, VstInt32 sampleFrames)
{
	std::cout << "." << std::flush;
	if (unsigned(sampleFrames)!=mExternBufferSize)
	{
		std::cerr << "Switching to "<< sampleFrames <<std::endl;
		mExternBufferSize=sampleFrames;
		UpdatePortFrameAndHopSize();
	}

	for (unsigned i=0; i<mReceiverList.size(); i++)
		mReceiverList[i].processing->SetExternalBuffer( inputs[i], mExternBufferSize, 0 );

	for (unsigned i=0; i<mSenderList.size(); i++)
		mSenderList[i].processing->SetExternalBuffer( outputs[i], mExternBufferSize, 0 );

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
			ConnectorInfo<AudioSource> info;
			info.name=it->first;
			info.processing=(AudioSource*)processing;
			info.processing->SetFrameAndHopSize( mExternBufferSize,0 );
			mReceiverList.push_back(info);
		}
		if (className == "AudioSink")
		{
			std::cout << "AudioSink: " << it->first << std::endl;
			ConnectorInfo<AudioSink> info;
			info.name=it->first;
			info.processing=(AudioSink*)processing;
			info.processing->SetFrameAndHopSize( mExternBufferSize,0 );
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
		mReceiverList[i].processing->SetFrameAndHopSize(mExternBufferSize,0);
		
	for (unsigned i=0; i<mSenderList.size(); i++)
		mSenderList[i].processing->SetFrameAndHopSize(mExternBufferSize,0);
}

} // CLAM


