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

