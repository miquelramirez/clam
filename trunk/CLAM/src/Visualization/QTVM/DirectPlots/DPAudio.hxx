#ifndef __DPAUDIO__
#define __DPAUDIO__

#include <string>
#include "Audio.hxx"

namespace CLAM
{
	namespace VM
	{
		void PlotAudio(	const Audio& audio,
						const std::string& label="",
						int x=100, 
						int y=100, 
						int w=500, 
						int h=225 );
	}
}

#endif
