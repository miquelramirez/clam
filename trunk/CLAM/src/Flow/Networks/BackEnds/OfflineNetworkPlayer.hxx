#ifndef _OfflineNetworkPlayer_hxx_
#define _OfflineNetworkPlayer_hxx_

#include "NetworkPlayer.hxx"
#include "AudioSource.hxx"
#include "AudioSink.hxx"


namespace CLAM
{


class OfflineNetworkPlayer : public NetworkPlayer
{
	std::vector<std::string> _inFileNames;
	std::vector<std::string> _outFileNames;
	std::vector<int> _outChannelsFiles;
	int _format;
	bool _enableLoopInputWavs;
	float _resultWavsTime;

public:
	OfflineNetworkPlayer()
		: _enableLoopInputWavs(false)
		, _resultWavsTime(0.0)
	{}
	void AddInputFile( const std::string& );
	void AddOutputFile( const std::string& );
	void AddNumChannels(int channel);
	void SetFormat(int format);
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

