#ifndef __DPAUDIOMARKS__
#define __DPAUDIOMARKS__

#include <list>
#include <string>
#include "Audio.hxx"

namespace CLAM
{
	namespace VM
	{
		void PlotAudioMarks(	const Audio& audio,
								std::list<unsigned>& marks,
								const std::string& label="",
								int x=100, 
								int y=100, 
								int w=500, 
								int h=225	);
	}
}

#endif
