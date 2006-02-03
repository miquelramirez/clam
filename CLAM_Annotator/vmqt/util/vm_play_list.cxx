#include "vm_player.hxx"
#include "vm_play_list.hxx"

namespace CLAM
{
	namespace VM
	{
		PlayList::PlayerList PlayList::pl_list;

		void PlayList::add(Player* player)
		{
			pl_list.push_back(player);
		}
		
		void PlayList::remove(Player* player)
		{
			PlayerList::iterator it = pl_list.begin();
			for(;it != pl_list.end(); it++)
			{
				if((*it)==player)
				{
					pl_list.erase(it);
					break;
				}
			}
		}
		
		void PlayList::stop()
		{
			PlayerList::iterator it = pl_list.begin();
			for(;it != pl_list.end(); it++)
			{
				if((*it)->is_playing())
				{
					(*it)->stop();
					break;
				}
			}
		}
	}
}

// END

