#ifndef _CSaltoResidualSynthesis_
#define _CSaltoResidualSynthesis_

#include "SpectralPeakArray.hxx"
#include "DataTypes.hxx"
#include "Spectrum.hxx"
#include "CSaltoSynthFrame.hxx"
#include "Frame.hxx"

namespace CLAM
{

class	CSaltoResidualSynthesis
{
public:
	CSaltoResidualSynthesis( TSize spectralFrameSize, TTime frameTime);
	~CSaltoResidualSynthesis();
	
	void DoAttackResidualSynthesis( CSaltoSynthFrame& synthFrame, TData attackResGain);

	void DoStatResidualSynthesis( CSaltoSynthFrame  &synthFrame,
	                              Frame *pStatResFrame,
								  TData            statResGain,
								  TData            resonanceFreq);
	void DoStatResidualSynthesis2( CSaltoSynthFrame  &synthFrame,
	                              Frame *pStatResFrame,
								  TData            statResGain,
								  TData            resonanceFreq);
private:
	// settings
	TSize     mSpectralFrameSize;
	double    mFrameTime;
															
	// storage

	//processing

};

}
#endif
