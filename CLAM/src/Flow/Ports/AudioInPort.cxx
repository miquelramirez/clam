
#include "AudioInPort.hxx"

namespace CLAM
{

AudioInPort::AudioInPort( const std::string & name, Processing * proc )
	: InPort<TData>(name,proc)
{
	const int size = 256; // arbitrary default value. It makes more sense that size==1 like generic ports.
	SetSize(size); 
	SetHop(size);
}

AudioInPort::~AudioInPort()
{
}

const Audio & AudioInPort::GetAudio()
{
	mAudio.GetBuffer().SetPtr( &(mRegion[0]), mRegion.Size() );
	return mAudio;
}

void AudioInPort::SetSampleRate( TData sampleRate )
{
	mAudio.SetSampleRate( sampleRate );
}

TData AudioInPort::GetSampleRate()
{
	return mAudio.GetSampleRate();
}

} // namespace CLAM

