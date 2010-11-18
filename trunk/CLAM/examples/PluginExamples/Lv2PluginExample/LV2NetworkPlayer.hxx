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

private:
	CLAM::Network _network;
	unsigned long mExternBufferSize;
public:
	static LV2_Descriptor * CreateLV2Descriptor(
		const std::string & networkXmlContent,
		const std::string & uri
	);

	LV2NetworkPlayer(
		const LV2_Descriptor * descriptor,
		double sampleRate,
		const char *bundlePath,
		const LV2_Feature * const* features);
	~LV2NetworkPlayer();

	void lv2_Activate();
	void lv2_Deactivate();
	void lv2_ConnectTo(uint32_t port, void *data);
	void lv2_Run(uint32_t nframes);
	void LocateConnections();
private:
	void ProcessInControlValues();
	void ProcessOutControlValues();
	void SetAudioSinkBuffers(unsigned long nframes);
	void SetAudioSourceBuffers(unsigned long nframes);
	void ChangeFrameSize(unsigned nframes);
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
