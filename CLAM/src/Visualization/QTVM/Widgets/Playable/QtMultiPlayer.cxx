#include "Player.hxx"
#include "PlayableList.hxx"
#include "QtMultiPlayer.hxx"

namespace CLAM
{
    namespace VM
    {
	QtMultiPlayer::QtMultiPlayer(QWidget* parent) 
	    : QtPlayer(parent),
	      mCurrentPlayer(0)
	{
	}
		
	QtMultiPlayer::~QtMultiPlayer()
	{
	    for(unsigned i=0; i < mPlayers.size(); i++)
	    {
		if(mPlayers[i]->IsPlaying()) mPlayers[i]->Stop();
	    }
	}
		
	void QtMultiPlayer::AddPlayer(Player* player)
	{
	    mPlayers.push_back(player);
	}

	void QtMultiPlayer::play()
	{
	    if(!mPlayers.size()) return;
	    PlayableList::Stop();
	    mPlayers[mCurrentPlayer]->Play();
	}
		
	void QtMultiPlayer::pause()
	{
	    if(!mPlayers.size()) return; 
	    mPlayers[mCurrentPlayer]->Pause();
	}
		
	void QtMultiPlayer::stop()
	{
	    if(!mPlayers.size()) return; 
	    for(unsigned i=0; i < mPlayers.size(); i++)
	    {
		if(mPlayers[i]->IsPlaying()) mPlayers[i]->Stop();
	    }
	}
		
   
	void QtMultiPlayer::SetPlaySegment(const MediaTime& time)
	{
	    if(!mPlayers.size()) return;
	    for(unsigned i=0; i < mPlayers.size(); i++)
	    {
		mPlayers[i]->SetBounds(time);
	    }
	}

	bool QtMultiPlayer::IsPlaying()
	{
	    if(!mPlayers.size()) return false;
	    bool playing = false;
	    for(unsigned i=0; i < mPlayers.size(); i++)
	    {
		if(mPlayers[i]->IsPlaying()) 
		{
		    playing=true;
		    break;
		}
	    }
	    return playing;
	}
		
    }
}

// END
