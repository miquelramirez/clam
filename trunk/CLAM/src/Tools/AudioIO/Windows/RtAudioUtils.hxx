#ifndef __RTAUDIOUTILS__
#define __RTAUDIOUTILS__

#include "Err.hxx"
#include "RtAudio.h"

// Simple wrapper for RtAudio exception which do not
// derive from std::exception

namespace CLAM
{

	class ErrRtAudio : public Err
	{
	public:
		ErrRtAudio( const RtError& rtErr );
	};

}

#endif // RTAudioUtils.hxx
