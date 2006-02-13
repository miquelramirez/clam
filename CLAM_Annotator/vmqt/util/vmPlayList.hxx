#ifndef __VMQT_PLAY_LIST_H__
#define __VMQT_PLAY_LIST_H__

#include <list>

namespace CLAM
{
	namespace VM
	{
		class Player;
		
		class PlayList
		{
			typedef std::list<Player*> PlayerList;
				
		public:
			static void Add(Player* player);
			static void Remove(Player* player);
			static void Stop();

		private:
			static PlayerList mList;
		};
	}
}

#endif

