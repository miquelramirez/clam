#include "SDIFCollection.hxx"
#include "SDIFFrame.hxx"
#include "SDIFStream.hxx"
#include "SDIFMatrix.hxx"

#ifndef NULL
#define NULL (0L)
#endif

SDIFCollection::SDIFCollection(bool autoStreamAdding)
{
	mpFirst = mpLast = NULL;
	mpFirstStream = mpLastStream = NULL;
	mAutoStreamAdding = autoStreamAdding;
}
	
void SDIFCollection::Add(SDIFFrame* pFrame)
{
	if (mpLast == NULL) {
		mpFirst = pFrame;
	}else{
		mpLast->mpNext = pFrame;
		pFrame->mpPrev = mpLast;
	}
	if (mAutoStreamAdding) {
		SDIFStream* pStream = FindStream(pFrame->mHeader.mStreamId);
		if (pStream == NULL) {
			pStream = new SDIFStream(pFrame);
			Add(pStream);
		} else {
			pStream->Add(pFrame);
		}
	}
	mpLast = pFrame;
}

void SDIFCollection::Add(SDIFStream* pStream)
{
	if (mpLastStream == NULL) {
		mpFirstStream = pStream;
	} else {
		mpLastStream->mpNext = pStream;
		pStream->mpPrev = mpLastStream;
	}
	mpLastStream = pStream;	
}

void SDIFCollection::ParseStreams(void)
{
	SDIFFrame* pFrame = mpFirst;
	while (pFrame)
	{
		SDIFStream* pStream = FindStream(pFrame->mHeader.mStreamId);
		if (pStream == NULL) {
			pStream = new SDIFStream(pFrame);
			Add(pStream);
		} else {
			pStream->Add(pFrame);
		}
		pFrame = pFrame->mpNext;
	}
}

SDIFStream* SDIFCollection::FindStream(TInt32 streamId)
{
	SDIFStream* pStream = mpFirstStream;
	while (pStream && pStream->mpFirst->mHeader.mStreamId!=streamId)
	{
		pStream = pStream->mpNext;
	}
	
	return pStream;
}


