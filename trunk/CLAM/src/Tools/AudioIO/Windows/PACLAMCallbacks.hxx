#ifndef __PACLAMCALLBACKS__
#define __PACLAMCALLBACKS__

#include <portaudio.h>

namespace CLAM
{
	// PortAudio event callback prototype
	/*	typedef int (PortAudioCB)(
               void *inputBuffer, void *outputBuffer,
               unsigned long framesPerBuffer,
               PaTimestamp outTime, void *userData );
	*/
	
	int monoOutCallback( void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 PaTimestamp outTime, void *userData );

	int stereoOutCallback( void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 PaTimestamp outTime, void *userData );

	int multiOutCallback( void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 PaTimestamp outTime, void *userData );

	int monoInCallback( void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 PaTimestamp outTime, void *userData );

	int stereoInCallback( void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 PaTimestamp outTime, void *userData );

	int multiInCallback( void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 PaTimestamp outTime, void *userData );

	int stereoFDCallback(void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 PaTimestamp outTime, void *userData );
}

#endif // PACLAMCallbacks.hxx
