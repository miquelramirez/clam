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
	enum Status { Playing=0, Stopped=1, Paused=2 };

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
	/// Here you need to call GetNetwork().Start()
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
	virtual bool IsRealTime() const = 0;

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
protected:
	unsigned GetNControlSinks() const { return _controlSinks.size(); }
	unsigned GetNControlSources() const { return _controlSources.size(); }
	unsigned GetNSinks() const { return _audioSinks.size(); }
	unsigned GetNSources() const { return _audioSources.size(); }
	/** Updates the sources and sinks lists */
	void CacheSourcesAndSinks();
	const std::string & ControlSourceName(unsigned source) const
	{
		return _controlSources[source].name;
	}
	const std::string & ControlSinkName(unsigned sink) const
	{
		return _controlSinks[sink].name;
	}
	const std::string & SourceName(unsigned source) const
	{
		return _audioSources[source].name;
	}
	const std::string & SinkName(unsigned sink) const
	{
		return _audioSinks[sink].name;
	}
	void SetSourceBuffer(unsigned source, const float * data, unsigned nframes);
	void SetSinkBuffer(unsigned sink, float * data, unsigned nframes);
	void ReadControlSource(unsigned source, const float * data);
	void FeedControlSink(unsigned sink, float * data);
	void SetSinkFrameSize(unsigned sink, unsigned frameSize);
	void SetSourceFrameSize(unsigned source, unsigned frameSize);
private:
	Network::Processings GetSources()
	{
		return GetNetwork().getOrderedProcessingsByAttribute("port_source_type");
	}

	Network::Processings GetSinks()
	{
		return GetNetwork().getOrderedProcessingsByAttribute("port_sink_type");
	}
	Network::Processings GetControlSources()
	{
		return GetNetwork().getOrderedProcessingsByAttribute("control_source_type", /*horizontal order*/ true);
	}
	Network::Processings GetControlSinks()
	{
		return GetNetwork().getOrderedProcessingsByAttribute("control_sink_type", /*horizontal order*/ true);
	}
	template <typename ProcessingType>
	void SetFrameAndHopSizeIf(Processing * proc, unsigned bufferSize, unsigned port)
	{
		if(typeid(*proc)!=typeid(ProcessingType)) return;
		((ProcessingType*)proc)->SetFrameAndHopSize(bufferSize, port);
		
	}
	template <typename ProcessingType>
	void SetExternalBuffer(Processing * proc, const float * data, unsigned nframes, unsigned port)
	{
		if(typeid(*proc)!=typeid(ProcessingType)) return;
		((ProcessingType*)proc)->SetExternalBuffer(data, nframes, port);
	}
	template <typename ProcessingType>
	void SetExternalBuffer(Processing * proc, float * data, unsigned nframes, unsigned port)
	{
		if(typeid(*proc)!=typeid(ProcessingType)) return;
		((ProcessingType*)proc)->SetExternalBuffer(data, nframes, port);
	}

private:
	struct ExportedPort
	{
		ExportedPort(Processing * aProcesing, unsigned aPort, const std::string & aName)
			: processing(aProcesing)
			, port(aPort)
			, name(aName)
		{
		}
		Processing * processing;
		unsigned port;
		std::string name;
	};
	typedef std::vector <ExportedPort> ExportedPorts;
	ExportedPorts _audioSources;
	ExportedPorts _audioSinks;
protected:
	ExportedPorts _controlSources;
	ExportedPorts _controlSinks;
private:
	Network *_network;
	volatile Status _status;
};

} //namespace CLAM

#endif // NetworkPlayer_hxx

