#ifndef _SDIFIn_
#define _SDIFIn_

#include "Segment.hxx"
#include "IndexArray.hxx"
#include "Processing.hxx"
#include "Err.hxx"
#include "OutPortTmpl.hxx"
#include "Filename.hxx"

namespace SDIF { class File; } //forward declaration

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

class SDIFIn: public Processing
{
public:

	SDIFIn(const SDIFInConfig& c);
	SDIFIn();
	
	virtual ~SDIFIn();

	const char * GetClassName() const {return "SDIFIn";}
	
	bool GetEnableResidual()        {return mConfig.GetEnableResidual();}
	bool GetEnablePeakArray()       {return mConfig.GetEnablePeakArray();}
	bool GetEnableFundFreq()        {return mConfig.GetEnableFundFreq();}
	
	bool Do(void);

	bool Do( CLAM::Segment& segment );

	const ProcessingConfig &GetConfig() const;

	SDIF::File* mpFile;
	OutPortTmpl<Segment> Output;
protected:

	bool ConcreteStart();

	bool ConcreteStop();

	bool LoadSDIFDataIntoSegment( CLAM::Segment& s );

private:
	
	bool ConcreteConfigure(const ProcessingConfig& c);
	
	SDIFInConfig mConfig;

// member variables
	TTime mLastCenterTime;
	IndexArray mPrevIndexArray;
	
};


};//CLAM
#endif
