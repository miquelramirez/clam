#include "SDIFInConfig.hxx"

namespace CLAM
{

void SDIFInConfig::DefaultInit()
{
	AddAll();
	UpdateData();

/*	This may have to change to false but right now, Salto is the most important app that
	uses it and needs it set to true.*/
	SetRelativePeakIndices(true);
	SetFileName( "nofile" );
	SetEnableResidual(true);
	SetEnablePeakArray(true);
	SetEnableFundFreq(true);
	SetSpectralRange(22050);
	SetMaxNumPeaks(100);

}

}


