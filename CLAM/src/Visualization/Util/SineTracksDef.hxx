#ifndef __SINETRACKSDEF__
#define __SINETRACKSDEF__

#include <list>
#include <vector>
#include "TimeFreqMagPoint.hxx"


namespace CLAMVM
{

		typedef std::list< TimeFreqMagPoint >            SinusoidalTrack;
		typedef std::list< SinusoidalTrack >             SineTrackList;
		typedef std::vector< SinusoidalTrack::iterator>  SineTrackSpanEnds;
		

}

#endif // SineTracksDef.hxx
