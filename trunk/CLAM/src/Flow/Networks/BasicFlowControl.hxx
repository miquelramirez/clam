
#ifndef _BasicFlowControl_hxx_
#define _BasicFlowControl_hxx_

#include "FlowControl.hxx"

namespace CLAM
{

class Processing;

class BasicFlowControl : public FlowControl
{
public:
	BasicFlowControl( int frameSize = 0 );
	virtual ~BasicFlowControl(){}

	void DoProcessings();
};

} // namespace CLAM

#endif // _BasicFlowControl_hxx_
