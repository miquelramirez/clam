#ifndef _SDIFOut_
#define _SDIFOut_

#include "IndexArray.hxx"
#include "Processing.hxx"
#include "Err.hxx"
#include "Segment.hxx"
#include <string.h>

namespace SDIF {class File;};//forward declaration

namespace CLAM{

class SDIFOutConfig:public ProcessingConfig
{
public:

	DYNAMIC_TYPE_USING_INTERFACE (SDIFOutConfig, 10, ProcessingConfig);
	DYN_ATTRIBUTE(0,public, std::string, Name);
	DYN_ATTRIBUTE(1,public, TData, SpectralRange);
	DYN_ATTRIBUTE(2,public, TIndex, MaxNumPeaks);
	DYN_ATTRIBUTE(3,public, bool,EnableResidual);
	DYN_ATTRIBUTE(4,public, bool,EnablePeakArray);
	DYN_ATTRIBUTE(5,public, bool,EnableFundFreq);
	DYN_ATTRIBUTE(6,public, std::string,FileName);
	DYN_ATTRIBUTE(7,public, TData, SamplingRate);
	DYN_ATTRIBUTE(8,public, TData, FrameSize);
	DYN_ATTRIBUTE(9,public, TData, SpectrumSize);

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
	
	bool Do(void){throw Err("Not implemented");}
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
