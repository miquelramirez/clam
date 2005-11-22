
#include "AudioLoadThread.hxx"
#include "MultiChannelAudioFileReaderConfig.hxx"
#include "MultiChannelAudioFileReader.hxx"

unsigned AudioLoadThread::number()
{
	static unsigned number = 0;
	return number++;
}

AudioLoadThread::AudioLoadThread(CLAM::Audio & audio, const std::string audioFileName)
	: mAborted(false)
	, mAudio(audio)
	, readSize(4096)
	, mNumber(number())
{
	std::cout << "AudioLoader " << mNumber << " created..." << std::endl;
	file.OpenExisting(audioFileName);
	int nChannels = file.GetHeader().GetChannels();
	float samplingRate = file.GetHeader().GetSampleRate();
	nSamples = unsigned( file.GetHeader().GetLength()/1000.0*samplingRate );
	audioFrameVector.resize(nChannels);
	for (int i=0;i<nChannels;i++)
		audioFrameVector[i].SetSize(readSize);
	mAudio.SetSize(0);
	mAudio.SetSize(nSamples);
	mAudio.SetSampleRate(samplingRate);
}
AudioLoadThread::~AudioLoadThread()
{
	std::cout << "Aborting audio loader " << mNumber << "..." << std::endl;
	mAborted=true;
	wait();
	std::cout << "AudioLoader " << mNumber << " destroyed..." << std::endl;
}
void AudioLoadThread::run()
{
	std::cout << "AudioLoader " << mNumber << " running..." << std::endl;
	CLAM::MultiChannelAudioFileReaderConfig cfg;
	cfg.SetSourceFile( file );
	CLAM::MultiChannelAudioFileReader reader(cfg);
	reader.Start();
	int beginSample=0;
	while(reader.Do(audioFrameVector))
	{
		mAudio.SetAudioChunk(beginSample,audioFrameVector[0]);
		beginSample+=readSize;
		if (mAborted) break;
		if ( beginSample+readSize > nSamples ) break;
	}
//		mAudio.SetSize(beginSample);
	reader.Stop();
	std::cout << "AudioLoader " << mNumber << " finished..." << std::endl;
}

