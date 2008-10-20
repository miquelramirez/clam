#ifndef _MonoOfflineNetworkPlayer_hxx_
#define _MonoOfflineNetworkPlayer_hxx_

#include "NetworkPlayer.hxx"
#include "AudioSource.hxx"
#include "AudioSink.hxx"

namespace CLAM
{


class MonoOfflineNetworkPlayer : public NetworkPlayer
{
	std::vector<std::string> _filenames;
	bool _enableLoopInputWavs;
	float _resultWavsTime;


public:
	MonoOfflineNetworkPlayer()
		: _enableLoopInputWavs(false)
		, _resultWavsTime(0.0)
	{}
	void AddInputFile( const std::string& );
	void AddOutputFile( const std::string& );
	// base class (virtual) interface:
	bool IsWorking() const;
	std::string NonWorkingReason() const;
	virtual void Start();
	virtual void Stop();

	void ProcessInputFile();

// specific interface - not inherited	
	void EnableLoopInputWavs() 
	{
		_enableLoopInputWavs=true;
	}
	void SetResultWavsTime( double time)
	{
		_resultWavsTime=time;
	}
};

}
#endif

