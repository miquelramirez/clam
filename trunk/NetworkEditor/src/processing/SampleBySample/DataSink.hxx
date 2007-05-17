#ifndef DataSink_hxx
#define DataSink_hxx

#include "Processing.hxx"
#include "SampleBySampleConfig.hxx"

namespace CLAM{

class DataSink:public CLAM::Processing
{
public:
	DataSink();	
	bool Do();
	const char* GetClassName() const {return "DataSink";}
	bool ConcreteConfigure(const CLAM::ProcessingConfig &){return true;}
	inline const CLAM::ProcessingConfig &GetConfig() const { return mConfig;}
protected:
	SampleBySampleConfig mConfig;

	InDataPort mInput;
};

};

#endif

