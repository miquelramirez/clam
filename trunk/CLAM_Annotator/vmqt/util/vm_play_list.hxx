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
			static void add(Player* player);
			static void remove(Player* player);
			static void stop();

		private:
			static PlayerList pl_list;
		};
	}
}

#endif

