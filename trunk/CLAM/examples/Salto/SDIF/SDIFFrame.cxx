#include "SDIFFrame.hxx"
#include "SDIFMatrix.hxx"

SDIFFrame::SDIFFrame(const SDIFType& type, TFloat64 time,TInt32 id)
:mHeader(type,time,id)
{
	mpFirst = mpLast = NULL;
	mpPrev = mpNext = mpNextInStream = mpPrevInStream = NULL;
}

SDIFFrame::~SDIFFrame()
{
	SDIFMatrix* p = mpFirst;
	while (p)
	{
		SDIFMatrix* next = p->mpNext;
		
		delete p;
		
		p = next;
	}
	if (mpPrev) {
		mpPrev->mpNext = mpNext;
	}
	if (mpNext) {
		mpNext->mpPrev = mpPrev;
	}
	if (mpPrevInStream) {
		mpPrevInStream->mpNextInStream = mpNextInStream;
	}
	if (mpNextInStream) {
		mpNextInStream->mpPrevInStream = mpPrevInStream;
	}
}

void SDIFFrame::Add(SDIFMatrix* pMatrix)
{
	if (mpLast == NULL) {
		mpFirst = pMatrix;
	} else {
		mpLast->mpNext = pMatrix;
	}
	mpLast = pMatrix;
};

