#ifndef __PROCESSINGKERNEL__
#define __PROCESSINGKERNEL__

namespace CLAM
{

class Thread;

class ProcessingKernel
{

public:

	virtual void ProcessingCleanup() = 0;

	virtual void ProcessingLoop() = 0;

	void AttachToThread( Thread& att_thread );

protected:

	bool LoopCondition();

	virtual bool UserCondition();

};

}

#endif // ProcessingKernel.hxx
