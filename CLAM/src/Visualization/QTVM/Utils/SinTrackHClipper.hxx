#ifndef __SINTRACKHCLIPPER__
#define __SINTRACKHCLIPPER__

#include <vector>
#include "DataTypes.hxx"
#include "SinTracksDef.hxx"

namespace CLAM
{
	namespace VM
	{
		class SinTrackHorClipper
		{
			typedef SineTrackSpanEnds::iterator peak_iterator;

			public:
				
				SinTrackHorClipper( )
						: mList( NULL)
				{
				}

				void SetTrackList( SineTrackList& list )
				{
					mList = &list;
				}

				void Cull( TIndex start_frame, TIndex end_frame, 
						   SineTrackSpanEnds& line_starts,
						   SineTrackSpanEnds& line_ends );

			protected:	
				void DetermineSpanPeaks( TIndex frame0, TIndex frame1, SineTrackList::iterator itrack,
										 peak_iterator& start, peak_iterator& end );
			private:	
				SineTrackList*   mList;
		};
	}
}


#endif 


