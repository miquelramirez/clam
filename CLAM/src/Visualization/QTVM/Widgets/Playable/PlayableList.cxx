#include "PlayablePlot.hxx"
#include "PlayableList.hxx"

namespace CLAM
{
	namespace VM
	{
		PlayableList::PlayablePlotsList PlayableList::_pList;

		void PlayableList::Add(PlayablePlot* plot)
		{
			_pList.push_back(plot);
		}
		
		void PlayableList::Remove(PlayablePlot* plot)
		{
			PlayablePlotsList::iterator it = _pList.begin();
			for(;it != _pList.end();it++)
			{
				if((*it)==plot)
				{
					_pList.erase(it);
					break;
				}
			}
		}
		
		void PlayableList::Stop()
		{
			PlayablePlotsList::iterator it = _pList.begin();
			for(;it != _pList.end();it++)
			{
				if((*it)->IsPlaying())
				{
					(*it)->Stop();
					break;
				}
			}
		}
	}
}

// END

