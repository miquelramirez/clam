
#include "AudioOutPort.hxx"

namespace CLAM
{

AudioOutPort::AudioOutPort( const std::string & name, Processing * proc )
	: OutPort<TData>(name,proc)
{
	const int size = 256; // arbitrary default value. It makes more sense that size==1 like generic ports.
	SetSize(size); 
	SetHop(size);
}

AudioOutPort::~AudioOutPort()
{
}

Audio & AudioOutPort::GetAudio()
{
	mAudio.GetBuffer().SetPtr( &(mRegion[0]), mRegion.Size() );
	return mAudio;
}

void AudioOutPort::SetSampleRate( TData sampleRate )
{
	mAudio.SetSampleRate( sampleRate );
}

TData AudioOutPort::GetSampleRate()
{
	return mAudio.GetSampleRate();
}

Audio & AudioOutPort::GetLastWrittenAudio( int offset )
{
	CLAM_DEBUG_ASSERT( (0 <= offset) && (offset <= GetSize()), 
		"AudioOutPort::GetLastWrittenData - Index out of bounds" );

	mAudio.GetBuffer().SetPtr( &(mRegion.GetLastWrittenData(offset)), mRegion.Size() );
	return mAudio;
}

Audio & AudioOutPort::GetLastWrittenAudio( OutPortBase & out, int offset )
{
	try
	{
		AudioOutPort & concreteOut = dynamic_cast< AudioOutPort& >(out);
		return concreteOut.GetLastWrittenAudio( offset );
	}
	catch(...)
	{
		CLAM_ASSERT( false, "AudioOutPort::GetLastWrittenAudio - Passed an outport of wrong type" );
	}
	return *(Audio *)NULL;
		
}

} // namespace CLAM

