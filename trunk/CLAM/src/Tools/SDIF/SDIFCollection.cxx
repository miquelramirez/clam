#include "SDIFCollection.hxx"
#include "SDIFFrame.hxx"
#include "SDIFStream.hxx"
#include "SDIFMatrix.hxx"

namespace SDIF
{

	Collection::Collection(bool autoStreamAdding)
	:mAutoStreamAdding(autoStreamAdding)
	{
	}

	void Collection::Add(Frame* pFrame)
	{
		mFrameList.push_back(pFrame);
		
		if (mAutoStreamAdding) {
			Stream* pStream = FindStream(pFrame->mHeader.mStreamId);
			if (!pStream)
			{
				pStream = 
					new Stream(pFrame->mHeader.mType,pFrame->mHeader.mStreamId);
				Add(pStream);
			}
			CLAM_ASSERT(pFrame->mHeader.mType==pStream->StreamType(),
	"Trying to add a frame to a stream with the same ID but a different type");
			pStream->Add(pFrame);
		}
	}

	void Collection::Add(Stream* pStream)
	{
		mStreamList.push_back(pStream);
	}

	void Collection::ParseStreams(void)
	{
		typedef std::list<Frame*>::iterator iterator;

		iterator it = mFrameList.begin();
		iterator end = mFrameList.end();

		while (it!=end)
		{
			Frame* pFrame = *it;

			Stream* pStream = FindStream(pFrame->mHeader.mStreamId);
			if (!pStream) {
				pStream = 
					new Stream(pFrame->mHeader.mType,pFrame->mHeader.mStreamId);

				Add(pStream);
			}
			pStream->Add(pFrame);

			it++;
		}
	}

	Stream* Collection::FindStream(CLAM::TInt32 streamId)
	{
		typedef std::list<Stream*>::iterator iterator;

		iterator it = mStreamList.begin();
		iterator end = mStreamList.end();

		while (it!=end)
		{
			Stream* pStream = *it;
			if (pStream->StreamId()==streamId) return pStream;
			
			it++;
		}

		return 0;
	}

	Stream* Collection::FindStream(TypeId streamTypeId)
	{
		typedef std::list<Stream*>::iterator iterator;

		iterator it = mStreamList.begin();
		iterator end = mStreamList.end();

		while (it!=end)
		{
			Stream* pStream = *it;
			if (pStream->StreamType()==streamTypeId) return pStream;
			
			it++;
		}

		return 0;
	}


}
