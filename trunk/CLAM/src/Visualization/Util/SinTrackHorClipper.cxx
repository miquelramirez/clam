#include "SinTrackHorClipper.hxx"
#include <algorithm>
#include "Assert.hxx"

namespace CLAMVM
{
	
	void SinTrackHorClipper::Cull( TIndex start_frame, TIndex end_frame, 
				       SineTrackSpanEnds& line_starts,
				       SineTrackSpanEnds& line_ends )
	{
		line_starts.clear();
		line_ends.clear();
		// Some defensive programming...
		CLAM_ASSERT( mList != NULL, "No list to clip!" );
		CLAM_DEBUG_ASSERT( end_frame > start_frame, "End frame must be greater than start Frame\n" );
		CLAM_DEBUG_ASSERT( ( end_frame - start_frame ) >= 3, "Three frames span is the minimum meaningful\n" ); 

		SineTrackList& list = *mList;

		// vectors resizing checking
		// the policy is simple: never shrink


		unsigned ntracks = list.size();

		if ( ntracks > line_starts.size() )
			line_starts.resize( ntracks );
		if ( ntracks > line_ends.size() )
			line_ends.resize( ntracks );

		SineTrackList::iterator  currentSineTrack, listEnd;

		currentSineTrack = list.begin();
		listEnd = list.end();
				
		SineTrackSpanEnds::iterator currentSpanStart, currentSpanEnd;
				
		currentSpanStart = line_starts.begin();
		currentSpanEnd = line_ends.begin();

		while ( currentSineTrack != listEnd )
		{
					
			DetermineSpanPeaks( start_frame, end_frame, currentSineTrack, currentSpanStart, currentSpanEnd );

			currentSineTrack++;
			currentSpanStart++;
			currentSpanEnd++;
		} 
	}

	void SinTrackHorClipper::DetermineSpanPeaks( TIndex frame0, TIndex frame1, SineTrackList::iterator itrack,
						     peak_iterator& start, peak_iterator& end )
	{
		// "Best" case: the span can be trivially rejected
				
		if ( itrack->front() > frame1 ) // track is to the right of the spanned frames
		{
			SinusoidalTrack::iterator	last = itrack->begin();
			*start = *end = last; // void span
			return;						
		}

		if ( itrack->back() < frame0 ) // track is to left of the spanned frames
		{
			SinusoidalTrack::iterator	last = itrack->begin();
			*start = *end = last;

			return;
		}

		// "Worst" case: the span is somewhere inside the list

		SinusoidalTrack::iterator nodeStart= itrack->begin();

		while ( (nodeStart != itrack->end()) )
		{
			if ( nodeStart->mTimeIndex >= frame0 )
			{
				if ( nodeStart == itrack->begin() )
				{
					nodeStart = itrack->begin();
					break;
				}
				nodeStart--;
				break;
			}
			nodeStart++;
		}

		CLAM_ASSERT( nodeStart!= itrack->end(), "Error!: The Horizontal clipping algorithm has failed miserably!" );
		*start = nodeStart;
				
		SinusoidalTrack::iterator nodeEnd = nodeStart;

		nodeEnd++;
		/**
		 *   @todo: find a better way of handling the popping at the extreme
		 */
		while( (nodeEnd != itrack->end()) && ( nodeEnd->mTimeIndex <= frame1+10 ) ) 
		{
			nodeEnd++;
		}
			
		if ( nodeEnd == itrack->end() ) 
			nodeEnd--; // points to the last node in the list

		*end = nodeEnd;
	}
		
}
