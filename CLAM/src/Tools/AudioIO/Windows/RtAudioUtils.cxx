#include "RtAudioUtils.hxx"

namespace CLAM
{

	ErrRtAudio::ErrRtAudio( const RtError& err )
		: Err( err.getMessage() )
	{
	}
}
