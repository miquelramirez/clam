/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __ThreadedBuffer__
#define __ThreadedBuffer__

#include "Processing.hxx"
#include "AudioFileIn.hxx"

#include <pthread.h>
#include <stdio.h>
#include <errno.h>

#define TB_DEBUG0
#define TB_DEBUG1
#define TB_DEBUG2
#define TB_DEBUG3

namespace CLAM {

pthread_mutex_t fastmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct ThreadedBufferInConfig:public ProcessingConfig
{
	DYNAMIC_TYPE_USING_INTERFACE
	(ThreadedBufferInConfig, 1, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);

	void DefaultInit(void)
	{
	}
};

#define MAXCHANNELS 2

class ThreadedBufferIn:public Processing
{
public:
	AudioFileIn* mWrapped;
	Array<Audio> mBuffers;

	pthread_t mThread;
	
	int mReadIndex;
	int mWriteIndex;
	int mBufferSize;
	int mBufferMask;
	int mFragmentSize;
	int mCompareMask;

	ThreadedBufferInConfig mConfig;

	bool ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}

	const ProcessingConfig& GetConfig(void) const { return mConfig; }

	ThreadedBufferIn(AudioFileIn* in)
	:mBuffers(in->Channels()){	
		mBuffers.SetSize(in->Channels());
		mWrapped = in;
		mReadIndex = 0;
		mWriteIndex = 0;
		
		mBufferSize = 65536*4;
		mBufferMask = mBufferSize-1;
		
		for (int i=0;i<mBuffers.Size();i++)
		{
			mBuffers[i].SetSize(mBufferSize);
		}

		mFragmentSize = 8192;
		
		mCompareMask = mBufferMask-(mFragmentSize-1);
	}

	bool Do(void)
	{
		CLAM_ASSERT(false,"ThreadedBufferIn: Do(): Supervised mode not implemented");
		return true;		
	}

	bool Do(Array<Audio*>& inArray)
	{
		int i,j;
		int nInputs = inArray.Size();
		TData** ptr=new TData*[nInputs];
		TData** srcPtr=new TData*[nInputs];

		int n = inArray[0]->GetSize();
				
		for (i=0;i<nInputs;i++) 
		{
			CLAM_ASSERT(inArray[i]->GetSize()==n,
				"ThreadedBufferIn::Do(): not all Audio has the same size");
			ptr[i] = inArray[i]->GetBuffer().GetPtr();
			srcPtr[i] = mBuffers[i].GetBuffer().GetPtr()+mReadIndex;
		}
		
		if (mThread == 0)
		{
			int m = n;
			n = (mWriteIndex-mReadIndex+mBufferSize)&(mBufferMask);
			if (n>m)
			{
				n = m;
			}
		}
		
		for (j=0;j<nInputs;j++)
		{
			for (i=0;i<n;i++)
			{
					*ptr[j]++ = *srcPtr[j]++;
			}
		}
		mReadIndex = (mReadIndex+n)&mBufferMask;

		if (((mWriteIndex+mFragmentSize)&mCompareMask)!=(mReadIndex&mCompareMask))
		{
			pthread_mutex_lock(&fastmutex);
			pthread_cond_signal(&cond);
			pthread_mutex_unlock(&fastmutex);
		}

		delete[] ptr;
		delete[] srcPtr;

		return true;
	}


	bool Do(Audio& in)
	{
		Array<Audio*> inArray(1);
		inArray.SetSize(1);
		inArray[0] = &in;
		return Do(inArray);
	}
	
	bool Do(Audio& inL,Audio& inR)
	{
		Array<Audio*> inArray(2);
		inArray.SetSize(2);
		inArray[0] = &inL;
		inArray[1] = &inR;
		return Do(inArray);
	}

	void WrappedDo(void)
	{
		int nInputs = mBuffers.Size();
		int i;

		Audio* tmp=new Audio[nInputs];

		for (i=0;i<nInputs;i++)
		{
			tmp[i].GetBuffer().SetPtr(
				mBuffers[i].GetBuffer().GetPtr()+mWriteIndex,
				mFragmentSize);
		}

		if (mBuffers.Size()==1)
		{				
			mWrapped->Do(tmp[0]);
		}
		else
		{
			mWrapped->Do(tmp[0],tmp[1]);
		}

		mWriteIndex=(mWriteIndex+tmp[0].GetSize())&mBufferMask;
		delete[] tmp;
	}
	
	void Background(void)
	{
		while (!mWrapped->Done())
		{
			pthread_mutex_lock(&fastmutex);
			if (((mWriteIndex+mFragmentSize)&mCompareMask)==(mReadIndex&mCompareMask))
			{
				pthread_cond_wait(&cond,&fastmutex);
			}
			pthread_mutex_unlock(&fastmutex);

			/* Make tmp point to a chunk of mBuffer, with size mFragmentSize
			** I am not sure if this should be done with the Audio api.
			*/
			while (
				((mWriteIndex+mFragmentSize)&mCompareMask)!=(mReadIndex&mCompareMask)
			)
			{
				WrappedDo();
			}
			pthread_testcancel();
		}
		mThread = 0;
	}
	static void* SBackground(ThreadedBufferIn* ptr)
	{
		ptr->Background();
		return 0;
	}	
	
	void Start(void)
	{
		mWrapped->Start();
		typedef void *(*startfn) (void *);
		do
		{
			WrappedDo();
		} while (mWriteIndex+mFragmentSize!=mBufferSize);

		pthread_create(&mThread,NULL,(startfn) SBackground,this);
		struct sched_param sched_param;
		int policy;

		if (pthread_getschedparam(mThread, &policy, &sched_param) < 0) {
			printf("Scheduler getparam failed...\n");
		}
		sched_param.sched_priority = sched_get_priority_max(SCHED_RR)-2;
		if (!pthread_setschedparam(mThread, SCHED_RR, &sched_param)) {
			printf("Scheduler set to SCHED_RR with priority %i...\n", sched_param.sched_priority);
		}
	}
};

}

#endif // ifndef __ThreadedBuffer__

