#include "SDIFStream.hxx"
#include "SDIFFrame.hxx"

namespace SDIF
{

	Stream::Stream(TypeId typeId, CLAM::TInt32 streamId)
	:mType(typeId),mStreamId(streamId)
	{
	}

	void Stream::Add(Frame* pFrame)
	{
		mFrameList.push_back(pFrame);
	}

}
