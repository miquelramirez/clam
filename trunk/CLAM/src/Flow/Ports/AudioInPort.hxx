
#ifndef __AudioInPort_hxx__
#define __AudioInPort_hxx__

#include "InPort.hxx"
#include "Audio.hxx"

namespace CLAM
{

class AudioInPort : public InPort<TData>
{
public:
	AudioInPort( const std::string & name = "unnamed in port", Processing * proc = 0 );
	virtual ~AudioInPort();
	const Audio & GetAudio();

	// TODO: Rethink propagation of audio sampling rate
	void SetSampleRate( TData sampleRate );
	TData GetSampleRate();
protected:
	Audio mAudio;		
};

} // namespace CLAM

#endif // __AudioInPort_hxx__

