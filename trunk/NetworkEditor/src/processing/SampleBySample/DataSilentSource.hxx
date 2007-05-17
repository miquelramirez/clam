#ifndef _SilentSource_
#define _SilentSource_

#include "Processing.hxx"
#include "SampleBySampleConfig.hxx"

namespace CLAM{

class SilentSource:public CLAM::Processing
{
public:
	SilentSource();	
	bool Do();
	const char* GetClassName() const {return "SilentSource";}
	bool ConcreteConfigure(const CLAM::ProcessingConfig &){return true;}
	inline const CLAM::ProcessingConfig &GetConfig() const { return mConfig;}
protected:
	SampleBySampleConfig mConfig;

	OutDataPort mOutput;
};

};



#endif