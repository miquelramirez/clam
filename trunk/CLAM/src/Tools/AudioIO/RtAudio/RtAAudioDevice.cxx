/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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
#include "RtAAudioDevice.hxx"

/*
#include "RtAudio.hxx"
#include "AudioIO.hxx"
#include "AudioIn.hxx"
#include "AudioOut.hxx"
#include "AudioDeviceList.hxx"
#include "AudioDevice.hxx"
#include <cstdio>
*/

namespace CLAM {
/*
#ifdef __MACOSX_CORE__
#define MACOSX_WORKAROUND
#endif
*/

//#ifdef __MACOSX_CORE__
/* a kludgy factor 2 (22050->44100) sample rate conversion
** because macosx drivers do not handle 22050 */ 
	/*
#define FACTOR2SRC_KLUDGE
#endif
*/

//#define DEBUG_RDWR_POS


/*
	typedef signed short  MY_TYPE;
	#define FORMAT RtAudio::RTAUDIO_SINT16
	class RtAAudioDevice: public AudioDevice
	{
	private:
		class Buffer
		{
		friend class RtAAudioDevice;
		private:
			Buffer(int channels = 0,int frames = 0)
			{
				mData = 0;
				Alloc(channels,frames);
				mChannelsDone = 0;
				mReadIndex = 0;
				mWriteIndex = 0;
			}
			void Alloc(int channels = 0,int frames = 0)
			{
				if (mData) delete mData;
				mChannels = channels;
				mFrames = frames;
				if (mChannels && mFrames)
				{
					mData = new TData[mChannels*mFrames];
				}else{
					mData = 0;
				}
			}
			
			TData* mData;
			int    mReadIndex;
			int    mWriteIndex;
			int    mChannels;
			int    mChannelsDone;
			int    mFrames;
			int    Filled(void)
			{
				return mWriteIndex >= mReadIndex ? 
					mWriteIndex - mReadIndex : 
					mWriteIndex + mFrames - mReadIndex;
			}

			void CopyTo(MY_TYPE* ptr,int frames)
			{
				int cnt = frames*mChannels;
				int limit = mFrames*mChannels;
				int i = mReadIndex*mChannels;

#ifdef DEBUG_RDWR_POS				
				printf("copyto: r=%d %d\n",mReadIndex,frames);
#endif				
				while (cnt--)
				{
					*ptr++ = (MY_TYPE)(mData[i++]*32767.);
					if (i==limit) i = 0;
				}
				mReadIndex += frames;
				if (mReadIndex >= mFrames) mReadIndex -= mFrames;

			}

#ifdef FACTOR2SRC_KLUDGE
			void CopyToFactor2SRC(MY_TYPE* ptr,int frames)
			{
				int cnt = frames*mChannels;
				int limit = mFrames*mChannels;
				int i = mReadIndex*mChannels;

#ifdef DEBUG_RDWR_POS				
				printf("copyto: r=%d %d\n",mReadIndex,frames);
#endif				
				while (cnt--)
				{
					*ptr++ = (MY_TYPE)(mData[i]*32767.);
					*ptr++ = (MY_TYPE)(mData[i]*32767.);
					i++;
					if (i==limit) i = 0;
				}
				mReadIndex += frames;
				if (mReadIndex >= mFrames) mReadIndex -= mFrames;

			}
#endif

			void CopyFrom(MY_TYPE* ptr,int frames)
			{
				int cnt = frames*mChannels;
				int limit = mFrames*mChannels;
				int i = mWriteIndex*mChannels;
				
#ifdef DEBUG_RDWR_POS				
				printf("copyfrom: w=%d %d\n",mWriteIndex,frames);
#endif				
				while (cnt--)
				{
					mData[i++] = TData(*ptr++)/32767.;
					if (i==limit) i = 0;
				}
				mWriteIndex += frames;
				if (mWriteIndex >= mFrames) mWriteIndex -= mFrames;
			}

#ifdef FACTOR2SRC_KLUDGE
			void CopyFromDoFactor2SRC(MY_TYPE* ptr,int frames)
			{
				int cnt = frames*mChannels;
				int limit = mFrames*mChannels;
				int i = mWriteIndex*mChannels;
				
#ifdef DEBUG_RDWR_POS				
				printf("copyfrom: w=%d %d\n",mWriteIndex,frames);
#endif				
				while (cnt--)
				{
					TData t = TData(*ptr++);
					t += TData(*ptr++);
					t /= 65534.;
					mData[i++] = t;
					if (i==limit) i = 0;
				}
				mWriteIndex += frames;
				if (mWriteIndex >= mFrames) mWriteIndex -= mFrames;
			}
#endif				

			void ChannelCopyFrom(TData* ptr,int size,int chnId)
			{
				int i = mWriteIndex*mChannels+chnId;
				int n = size;
				int limit = mChannels*mFrames;

#ifdef DEBUG_RDWR_POS				
				printf("ChannelCopyFrom: w=%d %d\n",mWriteIndex,size);
#endif
				fflush(stdout);
				while (n--)
				{
					mData[i] = *ptr++;
					i += mChannels;
					if (i>=limit) i = chnId;
				}
				mChannelsDone++;
				if (mChannelsDone==mChannels)
				{
					mWriteIndex += size;
					if (mWriteIndex >= mFrames) mWriteIndex -= mFrames;
					mChannelsDone = 0;
				}
			}

			void ChannelCopyTo(TData* ptr,int size,int chnId)
			{
				int i = mReadIndex*mChannels+chnId;
				int n = size;
				int limit = mChannels*mFrames;
				
#ifdef DEBUG_RDWR_POS				
				printf("ChannelCopyTo: r=%d %d\n",mReadIndex,size);
#endif
				while (n--)
				{
					*ptr++ = mData[i];
					i += mChannels;
					if (i>=limit) i = chnId;
				}
				mChannelsDone++;
				if (mChannelsDone==mChannels)
				{
					mReadIndex += size;
					if (mReadIndex >= mFrames) mReadIndex -= mFrames;
					mChannelsDone = 0;
				}
			}
		};

		RtAudio *mRtAudio;
		int mRtAudioStream;
		MY_TYPE *mRtAudioBuffer;
		int mRtAudioBufferSize;
#ifdef MACOSX_WORKAROUND
		int mInternalRtAudioBufferSize;
#endif
		Buffer mWriteBuffer;
		Buffer mReadBuffer;
		bool mStarted;
		bool mTickOnRead;
		bool mTickOnWrite;
		int mDevice;
#ifdef FACTOR2SRC_KLUDGE
		bool mDoFactor2SRC;
#endif
	public:
		RtAAudioDevice(const std::string& name,int _device);
		~RtAAudioDevice();
		
		void Start(void) throw(Err);
		void Stop(void) throw(Err);
		void Tick(void);
		void Read(Audio& audio,const int channelID);
		void Write(const Audio& audio,const int channelID);
	};
	*/
	RtAAudioDevice::RtAAudioDevice(const std::string& name,int _device): 
		AudioDevice(name)
	{
		mStarted = false;
		mRtAudio = 0;
		mTickOnRead = false;
		mTickOnWrite = false;
		mDevice = _device;
	}

