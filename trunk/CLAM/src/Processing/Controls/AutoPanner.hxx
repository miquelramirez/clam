
#ifndef _AutoPanner_hxx_
#define _AutoPanner_hxx_

#include "DataTypes.hxx"
#include "OutControl.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include <string>

namespace CLAM
{

class AutoPannerConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (AutoPannerConfig, 4, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, TData, Frequency);
	DYN_ATTRIBUTE (1, public, TData , SamplingRate);
	DYN_ATTRIBUTE (2, public, TData , Phase);
	DYN_ATTRIBUTE (3, public, int , FrameSize);
protected:
	void DefaultInit(void);
};

class AutoPanner : public Processing
{
private:
	
	TData mFreq;
	TData mSamplingRate;
	TData mPhase;
	TData mDeltaPhase;
	int mFrameSize;

	AutoPannerConfig mConfig;
	OutControl mLeft;
	OutControl mRight;

public:
	AutoPanner();
	AutoPanner( const AutoPannerConfig & );

	bool Do();
	virtual ~AutoPanner(){}
	const char * GetClassName() const {return "AutoPanner";}
	
	inline const ProcessingConfig &GetConfig() const { return mConfig;}
	bool ConcreteConfigure(const ProcessingConfig& c);

};

} //namespace CLAM

#endif
