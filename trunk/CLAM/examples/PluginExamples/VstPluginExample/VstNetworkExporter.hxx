#ifndef VstNetworkExporter_hxx
#define VstNetworkExporter_hxx

#include "audioeffectx.h"

#include <CLAM/Network.hxx>
#include <CLAM/ControlSource.hxx>
#include <CLAM/AudioSource.hxx>
#include <CLAM/AudioSink.hxx>

namespace CLAM
{

class VstNetworkPlayer;

class VstNetworkExporter
{
public:
	typedef VstNetworkPlayer Plugin;
	VstNetworkExporter (
		const std::string & networkContent,
		VstInt32 uniqueId,
		const std::string & effectName,
		const std::string & productString,
		const std::string & vendor,
		VstInt32 version);
	Plugin * createEffect(audioMasterCallback audioMaster);
private:
	VstInt32 _uniqueId;
	std::string _embeddedNetwork;
	std::string _effectName;
	std::string _productString;
	std::string _vendor;
	VstInt32 _version;
};


class VstNetworkPlayer : public NetworkPlayer, public AudioEffectX
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

public:
	typedef VstNetworkPlayer Plugin;
	VstNetworkPlayer (
		audioMasterCallback audioMaster,
		const std::string & networkContent,
		VstInt32 uniqueId,
		const std::string & effectName,
		const std::string & productString,
		const std::string & vendor,
		VstInt32 version);
	~VstNetworkPlayer ();

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
	unsigned mExternBufferSize;

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
public: // NetworkPlayer interface
	virtual bool IsWorking() { return true; }
	virtual std::string NonWorkingReason() { return ""; }
	virtual void Start() {}
	virtual void Stop() {}
//	virtual void Pause() {}
	virtual bool IsRealTime() const { return true; }
//	virtual unsigned BackendBufferSize();
//	virtual unsigned BackendSampleRate();
};

}

#endif