	void RtAAudioDevice::Start(void) throw(Err)
	{
		if (!mRtAudio)
		{
			int fs = SampleRate();
			mRtAudioBufferSize = Latency();

#ifdef __WINDOWS_DS__
			if (mRtAudioBufferSize<4096)
			{
				mRtAudioBufferSize = 4096;
			}
#endif

#ifdef FACTOR2SRC_KLUDGE
			mDoFactor2SRC = false;
			if (fs == 22050)
			{
				mDoFactor2SRC = true;
			}

			if (mDoFactor2SRC)
			{
				/* multiply the rt audio buffer size by two,
				** because while we will work on 22050 hz,
				** rtaudio will work at 44100, so it's internal
				** buffer will be double the size. when we access
				** the rt audio buffer in CopyFromDoFactor2SRC
				** and CopyToFactor2SRC, we indeed copy double 
				** mRtAudioBufferSize.
				** We multiply by two now, and device by two
				** later on to go back to our buffer size
				*/
				mRtAudioBufferSize *= 2;
				fs *= 2;
			}
#endif
			
#ifdef MACOSX_WORKAROUND
			mInternalRtAudioBufferSize = mRtAudioBufferSize;
			if (mInternalRtAudioBufferSize>2048)
			{
				mInternalRtAudioBufferSize = 2048;
			}
#endif

  		try {
    		mRtAudio = new RtAudio(&mRtAudioStream, 
					mDevice, mOutputs.size(),
					mDevice, mInputs.size(), 
					FORMAT, fs, 
#ifdef MACOSX_WORKAROUND
					&mInternalRtAudioBufferSize,
#else
					&mRtAudioBufferSize, 
#endif
				2);
  		}
  		catch (RtError &) {
    		exit(EXIT_FAILURE);
  		}

#ifdef MACOSX_WORKAROUND
			mRtAudioBufferSize = mInternalRtAudioBufferSize*((mRtAudioBufferSize+mInternalRtAudioBufferSize-1)/mInternalRtAudioBufferSize);
#endif

			
#ifdef FACTOR2SRC_KLUDGE
			if (mDoFactor2SRC)
			{
				/* see comment above */
				mRtAudioBufferSize /= 2;
#ifdef MACOSX_WORKAROUND
				mInternalRtAudioBufferSize /= 2;
#endif
			}
#endif
					
			/* update the latency value of the audiomanager */
			SetLatency(mRtAudioBufferSize);


			mWriteBuffer.Alloc(mOutputs.size(),mRtAudioBufferSize*2);
			mReadBuffer.Alloc(mInputs.size(),mRtAudioBufferSize*2);

	  		try {
  	  			mRtAudioBuffer = (MY_TYPE *) mRtAudio->getStreamBuffer(mRtAudioStream);
			}
  			catch (RtError &) {
    			exit(EXIT_FAILURE);
  			}
		}
		
		mStarted = false;
	}

