#include "PortAudioUtils.hxx"

namespace CLAM
{

	ErrPortAudio::ErrPortAudio( const char* msg )
		: Err( msg )
	{
	}

	ErrPortAudio::ErrPortAudio( const char* msg, PaError errval )
		: Err()
	{
		const char* paErrStr = Pa_GetErrorText( errval );
		mMsg = (char*) malloc( strlen( msg ) + strlen( paErrStr ) + 1 );
		strcpy( mMsg, msg );
		strcat( mMsg, paErrStr );
	}
	
}
