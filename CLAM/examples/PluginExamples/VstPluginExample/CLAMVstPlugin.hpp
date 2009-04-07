#ifndef __CLAMTest_H
#define __CLAMTest_H

#include "audioeffectx.h"

#include <CLAM/Network.hxx>
#include <CLAM/ControlSource.hxx>
#include <CLAM/AudioSource.hxx>
#include <CLAM/AudioSink.hxx>

namespace CLAM
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

//-------------------------------------------------------------------------------------------------------
class VstNetworkExporter : public AudioEffectX
{
public:
	VstNetworkExporter (
		audioMasterCallback audioMaster,
		const std::string & effectName,
		const std::string & productString,
		const std::string & vendor,
		int version);
	~VstNetworkExporter ();

	bool ok() const { return true; }

	// Processes
	virtual void process (float **inputs, float **outputs, long sampleFrames);
	virtual void processReplacing (float **inputs, float **outputs, VstInt32 sampleFrames) {} // TODO

	// Program
	virtual void setProgramName (char *name);
	virtual void getProgramName (char *name);

	// Parameters
	virtual void setParameter (long index, float value);
	virtual float getParameter (long index);
	virtual void getParameterLabel (long index, char *label);
	virtual void getParameterDisplay (long index, char *text);
	virtual void getParameterName (long index, char *text);

	// Metadata
	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion ();
	virtual VstPlugCategory getPlugCategory () { return kPlugCategEffect; }

protected:
	char programName[kVstMaxProgNameLen];
	CLAM::Network _network;
	CLAM::VSTInControlList mInControlList;
	CLAM::VSTInPortList mReceiverList;
	CLAM::VSTOutPortList mSenderList;
	int mClamBufferSize, mExternBufferSize;

private:
	CLAM::Network& GetNetwork() { return _network; }
	void FillNetwork();
	int GetNumberOfParameters(const char* );
	void LocateConnections();
	void ProcessInputControls();
	void UpdatePortFrameAndHopSize();

	std::string _effectName;
	std::string _productString;
	std::string _vendor;
	int _version;
};

}

#endif
