#ifndef __PROCESSINGKERNEL__
#define __PROCESSINGKERNEL__

namespace CLAM
{

class Thread;

class ProcessingKernel
{

public:
	ProcessingKernel();

	virtual ~ProcessingKernel() {}


	//MRJ: For some reason with Visual Studio 6.0 ( cl version 12.00.8168 )
	//     is not able to resolve the correct pointer for a derived class
	//     when accessing to a BaseClass::PureVirtualMethod implemented in
	//     the derived. Possible culprits:
	//          + Most surely, since the functor constructor is a virtual one
	//            through a tricky template function, *IS* possible that during
	//            some compile-time conversion, the compiler discards qualifiers
	//            over the reference to the derived class, thus producing a 
	//            "real pointer" (vcall) which seems to be some kind of compiler
	//            generated member pointer to identify pure virtual calls. Strange,
	//            huh?  Nonethless, this works right for patched Micro$oft compilers and
	//            gcc...

	virtual void ProcessingCleanup(){};

	virtual void ProcessingLoop(){};

	void AttachToThread( Thread& att_thread );

protected:

	bool LoopCondition();

	virtual bool UserCondition();

	void Cancel();

private:
	bool  mShouldRun;

};

}

#endif // ProcessingKernel.hxx