	void RtAAudioDevice::Stop(void) throw(Err)
	{
		if (mRtAudio)
		{
		  mRtAudio->stopStream(mRtAudioStream);
			mStarted = false;
			delete mRtAudio;
			mRtAudio = 0;
		}
	}

	RtAAudioDevice::~RtAAudioDevice()
	{
		Stop();
	}

	void RtAAudioDevice::Read(Audio& a,const int channelID)
	{
		if (!mStarted)
		{
	    mRtAudio->startStream(mRtAudioStream);
			mStarted = true;
			mTickOnRead = true;
			
			/* TODO: explain this: */
			int k = mRtAudioBufferSize/a.GetSize();
			if (a.GetSize()*k==mRtAudioBufferSize)
			{
				mWriteBuffer.mWriteIndex = mRtAudioBufferSize;
			}
		}

		if (mTickOnRead && mReadBuffer.Filled()<a.GetSize())
		{
			Tick();
		}

		mReadBuffer.ChannelCopyTo(a.GetBuffer().GetPtr(),a.GetSize(),channelID);
	}
	
	void RtAAudioDevice::Write(const Audio& a,const int channelID)
	{
		if (!mStarted)
		{
	    mRtAudio->startStream(mRtAudioStream);
			mStarted = true;
			mTickOnWrite = true;
		}
		
		mWriteBuffer.ChannelCopyFrom(a.GetBuffer().GetPtr(),a.GetSize(),channelID);

		if (mTickOnWrite && mWriteBuffer.Filled()>=mRtAudioBufferSize)
		{
			Tick();
		}
	}
	
