#ifndef _DummyProcessingData_hxx_
#define _DummyProcessingData_hxx_

#include "ProcessingData.hxx"

namespace CLAMTest
{


class DummyProcessingData : public CLAM::ProcessingData
{
	int _state;
public:
	DYNAMIC_TYPE_USING_INTERFACE( DummyProcessingData, 0, CLAM::ProcessingData );
public:
	void DefaultInit() { _state = -1; }
	void SetState(int val) { _state = val; }
	int GetState() { return _state; }
};

	
} // namespace CLAMTest	
#endif
