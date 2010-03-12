
#include "VstNetworkExporter.hxx"
#include <CLAM/XMLStorage.hxx>
#include <iostream>
#include <sstream>

namespace CLAM
{

VstNetworkExporter::VstNetworkExporter (
		const std::string & networkXml,
		VstInt32 uniqueId,
		const std::string & effectName,
		const std::string & productString,
		const std::string & vendor,
		VstInt32 version
	)
	: _uniqueId(uniqueId)
	, _embeddedNetwork(networkXml)
	, _effectName(effectName)
	, _productString(productString)
	, _vendor(vendor)
	, _version(version)
{
}

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

bool VstNetworkPlayer::getEffectName (char* name)
{
	strncpy (name, _effectName.c_str(), kVstMaxEffectNameLen);
	return true;
}

bool VstNetworkPlayer::getProductString (char* text)
{
	strncpy (text, _productString.c_str(), kVstMaxProductStrLen);
	return true;
}

bool VstNetworkPlayer::getVendorString (char* text)
{
	strncpy (text, _vendor.c_str(), kVstMaxVendorStrLen);
	return true;
}

VstInt32 VstNetworkPlayer::getVendorVersion ()
{
	return _version;
}

// VST interface: Program management

void VstNetworkPlayer::setProgramName (char *name)
{
	_programName = name;
}

void VstNetworkPlayer::getProgramName (char *name)
{
	strncpy (name, _programName.c_str(), kVstMaxProgNameLen);
}


// VST interface: Parameter management

void VstNetworkPlayer::setParameter (VstInt32 index, float value)
{
	ExternControlInfo & controlInfo = mInControlList[index];
	float realval = controlInfo.min + value * ( controlInfo.max - controlInfo.min ) ;
	
	controlInfo.lastvalue = realval;
	ReadControlSource(index, &realval);
	std::cout << "== setParameter " << GetControlSourceName(index) << ": " << value << " -> " << realval << std::endl;
}

float VstNetworkPlayer::getParameter (VstInt32 index)
{
	ExternControlInfo & control = mInControlList[index];
	float interfaceValue = (control.lastvalue-control.min)/(control.max-control.min);
	std::cout << "== getParameter " << GetControlSourceName(index) << ": " << control.lastvalue << " -> " << interfaceValue << std::endl;
	return interfaceValue;
}

void VstNetworkPlayer::getParameterName (VstInt32 index, char *label)
{
	strncpy ( label, GetControlSourceName().c_str(), kVstMaxParamStrLen ); 
	std::cout << "== getParameterName " << label << std::endl;
}

void VstNetworkPlayer::getParameterDisplay (VstInt32 index, char *text)
{
	float2string ( mInControlList[index].lastvalue, text, kVstMaxParamStrLen);
	std::cout << "== getParameterDisplay " << text << std::endl;
}

void VstNetworkPlayer::getParameterLabel(VstInt32 index, char *label)
{
	ControlSourceConfig& conf=const_cast<ControlSourceConfig&>(
		dynamic_cast<const ControlSourceConfig&>(
			mInControlList[index].processing->GetConfig() ));
	strncpy (label, conf.GetUnitName().c_str(), kVstMaxParamStrLen);
}

VstNetworkPlayer::VstNetworkPlayer (
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
	mExternBufferSize=512;
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

	setNumInputs (GetNSources());
	setNumOutputs (GetNSinks());
	setUniqueID (_uniqueId);
	_programName = "Default";
	_network.Start();
}

VstNetworkPlayer::~VstNetworkPlayer ()
{
	_network.Stop();
}


int VstNetworkPlayer::GetNumberOfParameters( const std::string & networkXmlContent )
{
	Network net;
	std::istringstream file(networkXmlContent);
	try
	{
		XmlStorage::Restore( net, file );
	}
	catch ( XmlStorageErr err)
	{
		std::cerr << "VstNetworkPlayer WARNING: error loading embedded network. "
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

void VstNetworkPlayer::processReplacing (float **inputs, float **outputs, VstInt32 sampleFrames)
{
	std::cout << "." << std::flush;
	if (unsigned(sampleFrames)!=mExternBufferSize)
	{
		std::cerr << "Switching to "<< sampleFrames <<std::endl;
		mExternBufferSize=sampleFrames;
		UpdatePortFrameAndHopSize();
	}

	for (unsigned i=0; i<GetNSources(); i++)
		SetSourceBuffer(i, inputs[i], mExternBufferSize);
	for (unsigned i=0; i<GetNSinks(); i++)
		SetSinkBuffer(i, outputs[i], mExternBufferSize);

	_network.Do();
}

void VstNetworkPlayer::LocateConnections()
{
	CacheSourcesAndSinks();
	for (unsigned i=0; i<GetNControlSources(); i++)
	{
		const ControlSourceConfig & conf = 
			dynamic_cast<const ControlSourceConfig&>(
				_controlSources[i]->GetConfig() );
			ExternControlInfo info;
			info.min=conf.GetMinValue();
			info.max=conf.GetMaxValue();
			info.lastvalue=conf.GetDefaultValue();
			mInControlList.push_back(info);
	}
	UpdatePortFrameAndHopSize();
}

void VstNetworkPlayer::UpdatePortFrameAndHopSize()
{
	for (unsigned i=0; i<GetNSources(); i++)
		SetSourceFrameSize(i, mExternBufferSize);
	for (unsigned i=0; i<GetNSinks(); i++)
		SetSinkFrameSize(i, mExternBufferSize);
}

} // CLAM


