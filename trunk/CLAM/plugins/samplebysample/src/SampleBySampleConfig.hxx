#ifndef SampleBySampleConfig_hxx
#define SampleBySampleConfig_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>

#include <CLAM/ProcessingConfig.hxx>

namespace CLAM{

typedef InPort<TData> InDataPort ;
typedef OutPort<TData> OutDataPort ;

class SampleBySampleConfig:public CLAM::ProcessingConfig
{
	DYNAMIC_TYPE_USING_INTERFACE( SampleBySampleConfig, 1, ProcessingConfig );
	DYN_ATTRIBUTE( 0, public, int, FrameSize);
protected:
	void DefaultInit()
	{
		AddAll();
		UpdateData();
		SetFrameSize(512);
	};

};

};
#endif

