#ifndef _SDIFIn_
#define _SDIFIn_

#include "IndexArray.hxx"
#include "Processing.hxx"
#include "Err.hxx"
#include "Port.hxx"
#include "Segment.hxx"
#include <string.h>

namespace SDIF{class File;};//forward declaration

namespace CLAM{


class SDIFInConfig:public ProcessingConfig
{
public:

	DYNAMIC_TYPE_USING_INTERFACE (SDIFInConfig, 8, ProcessingConfig);
	DYN_ATTRIBUTE(0,public, std::string, Name);
	DYN_ATTRIBUTE(1,public, double, SpectralRange);
	DYN_ATTRIBUTE(2,public, TIndex, MaxNumPeaks);
	DYN_ATTRIBUTE(3,public, bool,EnableResidual);
	DYN_ATTRIBUTE(4,public, bool,EnablePeakArray);
	DYN_ATTRIBUTE(5,public, bool,EnableFundFreq);
	DYN_ATTRIBUTE(6,public, std::string,FileName);
/** If true, indices are treated as relative to previous frame (useful for some synthesis
	engines like SALTO). Else index found in SDIF is loaded as is.
 */
	DYN_ATTRIBUTE(7,public,bool,RelativePeakIndices);
	void DefaultInit();
};

class SDIFIn: public Processing
{
public:

	SDIFIn(const SDIFInConfig& c);
	SDIFIn();
	
	virtual ~SDIFIn();
	
	bool GetEnableResidual()        {return mConfig.GetEnableResidual();}
	bool GetEnablePeakArray()       {return mConfig.GetEnablePeakArray();}
	bool GetEnableFundFreq()        {return mConfig.GetEnableFundFreq();}
	
	bool Do(void);

	const ProcessingConfig &GetConfig() const;

	SDIF::File* mpFile;
	OutPortTmpl<Segment> Output;
protected:

	bool ConcreteStart();

	bool ConcreteStop();

private:
	
	bool ConcreteConfigure(const ProcessingConfig& c);
	
	SDIFInConfig mConfig;

// member variables
	TTime mLastCenterTime;
	IndexArray mPrevIndexArray;
	
};


};//CLAM
#endif
