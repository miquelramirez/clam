
#ifndef _AutoPanner_hxx_
#define _AutoPanner_hxx_

#include "DataTypes.hxx"
#include "OutControl.hxx"

namespace CLAM
{
/**
 * \todo this class has to be a concrete Processing
 */
class AutoPanner
{
private:
	TData mFreq;
	TData mSamplingRate;
	TData mPhase;
	TData mDeltaPhase;
	int mFrameSize;

public:
	OutControl mLeft;
	OutControl mRight;

public:
	AutoPanner(TData freq,
		      TData samplingRate,
		      TData phase = 0.0,
		      int frameSize = 1);

	void Configure(TData freq,
		       TData samplingRate,
		       TData phase,
		       int frameSize);
	bool Do();
	virtual ~AutoPanner(){}
};

} //namespace CLAM

#endif
