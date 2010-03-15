
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

VstNetworkPlayer::VstNetworkPlayer (
		audioMasterCallback audioMaster,
		const std::string & networkXml,
		VstInt32 uniqueId,
		const std::string & effectName,
		const std::string & productString,
		const std::string & vendor,
		int version
	)
	: AudioEffectX (audioMaster, 0 /*nPrograms*/, 0 /*nParams, set later*/ )
	, _uniqueId(uniqueId)
	, _embeddedNetwork(networkXml)
	, _effectName(effectName)
	, _productString(productString)
	, _vendor(vendor)
	, _version(version)
{
	mExternBufferSize=512;
	SetNetworkBackLink(_network);
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
	CacheSourcesAndSinks();
	for (unsigned i=0; i<GetNControlSources(); i++)
	{
		const ControlSourceConfig & conf = 
			dynamic_cast<const ControlSourceConfig&>(
				_controlSources[i].processing->GetConfig() );
			ExternControlInfo info;
			info.min=conf.GetMinValue();
			info.max=conf.GetMaxValue();
			info.lastvalue=conf.GetDefaultValue();
			info.units=conf.GetUnitName();
			mInControlList.push_back(info);
	}
	ChangeFrameSize(mExternBufferSize);

	AudioEffectX::setUniqueID (_uniqueId);
	AudioEffectX::setNumInputs (GetNSources());
	AudioEffectX::setNumOutputs (GetNSinks());
	// breaks intended vst encapsulation but sending numParams
	// to the AudioEffectX constructor would imply loading
	// another network when no derived class vars can be set.
	AudioEffectX::getAeffect()->numParams = GetNControlSources();
	setInitialDelay(0); // TODO: Set the real one
}

VstNetworkPlayer::~VstNetworkPlayer ()
{
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


// VST interface: Parameter management
bool VstNetworkPlayer::getParameterProperties (VstInt32 index, VstParameterProperties* p)
{
	std::cout << "== getParameterProperties " << index << std::endl;
	bool result = AudioEffectX::getParameterProperties(index, p);
	std::cout << p->flags << std::endl;
	const char * units = mInControlList[index].units.c_str();
	strncpy(p->label, units, kVstMaxLabelLen);
	strncpy(p->shortLabel, units, kVstMaxShortLabelLen);
	p->flags = 0;
	if (false) // TODO: Detect bool parameters
	{
		p->flags |= kVstParameterIsSwitch;
		p->flags |= kVstParameterUsesIntStep;
		p->stepInteger=1;
		p->largeStepInteger=1;
	}
	if (false) // TODO: Detect float parameters
	{
		p->flags |= kVstParameterUsesFloatStep;
		p->stepFloat; // Normal float step
		p->smallStepFloat; // small float step
		p->largeStepFloat;         // large float step
	}
	if (false) // TODO: Detect integer parameters
	{
		p->flags |= kVstParameterUsesIntegerMinMax;
		p->minInteger=0;
		p->maxInteger=1;
	}
	if (false) // TODO: When
	{
		p->flags |= kVstParameterUsesIntStep;
		p->stepInteger;
		p->largeStepInteger;
	}
	if (false)
	{
		p->flags |= kVstParameterSupportsDisplayIndex;
		p->displayIndex = index;
	}
	if (false)
	{
		p->flags |= kVstParameterSupportsDisplayCategory;
		p->category; // 0 no category or categori index +1
//		strncpy(p->categoryLabel,"MyCategory",kVstMaxCategLabelLen);
		p->numParametersInCategory;
	}
	if (false)
		p->flags |= kVstParameterCanRamp; // parameter value can ramp up/down
 	;
	return true;
}

void VstNetworkPlayer::setParameter (VstInt32 index, float value)
{
	ExternControlInfo & controlInfo = mInControlList[index];
	float realval = controlInfo.min + value * ( controlInfo.max - controlInfo.min ) ;
	
	controlInfo.lastvalue = realval;
	ReadControlSource(index, &realval);
	std::cout << "== setParameter " << ControlSourceName(index) << ": " << value << " -> " << realval << std::endl;
}

float VstNetworkPlayer::getParameter (VstInt32 index)
{
	ExternControlInfo & control = mInControlList[index];
	float interfaceValue = (control.lastvalue-control.min)/(control.max-control.min);
	std::cout << "== getParameter " << ControlSourceName(index) << ": " << control.lastvalue << " -> " << interfaceValue << std::endl;
	return interfaceValue;
}

void VstNetworkPlayer::getParameterName (VstInt32 index, char *label)
{
	strncpy ( label, ControlSourceName(index).c_str(), kVstMaxParamStrLen ); 
	std::cout << "== getParameterName " << label << std::endl;
}

void VstNetworkPlayer::getParameterDisplay (VstInt32 index, char *text)
{
	float2string ( mInControlList[index].lastvalue, text, kVstMaxParamStrLen);
	std::cout << "== getParameterDisplay " << text << std::endl;
}

void VstNetworkPlayer::getParameterLabel(VstInt32 index, char *label)
{
	strncpy (label, mInControlList[index].units.c_str(), kVstMaxParamStrLen);
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
		ChangeFrameSize(mExternBufferSize);
	}

	for (unsigned i=0; i<GetNSources(); i++)
		SetSourceBuffer(i, inputs[i], mExternBufferSize);
	for (unsigned i=0; i<GetNSinks(); i++)
		SetSinkBuffer(i, outputs[i], mExternBufferSize);

	_network.Do();
}

void VstNetworkPlayer::ChangeFrameSize(unsigned nframes)
{
	for (unsigned i=0; i<GetNSources(); i++)
		SetSourceFrameSize(i, nframes);
	for (unsigned i=0; i<GetNSinks(); i++)
		SetSinkFrameSize(i, nframes);
}

} // CLAM


