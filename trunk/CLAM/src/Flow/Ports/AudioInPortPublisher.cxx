
#include "AudioInPortPublisher.hxx"

namespace CLAM
{
	
AudioInPortPublisher::AudioInPortPublisher( const std::string & name, Processing * proc )
	: InPortPublisher<TData>(name, proc)
{
}
/*
Audio & AudioInPortPublisher::GetAudio()
{
	mAudio.GetBuffer().SetPtr( &(mPublishedInPort->GetData()), mPublishedInPort->GetSize() );
	return mAudio;
}*/

} // namespace CLAM

