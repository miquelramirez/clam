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
	class DataInfo
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
	typedef std::vector< DataInfo< CLAM::AudioSource > > VSTInPortList;
	typedef std::vector< DataInfo< CLAM::AudioSink > > VSTOutPortList;

public:
	typedef VstNetworkExporter Plugin;
	VstNetworkExporter (
		const std::string & networkContent,
		const std::string & effectName,
		const std::string & productString,
		const std::string & vendor,
		int version);
	VstNetworkExporter (
		audioMasterCallback audioMaster,
		const std::string & networkContent,
		const std::string & effectName,
		const std::string & productString,
		const std::string & vendor,
		int version);
	~VstNetworkExporter ();

	Plugin * createEffect(audioMasterCallback audioMaster);
	bool ok() const { return true; }

	// Processes
	virtual void processReplacing (float **inputs, float **outputs, VstInt32 sampleFrames);
	virtual void process (float **inputs, float **outputs, VstInt32 sampleFrames) {} // TODO

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
	int mClamBufferSize, mExternBufferSize;

private:
	CLAM::Network& GetNetwork() { return _network; }
	int GetNumberOfParameters(const std::string & networkXmlContent);
	void LocateConnections();
	void ProcessInputControls();
	void UpdatePortFrameAndHopSize();

	std::string _embeddedNetwork;
	std::string _effectName;
	std::string _productString;
	std::string _vendor;
	int _version;
};

}

#endif
