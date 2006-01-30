#ifndef __VMQT_PLAY_LIST_H__
#define __VMQT_PLAY_LIST_H__

#include <list>

namespace CLAM
{
	namespace VM
	{
		class CLAM::VM::Player;
		
		class PlayList
		{
			typedef std::list<CLAM::VM::Player*> PlayerList;
				
		public:
			static void add(CLAM::VM::Player* player);
			static void remove(CLAM::VM::Player* player);
			static void stop();

		private:
			static PlayerList pl_list;
		};
	}
}

#endif

