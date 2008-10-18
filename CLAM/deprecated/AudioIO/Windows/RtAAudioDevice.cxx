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

#include "AudioDevice.hxx"
#include "AudioDeviceList.hxx"
#include "RtAudio.h"
#include "RtAudioUtils.hxx"

#include "CLAM_windows.h"
#undef GetClassName
#include <iostream>

using std::cout;
using std::endl;

namespace CLAM
{

	class RtAAudioDevice : public AudioDevice
	{
		enum IOModalities
		{
			eFullDuplex=0,
			eHalfDuplexIn,
			eHalfDuplexOut,
			eNoneYet
		};

	public:

		RtAAudioDevice( const std::string& str, int devID );
		~RtAAudioDevice( );

		void Start() throw( ErrRtAudio );
		void Stop() throw ( ErrRtAudio );
		void Read( Audio& samples, const int channelID );
		void Write( const Audio& samples, const  int channelID );

		virtual void GetInfo( AudioDevice::TInfo& );

	protected:

		void SetupMonoOutputStream() throw( ErrRtAudio );
		void SetupStereoOutputStream() throw( ErrRtAudio );
		void SetupMultiOutputStream() throw( ErrRtAudio );
		void SetupMonoInputStream() throw( ErrRtAudio );
		void SetupStereoInputStream() throw( ErrRtAudio );
		void SetupMultiInputStream() throw( ErrRtAudio );
		void SetupStereoFullDuplexStream() throw( ErrRtAudio );

	private:

		unsigned                 mNChannelsWritten;
		bool                     mChannelsWritten[256];

		unsigned                 mNChannelsRead;
		bool                     mChannelsRead[256];

		int                      mFramesPerBuffer;

		int                      mDevID;

		short*                   mInputSamples;
		short*                   mOutputSamples;

		int                      mInputStreamId;
		int                      mOutputStreamId;

		IOModalities             mIOModel;
		RtAudio*                 mDevice;

		bool                     mStarted;
	};


	class RtAAudioDeviceList : public AudioDeviceList
	{
	private:
		static RtAAudioDeviceList sDevices;

		RtAAudioDeviceList();

		std::vector< int > mDevIDs;

	protected:

		void EnumerateAvailableDevices() throw ( ErrRtAudio );

	public:

		virtual ~RtAAudioDeviceList();

		inline std::string DefaultDevice()
		{
			return mAvailableDevices[0];
		}

		AudioDevice* Create( const std::string& name, const std::string& device );

	};
	
	RtAAudioDevice::RtAAudioDevice( const std::string& name, int devID )
		: AudioDevice(name), mDevID( devID ), 
		  mIOModel( eNoneYet ),mStarted(false), mDevice( NULL ), 
		  mInputSamples( NULL ), mOutputSamples( NULL ), mInputStreamId( -1 ), mOutputStreamId( -1)
	{

		// MRJ: Read/Written Channel Masks initialization
		int i;

		mNChannelsWritten = 0;
		for (i=0;i<256;i++)
		{
			mChannelsWritten[i] = false;
		}

		mNChannelsRead = 0;
		for (i=0;i<256;i++)
		{
			mChannelsRead[i] = false;
		}

	}

	RtAAudioDevice::~RtAAudioDevice( )
	{
		if ( mDevice )
			delete mDevice;
	}

	void RtAAudioDevice::Start() throw( ErrRtAudio )
	{
		if ( mStarted )
			return;

		unsigned outChannels = mOutputs.size();
		unsigned inChannels = mInputs.size();

		try
		{
			// Check for Full Duplex I/O
			if ( (outChannels > 0) && (inChannels > 0) && (outChannels==inChannels) )
				{
					// both outChannels and inChannels have the same value
					// and are above zero
					mIOModel = eFullDuplex;
					mNChannels = outChannels;
					mFramesPerBuffer = Latency();

					mDevice = new RtAudio();
					mOutputStreamId= mDevice->openStream( mDevID,mNChannels, 0, 0, 
										   RtAudio::RTAUDIO_SINT16, SampleRate(), &mFramesPerBuffer, NumberOfInternalBuffers() );
					mInputStreamId= mDevice->openStream( 0, 0, mDevID, mNChannels, RtAudio::RTAUDIO_SINT16,
											 SampleRate(), &mFramesPerBuffer, NumberOfInternalBuffers() );

					mOutputSamples = (short*) mDevice->getStreamBuffer( mOutputStreamId );
					mDevice->startStream( mOutputStreamId );
					mInputSamples = (short*) mDevice->getStreamBuffer( mInputStreamId );
					mDevice->startStream( mInputStreamId );

				}
			// Check for Half Duplex Ouput
			else if ( (inChannels == 0) && (outChannels > 0))
				{
					mIOModel = eHalfDuplexOut;
					mNChannels = outChannels;
					// depending on outChannels value
					mFramesPerBuffer = Latency();
					mDevice = new RtAudio();
					mOutputStreamId= mDevice->openStream( mDevID,mNChannels, 0, 0, 
										   RtAudio::RTAUDIO_SINT16, SampleRate(), &mFramesPerBuffer, NumberOfInternalBuffers() );
					mOutputSamples = (short*) mDevice->getStreamBuffer( mOutputStreamId );
					mDevice->startStream( mOutputStreamId );
								
				}
			// Check for Half Duplex Input
			else if ( (outChannels == 0) && (inChannels > 0))
				{
					mIOModel = eHalfDuplexIn;
					// depending on the value of inChannels
					mNChannels = inChannels;
					mFramesPerBuffer = Latency();
					mDevice = new RtAudio();
					mInputStreamId= mDevice->openStream( 0, 0, mDevID, mNChannels, RtAudio::RTAUDIO_SINT16,
											 SampleRate(), &mFramesPerBuffer, NumberOfInternalBuffers());
					mInputSamples = (short*) mDevice->getStreamBuffer( mInputStreamId );
					mDevice->startStream( mInputStreamId );
				
				}
			else
				{
					// Error. None of the supported I/O modalities.
					CLAM_ASSERT( false, "Check the number of AudioIn's and AudioOut's in your system.\n"
										"I have detected a input output mismatch( either you have none input and some outputs or\n"
										" some input or none output or just the same number of inputs and outputs)");
				}
			mStarted = true;
		}
		catch( RtError& e )
		{
			ErrRtAudio new_e( e );
			throw ( new_e );
		}
	}

