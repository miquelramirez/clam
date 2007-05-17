#ifndef _DummyConfig_
#define _DummyConfig_

#include "ProcessingConfig.hxx"


namespace CLAM{

class DummyConfig : public CLAM::ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (DummyConfig, 1, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);
protected:
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
	}
};

};

#endif