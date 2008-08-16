/*
 * Copyright (c) 2001-2007 MUSIC TECHNOLOGY GROUP (MTG)
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


#ifndef _PORTAUDIO_NETWORK_PLAYER_HXX_
#define _PORTAUDIO_NETWORK_PLAYER_HXX_

#include <iostream>
#include <string>
#include "NetworkPlayer.hxx"
#include <portaudio.h>

namespace CLAM
{


class PANetworkPlayer : public NetworkPlayer
{
	int mPreferredBufferSize;
	int mSamplingRate;

	PaStream * mPortAudioStream;
	PaError mError;
	std::string mErrorMessage;
	bool mNeedsPriority;

public:
	//When created in the prototyper
	PANetworkPlayer(const std::string & networkFile);
	//When created in neteditor
	PANetworkPlayer();
	virtual ~PANetworkPlayer();

	// base class (virtual) interface:
	bool IsWorking() const;
	std::string NonWorkingReason() const;
	virtual void Start();
	virtual void Stop();
	
private:
	static int ProcessCallback (
							const void *inputBuffers,
							void *outputBuffers,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData);
	inline bool CheckPaError(PaError result);
	//Buffer copying methods
	void Do(const void *inputBuffers, void *outputBuffers, unsigned long framesPerBuffer);
	void DoInPorts(float** input, unsigned long nframes);
	void DoOutPorts(float** output, unsigned long nframes);
	void MuteOutBuffers(float** output, unsigned long nframes);
};

} //end namespace CLAM

#endif

