#ifndef DataAudioStreamer_hxx
#define DataAudioStreamer_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/AudioInPort.hxx>
#include "SampleBySampleConfig.hxx"

namespace CLAM
{

class AudioStreamer:public CLAM::Processing
{
public:
	AudioStreamer();	
	bool Do();
	const char* GetClassName() const {return "AudioStreamer";}
	bool ConcreteConfigure(const CLAM::ProcessingConfig &);
	inline const CLAM::ProcessingConfig &GetConfig() const { return mConfig;}
protected:	
	SampleBySampleConfig mConfig;
	CLAM::AudioInPort mInput;
	OutDataPort mOutput;

	int mCounter;
	CLAM::DataArray mBuffer;
};

};
#endif

