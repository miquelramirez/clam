#ifndef __TTIMEFREQMAGPOINT__
#define __TTIMEFREQMAGPOINT__

#include "DataTypes.hxx"

namespace CLAM
{
	namespace VM
	{
		struct TimeFreqMagPoint
		{
			TData   mMag;
			TData   mFreq;
			TData   mAttribute;
			TIndex  mTimeIndex;
			TIndex  mColorIndex;

			TimeFreqMagPoint();

			TimeFreqMagPoint( const TimeFreqMagPoint& );

			const TimeFreqMagPoint& operator=( const TimeFreqMagPoint& );

			~TimeFreqMagPoint();

			bool isValid() const
			{
				return ( mTimeIndex >= 0 );
			}

			bool operator==( TIndex i ) 
			{
				return mTimeIndex==i;
			}

			bool operator==( TIndex i ) const
			{
				return mTimeIndex==i;
			}

			bool operator<( TIndex i )
			{
				return mTimeIndex<i;
			}
					
			bool operator<( TIndex i ) const
			{
				return mTimeIndex<i;
			}

			bool operator>( TIndex i )
			{
				return mTimeIndex>i;
			}
					
			bool operator>( TIndex i ) const
			{
				return mTimeIndex>i;
			}
					

		};
	}
}

#endif 

