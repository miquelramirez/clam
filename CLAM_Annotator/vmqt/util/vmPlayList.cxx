#include "vmPlayer.hxx"
#include "vmPlayList.hxx"

namespace CLAM
{
	namespace VM
	{
		PlayList::PlayerList PlayList::mList;

		void PlayList::Add(Player* player)
		{
			mList.push_back(player);
		}
		
		void PlayList::Remove(Player* player)
		{
			PlayerList::iterator it = mList.begin();
			for(;it != mList.end(); it++)
			{
				if((*it)==player)
				{
					mList.erase(it);
					break;
				}
			}
		}
		
		void PlayList::Stop()
		{
			PlayerList::iterator it = mList.begin();
			for(;it != mList.end(); it++)
			{
				if((*it)->IsPlaying())
				{
					(*it)->stop();
					break;
				}
			}
		}
	}
}

// END

