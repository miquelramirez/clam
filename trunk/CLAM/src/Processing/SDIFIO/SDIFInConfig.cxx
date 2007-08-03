#include "SDIFInConfig.hxx"

namespace CLAM
{

void SDIFInConfig::DefaultInit()
{
	AddRelativePeakIndices();
	AddFileName();
	AddEnableResidual();
	AddEnablePeakArray();
	AddEnableFundFreq();
	AddSpectralRange();
	AddMaxNumPeaks();
	UpdateData();

/*	This may have to change to false but right now, Salto is the most important app that
	uses it and needs it set to true.*/
	SetRelativePeakIndices(true);
/* Is it a good idea to add a dummy FileName when we don't really have one?
*  In any case a few applications probably depend on this member already existing.
*  So let's leave this alone...
*/
	SetFileName( "nofile" );
	SetEnableResidual(true);
	SetEnablePeakArray(true);
	SetEnableFundFreq(true);
	SetSpectralRange(22050);
	SetMaxNumPeaks(100);
}

}


