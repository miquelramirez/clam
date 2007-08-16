#ifndef Auralizer_hxx
#define Auralizer_hxx

#include <vmBPFPlayer.hxx>

class Auralizer
{
public:
	CLAM::VM::BPFPlayer* mPlayer;
	Auralizer(QWidget * parent)
	{
		mPlayer = new CLAM::VM::BPFPlayer(parent);
	}
	void play() { mPlayer->play(); }
	void stop() { mPlayer->stop(); }
	void pause() { mPlayer->pause(); }
	bool isPlaying() { return mPlayer->IsPlaying(); }
	void seekTo(double miliseconds, double endMiliseconds)
	{
		mPlayer->timeBounds(miliseconds, endMiliseconds);
	}
};



#endif//Auralizer_hxx

