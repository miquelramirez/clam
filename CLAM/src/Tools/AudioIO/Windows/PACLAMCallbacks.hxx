/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

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
	
	int monoOutCallback( const void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData );

	int stereoOutCallback( const void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData );

	int multiOutCallback( const void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData );

	int monoInCallback( const void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData );

	int stereoInCallback( const void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData );

	int multiInCallback( const void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData );

	int stereoFDCallback( const void *inputBuffer, void *outputBuffer, 
						 unsigned long framesPerBuffer, 
						 const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData );
}

#endif // PACLAMCallbacks.hxx

