
#include "CSaltoTimbreVektor.hxx"

namespace CLAM
{

CSaltoTimbreVektor::CSaltoTimbreVektor():
mPitch			(0),
mAttack     (0),
mLevel		  (0),
mSmoothness	(0),
mTransitionFrames(2),
mNextPitch  (0)
{
}
CSaltoTimbreVektor::~CSaltoTimbreVektor()
{
}

} // end of namespace CLAM

