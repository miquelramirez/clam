#ifndef __TIMEFREQMAGPOINT__
#define __TIMEFREQMAGPOINT__

#include "DataTypes.hxx"

namespace CLAMVM
{

	using CLAM::TData;
	using CLAM::TIndex;

	// @todo: change the name of this struct to TimeFreqDatum or something similar
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

#endif // TimeFreqMagPoint.hxx
