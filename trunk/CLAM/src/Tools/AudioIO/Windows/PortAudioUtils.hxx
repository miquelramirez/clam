#ifndef __PORTAUDIOUTILS__
#define __PORTAUDIOUTILS__

#include <portaudio.h>
#include <cstdlib>
#include <cstring>
#include "Err.hxx"

namespace CLAM
{

typedef int (*PortAudioCallback)(
           void *inputBuffer, void *outputBuffer,
           unsigned long framesPerBuffer,
           PaTimestamp outTime, void *userData ) ;



class ErrPortAudio : public Err
{
public:
	ErrPortAudio( const char* msg ) throw();

	ErrPortAudio( const char* msg, PaError err ) throw();

};

#define CHECK_PA_ERROR( msg, errval ) if ( errval != paNoError) throw ErrPortAudio( msg, errval );


}

#endif // PortAudioUtils.hxx
