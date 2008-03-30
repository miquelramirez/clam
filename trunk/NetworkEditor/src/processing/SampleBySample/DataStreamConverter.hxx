#ifndef DataStreamConverter_hxx
#define DataStreamConverter_hxx

#include "Processing.hxx"
#include "AudioOutPort.hxx"
#include "SampleBySampleConfig.hxx"

namespace CLAM{


class Data2Audio:public CLAM::Processing
{
public:
	Data2Audio();	
	bool Do();
	const char* GetClassName() const {return "Data2Audio";}
	bool ConcreteConfigure(const ProcessingConfig &);
	inline const CLAM::ProcessingConfig &GetConfig() const { return mConfig;}
protected:	
	SampleBySampleConfig mConfig;
	CLAM::AudioOutPort mOutput;
	InDataPort mInput;

	int mCounter;
	CLAM::DataArray mBuffer;
};

};

#endif

