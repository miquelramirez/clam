
#include "CSaltoSegData.hxx"

namespace CLAM
{

CSaltoSegData::CSaltoSegData():
  mSegmentLength (0),
  mInterpolationStart(0),
  mInterpolationEnd(0),
  mPlayBackStart(0),
  mIndividualVolume(0),
  mLoopStart(0),
  mLoopEnd(0),
  mResFadeStart(0),
  mResFadeEnd(0),
  mAttackResVolume(0.0),
  mStatResVolume(0.0)
{
}

CSaltoSegData::~CSaltoSegData()
{
}

CSaltoSegData& 
CSaltoSegData::operator=(const CSaltoSegData &source)
{
  mSegmentLength        = source.mSegmentLength;
  mInterpolationStart   = source.mInterpolationStart;
  mInterpolationEnd     = source.mInterpolationEnd;
  mPlayBackStart        = source.mPlayBackStart;
  mIndividualVolume     = source.mIndividualVolume;
  mLoopStart            = source.mLoopStart;
  mLoopEnd              = source.mLoopEnd;
  mResFadeStart         = source.mResFadeStart;
  mResFadeEnd           = source.mResFadeEnd;
  mAttackResVolume      = source.mAttackResVolume;
  mStatResVolume        = source.mStatResVolume;
  return *this;
}

} // end of namespace CLAM

