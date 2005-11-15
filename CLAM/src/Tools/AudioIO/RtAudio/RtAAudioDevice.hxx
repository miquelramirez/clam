
#ifndef __RtAAudioDevice_hxx__
#define __RtAAudioDevice_hxx__

#include "RtAudio.hxx"
#include "AudioIO.hxx"
#include "AudioIn.hxx"
#include "AudioOut.hxx"
#include "AudioDeviceList.hxx"
#include "AudioDevice.hxx"
#include <cstdio>



#ifdef __MACOSX_CORE__
#define MACOSX_WORKAROUND
#endif

#ifdef __MACOSX_CORE__
/* a kludgy factor 2 (22050->44100) sample rate conversion
** because macosx drivers do not handle 22050 */ 
#define FACTOR2SRC_KLUDGE
#endif

namespace CLAM
{
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

		inline std::string DefaultDevice()
		{
			return "default";
		}
		static void init();

		AudioDevice* Create( const std::string& name, const std::string& device );

	};

} // namespace CLAM

#endif // __RtAAudioDevice_hxx__

