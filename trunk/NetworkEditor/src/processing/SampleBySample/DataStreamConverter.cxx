#include "DataStreamConverter.hxx"
#include "Factory.hxx"


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
namespace CLAM
{

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

namespace detail
{
	static ProcessingFactory::Registrator<Data2Audio> regtData2Audio( "Data2Audio" );
}


};