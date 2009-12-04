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

#ifndef NetworkPlayer_hxx
#define NetworkPlayer_hxx

#include "Network.hxx"
#include "AudioSource.hxx"
#include "AudioSink.hxx"

namespace CLAM
{

namespace {

template<typename Container> unsigned GetSize(Container const& t)
{
	unsigned nrOfPorts = 0;
	for (typename Container::const_iterator it = t.begin(); it != t.end(); ++it)
		nrOfPorts += (*it)->GetPorts().size();
	return nrOfPorts;
}

unsigned GetSizeSourceBuffer(const std::vector<Processing*>& t)
{
	unsigned nrOfPorts = 0;
	
	for (std::vector<Processing*>::const_iterator it = t.begin(); it != t.end(); ++it)
		nrOfPorts += ((AudioSourceBuffer*)(*it))->GetPorts().size();
	return nrOfPorts;
}

unsigned GetSizeSinkBuffer(const std::vector<Processing*>& t)
{
	unsigned nrOfPorts = 0;
	for (std::vector<Processing*>::const_iterator it = t.begin(); it != t.end(); ++it)
		nrOfPorts += ((AudioSinkBuffer*)(*it))->GetPorts().size();
	return nrOfPorts;
}


} // namespace

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
	enum Status { Playing=0, Stopped=1, Paused=2 };

	Network::AudioSources _audioSources;
	Network::AudioSinks _audioSinks;

	Network::Processings _audioSourcesBuffer;
	Network::Processings _audioSinksBuffer;	
public:
	NetworkPlayer()
		: _network(NULL)
		, _status(Stopped)
	{
	}

	virtual ~NetworkPlayer()
	{
	}

	/// Should return true when the backend is able to run the network
	virtual bool IsWorking() = 0;
	
	/// Whenever the backend is not working, this method returns the reason
	virtual std::string NonWorkingReason() = 0;

	/// Redefine to add any initialization after being attached to a network
	/// TODO: Consider removing it as just Jack backend uses it but it is redundant
	virtual void Init() {}
	
	/// Redefine to make the backend ready to process and start the network.
	/// If IsPlaying() should do nothing.
	/// If it IsPaused() you should consider just call BePlaying()
	/// without starting the processings.
	virtual void Start()=0; // { if (not IsPlaying()) BePlaying(); }
	
	/// Redefine it to deactivate the backend.
	virtual void Stop()=0; // { if (not IsStopped()) BeStopped(); }
	
	virtual void Pause() { if (IsPlaying()) BePaused(); }

	void SetNetworkBackLink( Network& net )
	{
		_network=&net;
	}

	void BePaused() { _status=Paused; }
	void BeStopped() { _status=Stopped; }
	void BePlaying() { _status=Playing; }
	bool IsPaused() const { return _status==Paused; }
	bool IsStopped() const { return _status==Stopped; }
	bool IsPlaying() const { return _status==Playing; }

	virtual unsigned BackendBufferSize()
	{
		return 512;
	}

	virtual unsigned BackendSampleRate()
	{
		return 44100;
	}

	std::string SourcesAndSinksToString();

protected:
	Network& GetNetwork()
	{
		CLAM_ASSERT( (_network!=NULL), "NetworkPlayer::GetNetwork() : NetworkPlayer does not have any Network");
		return *_network;
	}

	Network::AudioSources GetAudioSources() 
	{
		return GetNetwork().getOrderedSources();
	}

	Network::AudioSinks GetAudioSinks() 
	{ 
		return GetNetwork().getOrderedSinks(); 
	}

	Network::Processings GetAudioSourcesBuffer() 
	{	
		return GetNetwork().getOrderedProcessings("AudioSourceBuffer", /*vertical order */ true);
	}

	Network::Processings GetAudioSinksBuffer() 
	{ 
		return GetNetwork().getOrderedProcessings("AudioSinkBuffer", /*vertical order */ false);
	}

	void CacheSourcesAndSinks()
	{
		_audioSources = GetAudioSources();
		_audioSinks = GetAudioSinks();
		_audioSourcesBuffer = GetAudioSourcesBuffer();
		_audioSinksBuffer = GetAudioSinksBuffer();
	}

private:
	Network *_network;
	volatile Status _status;
};

} //namespace CLAM

#endif // NetworkPlayer_hxx

