#ifndef __DPAUDIOSTEREO__
#define __DPAUDIOSTEREO__

#include <vector>
#include <string>
#include "Audio.hxx"

namespace CLAM
{
	namespace VM
	{
		void PlotAudioStereo(	std::vector<Audio> data,
								const std::string& label="",
								int x=100, 
								int y=100, 
								int w=660, 
								int h=250 );
	}
}

#endif
