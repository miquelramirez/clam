#ifndef __SINTRACKSDEF__
#define __SINTRACKSDEF__

#include <list>
#include <vector>
#include "TTimeFreqMagPoint.hxx"


namespace CLAM
{
	namespace VM
	{
		typedef std::list< TimeFreqMagPoint >            SinusoidalTrack;
		typedef std::list< SinusoidalTrack >             SineTrackList;
		typedef std::vector< SinusoidalTrack::iterator>  SineTrackSpanEnds;
	}
}

#endif


