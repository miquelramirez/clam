#ifndef Auralizer_hxx
#define Auralizer_hxx

#include <vmBPFPlayer.hxx>

class Auralizer
{
public:
	CLAM::Audio mClick; ///< A vector of audios to produce click
	CLAM::Audio mOnsetAuralizationAudio; ///< Current audio with segmentation marks inserted
	const CLAM::Audio & mCurrentAudio;
	CLAM::VM::BPFPlayer* mPlayer;
	Auralizer(QWidget * parent, const CLAM::Audio & audio)
		: mCurrentAudio(audio)
	{
		mPlayer = new CLAM::VM::BPFPlayer(parent);
		initClick();
	}
	void play() { mPlayer->play(); }
	void stop() { mPlayer->stop(); }
	void pause() { mPlayer->pause(); }
	bool isPlaying() { return mPlayer->IsPlaying(); }
	void seekTo(double miliseconds, double endMiliseconds)
	{
		mPlayer->timeBounds(miliseconds, endMiliseconds);
	}
	void setSegmentation(const CLAM::Segmentation * segmentation)
	{
		if (!segmentation) return;
		const std::vector<double> & marks = segmentation->onsets();
		int nMarks = marks.size();
		mOnsetAuralizationAudio.SetSize(0);
		mOnsetAuralizationAudio.SetSize(mCurrentAudio.GetSize());
		mOnsetAuralizationAudio.SetSampleRate(mCurrentAudio.GetSampleRate());
		int size = mOnsetAuralizationAudio.GetSize();
		for (int i=0; i<nMarks; i++)
		{
			int samplePosition = Round(marks[i]*mCurrentAudio.GetSampleRate());
			if(samplePosition<size)
				mOnsetAuralizationAudio.SetAudioChunk(samplePosition,mClick);
		}
	}
	void setOptions(bool playOnsets, bool playLLDs)
	{
//		unsigned int LEFT_CHANNEL = 1;
		unsigned int RIGHT_CHANNEL = 2;
		mPlayer->SetAudioPtr(&mCurrentAudio);
		if (playOnsets)
			mPlayer->SetAudioPtr(&(mOnsetAuralizationAudio), RIGHT_CHANNEL);
		mPlayer->SetPlayingFlags( CLAM::VM::eAudio | (playLLDs?CLAM::VM::eUseOscillator:0));
	}
private:
	void initClick()
	{
		mClick.SetSize(1000);

		CLAM::OscillatorConfig cfg;
		cfg.SetFrequency(1000);
		cfg.SetAmplitude(1);
		cfg.SetSamplingRate(44100);
		CLAM::Oscillator osc(cfg);
		osc.Start();
		osc.Do(mClick);
		osc.Stop();
	}
};



#endif//Auralizer_hxx

