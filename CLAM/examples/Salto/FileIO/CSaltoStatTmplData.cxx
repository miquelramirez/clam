#include "CSaltoStatTmplData.hxx"
#include "GlobalEnums.hxx"

namespace CLAM
{

CSaltoStatTmplData::CSaltoStatTmplData()
:mFundFreq(0.0),
 mNumValidPeaks(0)
{
}

void CSaltoStatTmplData::SetStatPeakArray(SpectralPeakArray&  p)
{
  TIndex numPeaks = p.GetnPeaks();
  if (numPeaks>=MAX_PEAKS_IN_STAT_TMPL)
    numPeaks=MAX_PEAKS_IN_STAT_TMPL-1;
  
//  for(int i=0;i<numPeaks;i++) p.GetPeak(i,mpStatPeak[i]);
  for(int i=0;i<numPeaks;i++)
    mpStatPeak[i] = p.GetSpectralPeak(i);

  mNumValidPeaks = numPeaks; 
}

SpectralPeakArray CSaltoStatTmplData::GetStatPeakArray()
{
//  SpectralPeakArrayConfig paConfig;
//	paConfig.SetScale(EScale::eLinear);
//	SpectralPeakArray p(paConfig);
	SpectralPeakArray p;
	
	//TODO XA: maybe some are not necessary
	p.AddAll();
	p.UpdateData();
//  p.SetnMaxPeaks(200);

  for(int i=0;i<mNumValidPeaks;i++)
    p.AddSpectralPeak(mpStatPeak[i]); //peta

  return p;
}          

} // end of namespace CLAM