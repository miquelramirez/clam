#include "DataAudioStreamer.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace detail
{
	static FactoryRegistrator<ProcessingFactory, AudioStreamer> regAudioStreamer("AudioStreamer");
}

AudioStreamer::AudioStreamer():
	mInput("Input", this),
	mOutput("Output", this)
{
	Configure(SampleBySampleConfig());
}

bool AudioStreamer::ConcreteConfigure(const CLAM::ProcessingConfig &c)
{
	CopyAsConcreteConfig(mConfig,c);
	int frameSize = mConfig.GetFrameSize();
	mInput.SetSize(frameSize);
	mInput.SetHop(frameSize);
	mBuffer.Resize(frameSize);
	mBuffer.SetSize(frameSize);
	mCounter=0;
	return true;
}

bool AudioStreamer::Do()
{
	if(mCounter==mBuffer.Size())
	{
		mCounter = 0;
	}
	
	if(mCounter==0)
	{
		mBuffer = mInput.GetAudio().GetBuffer();
		mInput.Consume();
	}

	mOutput.GetData()= mBuffer[mCounter];
	mOutput.Produce();
	mCounter++;
	return true;
}


};