	void RtAAudioDevice::Stop() throw( ErrRtAudio )
	{
		switch ( mIOModel )
			{
			case eFullDuplex:
				mDevice->stopStream( mOutputStreamId );
				mDevice->stopStream( mInputStreamId );
				mDevice->closeStream( mOutputStreamId );
				mDevice->closeStream( mInputStreamId );
				delete mDevice;
				mDevice=NULL;
				mInputSamples = NULL;
				mOutputSamples = NULL;
				mOutputStreamId = -1;
				mInputStreamId = -1;
				break;
			case eHalfDuplexIn:
				mDevice->stopStream( mInputStreamId );
				mDevice->closeStream( mInputStreamId );
				delete mDevice;
				mDevice=NULL;
				mInputSamples = NULL;
				mInputStreamId = -1;
				break;
			case eHalfDuplexOut:
				mDevice->stopStream( mOutputStreamId );
				mDevice->closeStream( mOutputStreamId );
				delete mDevice;
				mDevice=NULL;
				mOutputSamples = NULL;
				mOutputStreamId = -1;
				break;
			case eNoneYet:
				CLAM_ASSERT( false, "You cannot stop what hasn't been started" );
				break;
			}
		mStarted = false;
	}

	void RtAAudioDevice::GetInfo( AudioDevice::TInfo& nfo )
	{
		AudioDevice::GetInfo( nfo );
	}

	void RtAAudioDevice::Read( Audio& samples, const int channelID )
	{
		TData* dst = samples.GetBuffer().GetPtr();
		short* src = NULL;
		unsigned samples_to_read = mFramesPerBuffer;

		// Some error checking - proof of concepts
		CLAM_ASSERT( mInputStreamId!=-1, "No Input stream over the device has been created yet!" );			 
		CLAM_ASSERT( samples_to_read == samples.GetSize(), "Inconsistent Audio size" );

		static TData inv_2_15 = 1 / TData(32767.);

		src = mInputSamples + channelID;

		while ( samples_to_read-- )
			{
				*dst++ = TData(*src)*inv_2_15;
				src+=mNChannels;
			}


		mChannelsRead[channelID] = true;
		mNChannelsRead++;

		if (mNChannelsRead==mNChannels)
		{
			mNChannelsRead = 0;
			for (int i=0;i<mNChannels;i++)
				mChannelsRead[i] = false;
			mDevice->tickStream( mInputStreamId );
		}


	}
	
	void RtAAudioDevice::Write( const Audio& samples, const int channelID )
	{
		unsigned samples_to_write = mFramesPerBuffer;

		CLAM_ASSERT( mOutputStreamId!=-1, "No Output stream over the device has been created yet!" );			 
		CLAM_ASSERT( samples_to_write == samples.GetSize(), "Inconsistent Audio size" );

		short* dst = mOutputSamples + channelID;
		TData* src = samples.GetBuffer().GetPtr();

		while ( samples_to_write-- )
			{
				*dst = (short) ( 32767. * (*src++));
				dst+=mNChannels;
			}
		mChannelsWritten[channelID] = true;
		mNChannelsWritten++;

		if (mNChannelsWritten==mNChannels)
		{
		
			
			mNChannelsWritten = 0;
			for (int i=0;i<mNChannels;i++)
				mChannelsWritten[i] = false;

			mDevice->tickStream( mOutputStreamId );
		}


	}

	RtAAudioDeviceList::RtAAudioDeviceList()
		: AudioDeviceList( std::string("rtaudio") )
	{

		EnumerateAvailableDevices();

		AddMe();
	}

	RtAAudioDeviceList::~RtAAudioDeviceList()
	{
	}

	void RtAAudioDeviceList::EnumerateAvailableDevices() throw ( ErrRtAudio )
	{
		RtAudio* instance = NULL;
		
		try
			{
				instance = new RtAudio();
				
				int numDevs = instance->getDeviceCount();
				int k = 0;
				RtAudio::RTAUDIO_DEVICE devnfo;
				
				for ( k = 0; k < numDevs; k++ )
					{
						instance->getDeviceInfo( k, &devnfo );
						mAvailableDevices.push_back( devnfo.name );
						mDevIDs.push_back( k );
					}
				delete instance;
			}
		catch( RtError& err )
			{
				if ( instance )
					delete instance;
				ErrRtAudio new_err( err );
				throw ( new_err );
			}

	}

	AudioDevice* RtAAudioDeviceList::Create( const std::string& name, const std::string& device )
	{
		int i = 0;

		for ( i=0; i < mAvailableDevices.size(); i++ )
			{
				if ( device == mAvailableDevices[i] )
					return new RtAAudioDevice( name, mDevIDs[i] );
			}
		
		return 0;
	}

	RtAAudioDeviceList RtAAudioDeviceList::sDevices;

}

