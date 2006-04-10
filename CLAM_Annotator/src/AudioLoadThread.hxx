
#ifndef __AudioLoadThread_hxx__
#define __AudioLoadThread_hxx__

#include <QtCore/QThread>
#include <vector>
#include <CLAM/Audio.hxx>
#include <CLAM/AudioFile.hxx>

class AudioLoadThread : public QThread
{	
	bool mAborted;
	CLAM::Audio & mAudio;
	const CLAM::TSize readSize;
	unsigned nSamples;
	CLAM::AudioFile file;
	std::vector<CLAM::Audio> audioFrameVector;
	unsigned mNumber;
public:
	static unsigned number();
	AudioLoadThread(CLAM::Audio & audio, const std::string audioFileName);
	virtual ~AudioLoadThread();
	void run();
};



#endif // __AudioLoadThread_hxx__

