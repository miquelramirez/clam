
#ifndef _CSaltoData_
#define _CSaltoData_

namespace CLAM
{

class CSaltoData
{
//----------------------------------------------------------------------------//
CSaltoData();
~CSaltoData();
//----------------------------------------------------------------------------//
public:
inline void  SetSegLength(TIndex val)       {mSegmentLength = val;}
inline void  SetInterpolStart(TIndex val)   {mInterpolationStart = val;}
inline void  SetInterpolEnd(TIndex val)     {mInterpolationEnd = val;}
inline void  SetLoopStart(TIndex val)       {mLoopStart = val;}
inline void  SetLoopEnd(TIndex val)         {mLoopEnd = val;}

inline TIndex  GetSegLength()               {return mSegmentLength;}
inline TIndex  GetInterpolStart()           {return mInterpolationStart;}
inline TIndex  GetInterpolEnd()             {return mInterpolationEnd;}
inline TIndex  GetLoopStart()               {return mLoopStart;}
inline TIndex  GetLoopEnd()                 {return mLoopEnd;}



//----------------------------------------------------------------------------//
private:

TIndex 
  mSegmentLength,
  mInterpolationStart,
  mInterpolationEnd,
  mLoopStart,
  mLoopEnd;
};

} // end of namespace CLAM

#endif
//----------------------------------------------------------------------------//