#ifndef __DPAUDIOSTEREOMARKS__
#define __DPAUDIOSTEREOMARKS__

#include <list>
#include <vector>
#include <string>
#include "Audio.hxx"

namespace CLAM
{
	namespace VM
	{
		void PlotAudioStereoMarks(	std::vector<Audio> data,
									std::list<unsigned>& marks,
									const std::string& label="",
									int x=100, 
									int y=100, 
									int w=660, 
									int h=250 );
	}
}

#endif
