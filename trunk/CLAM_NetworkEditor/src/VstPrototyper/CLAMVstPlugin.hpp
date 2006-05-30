#ifndef __CLAMTest_H
#define __CLAMTest_H

#include "audioeffectx.h"

#include <CLAM/Network.hxx>
#include <CLAM/ExternInControl.hxx>
#include <CLAM/ExternGenerator.hxx>
#include <CLAM/ExternSink.hxx>

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
		ExternInControl* processing;
		float lastvalue, min, max;
	};

	typedef std::vector< ExternControlInfo > VSTInControlList;
	typedef std::vector< DataInfo< CLAM::ExternGenerator > > VSTInPortList;
	typedef std::vector< DataInfo< CLAM::ExternSink > > VSTOutPortList;

}

//-------------------------------------------------------------------------------------------------------
class CLAMTest : public AudioEffectX
{
public:
	CLAMTest (audioMasterCallback audioMaster);
	~CLAMTest ();

	// Processes
	virtual void process (float **inputs, float **outputs, long sampleFrames);

	// Program
	virtual void setProgramName (char *name);
	virtual void getProgramName (char *name);

	// Parameters
	virtual void setParameter (long index, float value);
	virtual float getParameter (long index);
	virtual void getParameterLabel (long index, char *label);
	virtual void getParameterDisplay (long index, char *text);
	virtual void getParameterName (long index, char *text);

	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual long getVendorVersion () { return 1000; }
	
	virtual VstPlugCategory getPlugCategory () { return kPlugCategEffect; }

protected:
	char programName[32];
	CLAM::Network* mNet;
	CLAM::VSTInControlList mInControlList;
	CLAM::VSTInPortList mReceiverList;
	CLAM::VSTOutPortList mSenderList;
	int mClamBufferSize, mExternBufferSize;

private:
	CLAM::Network& GetNetwork() { return *mNet; }
	void FillNetwork();
	int GetNumberOfParameters( char* );
	void ProcessInputControls();
	void ProcessInputPorts();
	void ProcessOutputPorts();
	void UpdatePortFrameAndHopSize();
};

#endif
