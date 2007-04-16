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

#ifndef _NETWORK_PLAYER_HXX_
#define _NETWORK_PLAYER_HXX_

#include "Network.hxx"
#include "AudioSource.hxx"
#include "AudioSink.hxx"

namespace CLAM
{
/**
 * A NetworkPlayer is an object that controls the playback of a 
 * Network providing a high level transport like interface.
 * This class is an abstract class.
 * Subclasses contextualizes the network inside a given execution
 * context dealing with threading, callback calling and device mapping
 * on concrete architectures such as Alsa, PortAudio, Jack, VST, Ladspa...
 */
class NetworkPlayer
{
protected:
	typedef std::vector<AudioSource*> AudioSources;
	typedef std::vector<AudioSink*> AudioSinks;
public:
	NetworkPlayer()
	{
		SetStopped(true);
		mNetwork=NULL;
	}

	virtual ~NetworkPlayer()
	{
	}
	virtual bool IsCallbackBased() const = 0;
	/// Should return true when the backend is able to run the network
	virtual bool IsWorking() const = 0;
	/// Whenever the backend is not working, this method returns the reason
	virtual std::string NonWorkingReason() const = 0;
	
	virtual void Start()=0;
	virtual void Stop()=0;
	
	void SetNetworkBackLink( Network& net )
	{
		mNetwork=&net;
	}
	bool IsStopped()
	{
		return mStopped;
	}
protected:
	Network& GetNetwork()
	{
		CLAM_ASSERT( (mNetwork!=NULL), "NetworkPlayer::GetNetwork() : NetworkPlayer does not have any Network");
		return *mNetwork;
	}
	void SetStopped(const bool val)	{ mStopped=val; }
	void CollectSourcesAndSinks();
	AudioSources& GetAudioSources() { return mSources; }
	AudioSinks& GetAudioSinks() { return mSinks; }

	AudioSources mSources;
	AudioSinks mSinks;
private:
	Network *mNetwork;
	bool mStopped;

};
} //namespace CLAM

#endif // _NETWORK_PLAYER_HXX_

