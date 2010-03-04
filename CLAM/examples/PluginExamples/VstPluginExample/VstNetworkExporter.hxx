#ifndef VstNetworkExporter_hxx
#define VstNetworkExporter_hxx

#include "audioeffectx.h"

#include <CLAM/Network.hxx>
#include <CLAM/ControlSource.hxx>
#include <CLAM/AudioSource.hxx>
#include <CLAM/AudioSink.hxx>

namespace CLAM
{
//-------------------------------------------------------------------------------------------------------
class VstNetworkExporter : public AudioEffectX
{
	template<class T>
	class ConnectorInfo
	{
	public:
		std::string name;
		T* processing;
	};

	class ExternControlInfo
	{
	public:
		std::string name;
		ControlSource* processing;
		float lastvalue, min, max;
	};

	typedef std::vector< ExternControlInfo > VSTInControlList;
	typedef std::vector< ConnectorInfo< CLAM::AudioSource > > VSTInPortList;
	typedef std::vector< ConnectorInfo< CLAM::AudioSink > > VSTOutPortList;

public:
	typedef VstNetworkExporter Plugin;
	VstNetworkExporter (
		const std::string & networkContent,
		VstInt32 uniqueId,
		const std::string & effectName,
		const std::string & productString,
		const std::string & vendor,
		VstInt32 version);
	VstNetworkExporter (
		audioMasterCallback audioMaster,
		const std::string & networkContent,
		VstInt32 uniqueId,
		const std::string & effectName,
		const std::string & productString,
		const std::string & vendor,
		VstInt32 version);
	~VstNetworkExporter ();

	Plugin * createEffect(audioMasterCallback audioMaster);
	bool ok() const { return true; }

	// * VST AudioEffectX API *

	// Processes
	virtual void processReplacing (float **inputs, float **outputs, VstInt32 sampleFrames);

	// Program
	virtual void setProgramName (char *name);
	virtual void getProgramName (char *name);

	// Parameters
	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterLabel (VstInt32 index, char *label);
	virtual void getParameterDisplay (VstInt32 index, char *text);
	virtual void getParameterName (VstInt32 index, char *text);

	// Metadata
	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion ();
	virtual VstPlugCategory getPlugCategory () { return kPlugCategEffect; }

protected:
	std::string _programName;
	CLAM::Network _network;
	VSTInControlList mInControlList;
	VSTInPortList mReceiverList;
	VSTOutPortList mSenderList;
	unsigned mClamBufferSize, mExternBufferSize;

private:
	CLAM::Network& GetNetwork() { return _network; }
	int GetNumberOfParameters(const std::string & networkXmlContent);
	void LocateConnections();
	void ProcessInputControls();
	void UpdatePortFrameAndHopSize();

	VstInt32 _uniqueId;
	std::string _embeddedNetwork;
	std::string _effectName;
	std::string _productString;
	std::string _vendor;
	VstInt32 _version;
};

}

#endif
