#ifndef _CSaltoStatTmplData_
#define _CSaltoStatTmplData_

#define MAX_PEAKS_IN_STAT_TMPL  100

#include "SpectralPeakArray.hxx"


namespace CLAM
{

class CSaltoFileIO;
class CSaltoEditor;

class CSaltoStatTmplData
{
//----------------------------------------------------------------------------//
friend class CSaltoFileIO;
friend class CSaltoEditor;

public:
CSaltoStatTmplData();
~CSaltoStatTmplData(){};

void                      SetStatPeakArray(SpectralPeakArray&  p);

SpectralPeakArray    GetStatPeakArray();
inline double             GetFundFreq()                       {return mFundFreq;}
inline double             GetNumValidPeaks()                  {return mNumValidPeaks;}
inline SpectralPeak  GetPeak(int pos)                    {return mpStatPeak[pos];}



//----------------------------------------------------------------------------//
private:

SpectralPeak mpStatPeak[MAX_PEAKS_IN_STAT_TMPL];
TIndex mNumValidPeaks;
double mFundFreq;

void                    SetPeak(int pos,SpectralPeak peak)      {mpStatPeak[pos]=peak;}
inline void             SetFundFreq(double d)               {mFundFreq = d;}
inline void             SetNumValidPeaks(int i)             {mNumValidPeaks = i;}

};

/* operator */
 inline EDataFormat DataFormat(class CSaltoStatTmplData&) { return eFmtDefault; }
 inline EDataFormat DataFormat(class CSaltoStatTmplData*&) { return eFmtDefault; }

//----------------------------------------------------------------------------//
} // end of namespace CLAM
#endif
