#ifndef _SDIFInConfig_
#define _SDIFInConfig_

#include "IndexArray.hxx"
#include "Processing.hxx"
#include "Err.hxx"
#include "Filename.hxx"

namespace CLAM
{

class SDIFInConfig:public ProcessingConfig
{
public:

	DYNAMIC_TYPE_USING_INTERFACE (SDIFInConfig, 7, ProcessingConfig);
	DYN_ATTRIBUTE(0,public, double, SpectralRange);
	DYN_ATTRIBUTE(1,public, TIndex, MaxNumPeaks);
	DYN_ATTRIBUTE(2,public, bool,EnableResidual);
	DYN_ATTRIBUTE(3,public, bool,EnablePeakArray);
	DYN_ATTRIBUTE(4,public, bool,EnableFundFreq);
	DYN_ATTRIBUTE(5,public, Filename, FileName);
/** If true, indices are treated as relative to previous frame (useful for some synthesis
	engines like SALTO). Else index found in SDIF is loaded as is.
 */
	DYN_ATTRIBUTE(6,public,bool,RelativePeakIndices);
	void DefaultInit();
};

}

#endif
