#ifndef __PARTIAL__
#define __PARTIAL__

#include "DataTypes.hxx"

namespace CLAMVM
{
		using CLAM::TData;
		using CLAM::TIndex;

		struct Partial
		{
				TData    mMag;
				TData    mFreq;
				TData    mPhase;
				TIndex   mTrackId;
		};
}

#endif // Partial.hxx
