#include "DataStreamConverter.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace detail
{
	static const char * metadata[] = {
		"key", "Data2Audio",
		"category", "Sample by Sample (experimental)",
		"description", "Data2Audio",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, Data2Audio> regData2Audio("Data2Audio");
	static FactoryRegistrator<ProcessingFactory, Data2Audio> reg = metadata;
}

Data2Audio::Data2Audio():
	mInput("Input", this),
	mOutput("Output", this)
{
	Configure(SampleBySampleConfig());
}

bool Data2Audio::ConcreteConfigure(const CLAM::ProcessingConfig &c)
{
	mCounter=0;
	
	CopyAsConcreteConfig(mConfig,c);
	int frameSize = mConfig.GetFrameSize();
	mOutput.SetSize(frameSize);
	mOutput.SetHop(frameSize);
	mBuffer.Resize(frameSize);
	mBuffer.SetSize(frameSize);
	return true;
}

bool Data2Audio::Do()
{
	mBuffer[mCounter]=mInput.GetData();
	mInput.Consume();
	mCounter++;
	int frameSize = mConfig.GetFrameSize();
	if(mCounter==frameSize)
	{
		mCounter = 0;
		mOutput.GetAudio().GetBuffer()=mBuffer;
		mOutput.Produce();
		mBuffer.Resize(0);
		mBuffer.Resize(frameSize);
		mBuffer.SetSize(frameSize);
	}
	
	return true;
}



};

