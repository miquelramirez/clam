
#include "AudioOutWrapper.hxx"
#include "AudioIO.hxx"

namespace CLAM
{

void AudioOutWrapperConfiguration::DefaultInit()
{
	AddAll();
	UpdateData();
	
	SetDevice("default:default");
	SetSampleRate(0);
	SetFrameSize(512);
}

AudioOutWrapper::AudioOutWrapper()
	: mInput( "Audio Input", this, 1 )
{
	AdoptChildren();
	Configure(AudioOutWrapperConfiguration());
}

AudioOutWrapper::AudioOutWrapper( AudioOutWrapperConfiguration & cfg)
	: mInput( "Audio Inout", this, 1 )
{
	AdoptChildren();
	Configure( cfg );
}

void AudioOutWrapper::AdoptChildren()
{
	mLeft.SetParent( this );
	mRight.SetParent( this );
}

bool AudioOutWrapper::ConfigureChildren(int size)
{
	AudioIOConfig leftCfg;
	AudioIOConfig rightCfg;
	
	leftCfg.SetName("left out");
	leftCfg.SetChannelID(0);
	leftCfg.SetDevice( mConfig.GetDevice() );
	leftCfg.SetSampleRate( mConfig.GetSampleRate() );
	leftCfg.SetFrameSize( mConfig.GetFrameSize() );

	rightCfg.SetName("right out");
	rightCfg.SetChannelID(1);
	rightCfg.SetDevice( mConfig.GetDevice() );
	rightCfg.SetSampleRate( mConfig.GetSampleRate() );
	rightCfg.SetFrameSize( mConfig.GetFrameSize() );

	if( !mLeft.Configure( leftCfg ) )
		return false;
	if( !mRight.Configure( rightCfg ) )
		return false;
	return true;
}

bool AudioOutWrapper::ConcreteConfigure(const ProcessingConfig& cfg)
{	
	CopyAsConcreteConfig(mConfig, cfg);

	int size=0;
	if (mConfig.HasFrameSize())
		size = mConfig.GetFrameSize();

	ConfigureChildren(size);

	return true;
}

bool AudioOutWrapper::Do(void)
{
	bool res = Do(mInput.GetData());
	mInput.LeaveData();
	return res;
	
}

bool AudioOutWrapper::Do(Audio& in)
{
	mLeft.Do(in);
	mRight.Do(in);
}

} // namespace CLAM
