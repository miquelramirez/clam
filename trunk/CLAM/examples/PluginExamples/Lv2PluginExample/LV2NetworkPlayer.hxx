#ifndef LV2NetworkPlayer_hxx
#define LV2NetworkPlayer_hxx
#include <vector>
#include <lv2.h>
#include <math.h>
#include <iostream>

#include <CLAM/Network.hxx>
#include <CLAM/AudioSource.hxx>
#include <CLAM/AudioSink.hxx>
#include <CLAM/ControlSource.hxx>
#include <CLAM/ControlSink.hxx>
#include <CLAM/PushFlowControl.hxx>
#include <CLAM/XMLStorage.hxx>
#include <CLAM/NetworkPlayer.hxx>

namespace CLAM
{

class LV2NetworkPlayer : public NetworkPlayer
{
	template<class T>
		class LV2Info
		{
		public:
			std::string name;
			T* processing;
			float *dataBuffer;
			unsigned port;
		};

private:
	typedef std::vector<float * > Buffers;
	Buffers _sourceBuffers;
	Buffers _sinkBuffers;
	Buffers _inControlBuffers;
	Buffers _outControlBuffers;

	typedef std::vector<CLAM::ControlSource*> InControlList;
	typedef std::vector<CLAM::ControlSink*> OutControlList;
private:
	typedef std::vector< LV2Info<CLAM::ControlSource> > LV2InControlList;
	typedef std::vector< LV2Info<CLAM::ControlSink> > LV2OutControlList;

	CLAM::Network _network;
	LV2InControlList mInControlList;
	LV2OutControlList mOutControlList;

	unsigned long mClamBufferSize;
	unsigned long mExternBufferSize;
		
public:
	LV2NetworkPlayer(const LV2_Descriptor * descriptor);
	~LV2NetworkPlayer();

	void ActivateExporter();
	void DeactivateExporter();
	void CleanupExporter();
	void InstantiateExporter();
	void ConnectPortExporter(uint32_t port, void *data);
	void RunExporter(uint32_t nframes);
	static LV2_Descriptor * CreateLV2Descriptor(
		const std::string & networkXmlContent,
		const std::string & uri
	);

	void ProcessOutControlValues();
	void SetAudioSinkBuffers(const unsigned long nframes);
	void SetAudioSourceBuffers(const unsigned long nframes);
	void UpdatePortFrameAndHopSize();	
	void ProcessInControlValues();
	void LocateConnections();
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

#endif //LV2NetworkPlayer_hxx
