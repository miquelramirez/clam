#ifndef __TPARTIAL__
#define __TPARTIAL__

#include "DataTypes.hxx"

namespace CLAM
{
	namespace VM
	{
		struct Partial
		{
			TData    mMag;
			TData    mFreq;
			TData    mPhase;
			TIndex   mTrackId;
		};
	}
}

#endif 


