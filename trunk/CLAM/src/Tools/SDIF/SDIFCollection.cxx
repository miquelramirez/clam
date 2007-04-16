/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

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

