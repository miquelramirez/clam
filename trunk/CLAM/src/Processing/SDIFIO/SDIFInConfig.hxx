#ifndef _SDIFInConfig_
#define _SDIFInConfig_

#include "IndexArray.hxx"
#include "Processing.hxx"
#include "Err.hxx"
#include "Filename.hxx"

namespace CLAM
{

class InSDIFFilename : public InFilename
{
public:
	InSDIFFilename(const std::string & s="") : InFilename(s) {}
	InSDIFFilename(const char * s) : InFilename(s) {}
	virtual const char * TypeFamily() const { return "SDIF"; }
	virtual const Filter * Filters() const
	{
		static const Filter filters[] = {
			{"SDIF", "*.sdif"},
			{"SDIF", "*.sdf"},
			{0,0}
		};
		return filters;
	}
};
CLAM_TYPEINFOGROUP(BasicCTypeInfo, InSDIFFilename);

class SDIFInConfig:public ProcessingConfig
{
public:

	DYNAMIC_TYPE_USING_INTERFACE (SDIFInConfig, 9, ProcessingConfig);
	DYN_ATTRIBUTE(0,public, double, SpectralRange);
	DYN_ATTRIBUTE(1,public, TIndex, MaxNumPeaks);
	DYN_ATTRIBUTE(2,public, bool,EnableResidual);
	DYN_ATTRIBUTE(3,public, bool,EnablePeakArray);
	DYN_ATTRIBUTE(4,public, bool,EnableFundFreq);
	DYN_ATTRIBUTE(5,public, InSDIFFilename, FileName);
/** If true, indices are treated as relative to previous frame (useful for some synthesis
	engines like SALTO). Else index found in SDIF is loaded as is.
 */
	DYN_ATTRIBUTE(6,public,bool,RelativePeakIndices);
	DYN_ATTRIBUTE(7,public,int,NumberOfFramesToPreload);
	DYN_ATTRIBUTE(8,public,int,NumberOfFramesToLoad);
	
	void DefaultInit();
};

}

#endif
