
#ifndef __AudioOutPort_hxx__
#define __AudioOutPort_hxx__

#include "OutPort.hxx"
#include "Audio.hxx"

namespace CLAM
{

class AudioOutPort : public OutPort<TData>
{
public:
	AudioOutPort( const std::string & name = "unnamed in port", Processing * proc = 0 );
	virtual ~AudioOutPort();
	Audio & GetAudio();
	void SetSampleRate( TData sampleRate );
	TData GetSampleRate();
	
	
	static Audio & GetLastWrittenAudio( OutPortBase &, int offset = 0);

protected:
	Audio mAudio;
	Audio mLastWrittenAudio;
	Audio & GetLastWrittenAudio( int offset = 0 );
	
};

} // namespace CLAM

#endif // __AudioOutPort_hxx__

