
#ifndef __AudioOutPortPublisher_hxx__
#define __AudioOutPortPublisher_hxx__

#include "OutPortPublisher.hxx"
#include "Audio.hxx"

namespace CLAM
{

class AudioOutPortPublisher : public OutPortPublisher<TData>
{
public:
	AudioOutPortPublisher( const std::string & name = "unnamed out port", Processing * proc = 0 );
	Audio & GetAudio();
protected:
	Audio mAudio;
};

} // namespace CLAM

#endif // __AudioOutPortPublisher_hxx__

