
#ifndef __AudioInPortPublisher_hxx__
#define __AudioInPortPublisher_hxx__

#include "InPortPublisher.hxx"
#include "Audio.hxx"

namespace CLAM
{

class AudioInPortPublisher : public InPortPublisher<TData>
{
public:
	AudioInPortPublisher( const std::string & name = "unnamed in port", Processing * proc = 0 );
	virtual ~AudioInPortPublisher(){}
};

} // namespace CLAM

#endif // __AudioInPortPublisher_hxx__

