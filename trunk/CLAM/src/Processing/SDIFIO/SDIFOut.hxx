#ifndef _SDIFOut_
#define _SDIFOut_

#include "IndexArray.hxx"
#include "Processing.hxx"
#include "Err.hxx"
#include "Segment.hxx"
#include "Filename.hxx"


namespace SDIF {class File;};//forward declaration

namespace CLAM{

class SDIFOutConfig:public ProcessingConfig
{
public:

	DYNAMIC_TYPE_USING_INTERFACE (SDIFOutConfig, 9, ProcessingConfig);
	DYN_ATTRIBUTE(0,public, TData, SpectralRange);
	DYN_ATTRIBUTE(1,public, TIndex, MaxNumPeaks);
	DYN_ATTRIBUTE(2,public, bool,EnableResidual);
	DYN_ATTRIBUTE(3,public, bool,EnablePeakArray);
	DYN_ATTRIBUTE(4,public, bool,EnableFundFreq);
	DYN_ATTRIBUTE(5,public, Filename,FileName);
	DYN_ATTRIBUTE(6,public, TData, SamplingRate);
	DYN_ATTRIBUTE(7,public, TData, FrameSize);
	DYN_ATTRIBUTE(8,public, TData, SpectrumSize);

	void DefaultInit();
};

class SDIFOut: public Processing
{
public:

	SDIFOut(const SDIFOutConfig& c);
	SDIFOut();
	
	virtual ~SDIFOut();
	const char * GetClassName() const {return "SDIFOut";}
	
	bool GetEnableResidual()        {return mConfig.GetEnableResidual();}
	bool GetEnablePeakArray()       {return mConfig.GetEnablePeakArray();}
	bool GetEnableFundFreq()        {return mConfig.GetEnableFundFreq();}
	
	bool Do(void)
	{
		CLAM_ASSERT(false,"Not implemented");
		return false;
	}
	bool Do(const Frame& frame);

	const ProcessingConfig &GetConfig() const;

	SDIF::File* mpFile;
	
protected:

	bool ConcreteStart();

	bool ConcreteStop();

private:
	
	bool ConcreteConfigure(const ProcessingConfig& c);
	
	SDIFOutConfig mConfig;

// member variables
	IndexArray mPrevIndexArray;
		
};



};//CLAM
#endif
