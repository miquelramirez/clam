#include "SDIFStream.hxx"
#include "SDIFFrame.hxx"

SDIFStream::SDIFStream(SDIFFrame* pFrame)
{
	mpFirst = mpLast = mpCurrent = pFrame;
  mpNext = mpPrev = NULL;
}

void SDIFStream::Parse(void)
{
	SDIFFrame* pFrame = mpFirst;
	while (pFrame) {
		while (
			pFrame->mpNext && 
			pFrame->mpNext->mHeader.mStreamId!=mpLast->mHeader.mStreamId)
		{
			pFrame=pFrame->mpNext;		
		}
		pFrame = pFrame->mpNext;
		if (pFrame) {
			Add(pFrame);
		}
	}
}

void SDIFStream::Add(SDIFFrame* pFrame)
{
	mpLast->mpNextInStream = pFrame;
	pFrame->mpPrevInStream = mpLast;
	mpLast=pFrame;	
}

