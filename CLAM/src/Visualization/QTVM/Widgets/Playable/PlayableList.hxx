#ifndef __PLAYABLELIST__
#define __PLAYABLELIST__

#include <list>

namespace CLAM
{
	namespace VM
	{
		class PlayablePlot;
		
		class PlayableList
		{
			typedef std::list<PlayablePlot*> PlayablePlotsList;
				
			public:
				static void Add(PlayablePlot* plot);
				static void Remove(PlayablePlot* plot);
				static void Stop();

			private:
				static PlayablePlotsList _pList;
		};
	}
}

#endif

