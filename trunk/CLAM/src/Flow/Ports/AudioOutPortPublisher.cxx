
#include "AudioOutPortPublisher.hxx"

namespace CLAM
{
	
AudioOutPortPublisher::AudioOutPortPublisher( const std::string & name, Processing * proc )
	: OutPortPublisher<TData>(name, proc)
{
}

Audio & AudioOutPortPublisher::GetAudio()
{
	mAudio.GetBuffer().SetPtr( &(mPublishedOutPort->GetData()), mPublishedOutPort->GetSize() );
	return mAudio;
}

} // namespace CLAM

