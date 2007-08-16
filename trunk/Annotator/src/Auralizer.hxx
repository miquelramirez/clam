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

