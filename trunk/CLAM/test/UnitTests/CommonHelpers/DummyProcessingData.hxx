#ifndef _DummyProcessingData_hxx_
#define _DummyProcessingData_hxx_

#include "ProcessingData.hxx" // CLAM

namespace CLAMTest
{


class DummyProcessingData : public CLAM::ProcessingData
{
	int mState;
public:
	DYNAMIC_TYPE_USING_INTERFACE( DummyProcessingData, 0, CLAM::ProcessingData );
public:
	void DefaultInit() { mState = -1; }
	void SetState(int val) { mState = val; }
	int GetState() { return mState; }
};

	
} // namespace CLAMTest	
#endif