	void RtAAudioDevice::Tick(void)
	{
#ifdef MACOSX_WORKAROUND
		int i = mRtAudioBufferSize/mInternalRtAudioBufferSize;
		
#ifdef FACTOR2SRC_KLUDGE
		if (mDoFactor2SRC)
		{
			while (i--)
			{
				mWriteBuffer.CopyToFactor2SRC(mRtAudioBuffer,mInternalRtAudioBufferSize);
				mRtAudio->tickStream(mRtAudioStream);
				mReadBuffer.CopyFromDoFactor2SRC(mRtAudioBuffer,mInternalRtAudioBufferSize);
			}
		}else
#endif
		{
			while (i--)
			{
				mWriteBuffer.CopyTo(mRtAudioBuffer,mInternalRtAudioBufferSize);
				mRtAudio->tickStream(mRtAudioStream);
				mReadBuffer.CopyFrom(mRtAudioBuffer,mInternalRtAudioBufferSize);
			}
		}
#else
#ifdef FACTOR2SRC_KLUDGE
		if (mDoFactor2SRC)
		{
			mWriteBuffer.CopyToFactor2SRC(mRtAudioBuffer,mRtAudioBufferSize);
			mRtAudio->tickStream(mRtAudioStream);
			mReadBuffer.CopyFromDoFactor2SRC(mRtAudioBuffer,mRtAudioBufferSize);
		}
		else
#endif
		{
			mWriteBuffer.CopyTo(mRtAudioBuffer,mRtAudioBufferSize);
			mRtAudio->tickStream(mRtAudioStream);
			mReadBuffer.CopyFrom(mRtAudioBuffer,mRtAudioBufferSize);
		}
#endif
	}

	/*
	class RtAAudioDeviceList : public AudioDeviceList
	{
	private:
		static RtAAudioDeviceList sDevices;

		RtAAudioDeviceList();

		std::vector< int > mDevIDs;

	protected:

		void EnumerateAvailableDevices() throw ( Err );

	public:

		virtual ~RtAAudioDeviceList();
		static std::string foo()
		{
			return std::string("buabua");
		}

		inline std::string DefaultDevice()
		{
			return "default";
		}

		AudioDevice* Create( const std::string& name, const std::string& device );

	};
	*/
	RtAAudioDeviceList::RtAAudioDeviceList()
		: AudioDeviceList( std::string("rtaudio") )
	{

		EnumerateAvailableDevices();

		AddMe();
	}

	void RtAAudioDeviceList::init()
	{
	//	std::cout << "rtaudio initialized" << std::endl;
	}

	RtAAudioDeviceList::~RtAAudioDeviceList()
	{
	}

	void RtAAudioDeviceList::EnumerateAvailableDevices() throw ( Err )
	{
		RtAudio* instance = NULL;
		
		try {
				instance = new RtAudio();
				
				int numDevs = instance->getDeviceCount();
				int k;
				RtAudio::RTAUDIO_DEVICE devnfo; 

				mAvailableDevices.push_back("default");
				mDevIDs.push_back( 0 );
				
				for ( k = 1; k <= numDevs; k++ )
					{
						instance->getDeviceInfo( k, &devnfo );
						mAvailableDevices.push_back( devnfo.name );
						mDevIDs.push_back( k );
					}
				delete instance;
		} catch( ::RtError& err ) {
			if ( instance )
				delete instance;
			Err new_err("RtAAudioDeviceList::EnumerateAvailableDevices failed");
			new_err.Embed( err.getMessage() );
			throw ( new_err );
		}

	}

	AudioDevice* RtAAudioDeviceList::Create( const std::string& name, const std::string& device )
	{
		unsigned int i = 0;

		for ( i=0; i < mAvailableDevices.size(); i++ )
		{
			if ( device == mAvailableDevices[i] )
				return new RtAAudioDevice( name, mDevIDs[i] );
		}
		for ( i=0; i < mAvailableDevices.size(); i++ )
		{
			char tmp[16];
			sprintf(tmp,"%d",mDevIDs[i]);
			if ( device == tmp )
				return new RtAAudioDevice( name, mDevIDs[i] );
		}

		return 0;
	}

	RtAAudioDeviceList RtAAudioDeviceList::sDevices;
}


