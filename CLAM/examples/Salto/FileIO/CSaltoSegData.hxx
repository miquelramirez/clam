
#ifndef _CSaltoSegData_
#define _CSaltoSegData_

#include "DataTypes.hxx"
#include "EDataFormat.hxx"

namespace CLAM
{

class CSaltoSegData
{
//----------------------------------------------------------------------------//
public:
CSaltoSegData();
~CSaltoSegData();
//----------------------------------------------------------------------------//

inline void   SetSegLength(TIndex val)       {mSegmentLength = val;}
inline void   SetPlayBackStart(TIndex val)   {mPlayBackStart = val;}
inline void   SetIndividualVolume(TIndex val){mIndividualVolume = val;}  
inline void   SetLoopStart(TIndex val)       {mLoopStart = val;}
inline void   SetLoopEnd(TIndex val)         {mLoopEnd = val;}
inline void   SetInterpolStart(TIndex val)   {mInterpolationStart = val;}
inline void   SetInterpolEnd(TIndex val)     {mInterpolationEnd = val;}
inline void   SetResFadeStart(TIndex val)    {mResFadeStart = val;}
inline void   SetResFadeEnd(TIndex val)      {mResFadeEnd = val;}
inline void   SetAttackResVolume(double val) {mAttackResVolume = val;}
inline void   SetStatResVolume(double val)   {mStatResVolume = val;}

inline TIndex  GetSegLength()               {return mSegmentLength;}
inline TIndex  GetPlayBackStart()           {return mPlayBackStart;}
inline TIndex  GetIndividualVolume()        {return mIndividualVolume;}
inline TIndex  GetLoopStart()               {return mLoopStart;}
inline TIndex  GetLoopEnd()                 {return mLoopEnd;}
inline TIndex  GetInterpolStart()           {return mInterpolationStart;}
inline TIndex  GetInterpolEnd()             {return mInterpolationEnd;}
inline TIndex  GetResFadeStart()            {return mResFadeStart;}
inline TIndex  GetResFadeEnd()              {return mResFadeEnd;}
inline double  GetAttackResVolume()         {return mAttackResVolume;}
inline double  GetStatResVolume()           {return mStatResVolume;}

// operators
CSaltoSegData& operator=(const CSaltoSegData &source);


//----------------------------------------------------------------------------//
private:

TIndex 
  mSegmentLength,
  mInterpolationStart,
  mInterpolationEnd,
  mPlayBackStart,
  mIndividualVolume,
  mLoopStart,
  mLoopEnd,
  mResFadeStart,
  mResFadeEnd;

double
  mAttackResVolume,
  mStatResVolume;
};


inline EDataFormat DataFormat(class CSaltoSegData&) { return eFmtDefault; }
inline EDataFormat DataFormat(class CSaltoSegData*&) { return eFmtDefault; }

 
} // end of namespace CLAM
//----------------------------------------------------------------------------//
#endif
