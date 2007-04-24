#ifndef _FreewheelingNetworkPlayer_hxx_
#define _FreewheelingNetworkPlayer_hxx_

#include "NetworkPlayer.hxx"
#include "AudioSource.hxx"
#include "AudioSink.hxx"

namespace CLAM
{


class FreewheelingNetworkPlayer : public NetworkPlayer
{
	int mBufferSize;
	std::string mInFilename;
	std::string mOutFilename;
	std::vector<std::string> mFilenames;

public:
	FreewheelingNetworkPlayer();
	void InputFile( const std::string& );
	void OutputFile( const std::string& );
	// base class (virtual) interface:
	bool IsWorking() const;
	std::string NonWorkingReason() const;
	virtual bool IsCallbackBased() const { return true; }
	virtual void Start();
	virtual void Stop();

	void ProcessInputFile();
	
};

}
#endif

