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

#include "PACLAMCallbacks.hxx"
#include "CLAM_windows.h"
#undef GetClassName
#include <iostream>
using std::cout;
using std::endl;

#include "AudioIO.hxx"
#include "AudioDevice.hxx"
#include "AudioDeviceList.hxx"
#include <portaudio.h>
#include "PortAudioUtils.hxx"
#include "PAAudioInputStream.hxx"
#include "PAAudioOutputStream.hxx"
#include "PAAudioFullDuplexStream.hxx"

#include "DoubleBuffer.hxx"

namespace CLAM
{

	class PAAudioDevice : public AudioDevice
	{
		enum IOModalities
		{
			eFullDuplex=0,
			eHalfDuplexIn,
			eHalfDuplexOut,
			eNoneYet
		};

	public:

		PAAudioDevice( const std::string& str, PaDeviceIndex devID );
		~PAAudioDevice( );

		void Start() throw( ErrPortAudio );
		void Stop() throw ( ErrPortAudio );
		void Read( Audio& samples, const int channelID );
		void Write( const Audio& samples, const  int channelID );

		virtual void GetInfo( AudioDevice::TInfo& );

	protected:

		void SetupMonoOutputStream() throw( ErrPortAudio );
		void SetupStereoOutputStream() throw( ErrPortAudio );
		void SetupMultiOutputStream() throw( ErrPortAudio );
		void SetupMonoInputStream() throw( ErrPortAudio );
		void SetupStereoInputStream() throw( ErrPortAudio );
		void SetupMultiInputStream() throw( ErrPortAudio );
		void SetupStereoFullDuplexStream() throw( ErrPortAudio );

	private:

		unsigned                 mNChannelsWritten;
		bool                     mChannelsWritten[256];

		unsigned                 mNChannelsRead;
		bool                     mChannelsRead[256];

		unsigned                 mWriteBuffSize;
		unsigned                 mReadBuffSize;

		PaDeviceIndex               mDevID;

		DoubleBuffer             mInputIntBuffer;
		DoubleBuffer             mOutputIntBuffer;

		PAAudioInputStream*      mInStream;
		PAAudioOutputStream*     mOutStream;
		PAAudioFullDuplexStream* mFullDuplexStream;

		IOModalities             mIOModel ;

		bool                     mStarted;
	};


	class PAAudioDeviceList : public AudioDeviceList
	{
	private:
		static PAAudioDeviceList mDevices;

		PAAudioDeviceList();

		std::vector< PaDeviceIndex > mDevIDs;

	protected:

		void EnumerateAvailableDevices();

	public:

		virtual ~PAAudioDeviceList();

		inline std::string DefaultDevice()
		{
			return mAvailableDevices[0];
		}

		AudioDevice* Create( const std::string& name, const std::string& device );

	};

	PAAudioDevice::PAAudioDevice( const std::string& name, PaDeviceIndex devID )
		: AudioDevice(name), mDevID( devID ), 
		  mInStream( NULL ), mOutStream( NULL ), mIOModel( eNoneYet ),mStarted(false)
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

	PAAudioDevice::~PAAudioDevice( )
	{
		mInputIntBuffer.DeAllocate();
		mOutputIntBuffer.DeAllocate();

		if ( mInStream )
			delete mInStream;

		if ( mOutStream )
			delete mOutStream;

		if ( mFullDuplexStream )
			delete mFullDuplexStream;
	}

	void PAAudioDevice::Start() throw( Err )
	{
    if ( mStarted && (((mInputs.size() | mOutputs.size()) != mNChannels) || 
      (mInputIntBuffer.GetSize() != mInputs.size()*Latency()) || 
      (mOutputIntBuffer.GetSize() != mOutputs.size()*Latency()))) // evil workaround start
    {
		  switch ( mIOModel )
			  {
			  case eFullDuplex:
				  mFullDuplexStream->Stop();
          delete mFullDuplexStream;
				  break;
			  case eHalfDuplexIn:
				  mInStream->Stop();
          delete mInStream;
				  break;
			  case eHalfDuplexOut:
				  mOutStream->Stop();
          delete mOutStream;
				  break;
			  case eNoneYet:
				  CLAM_ASSERT( false, "You cannot stop what hasn't been started" );
				  break;
			  }
		  mStarted = false;     
    } // evil workaround end

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
					switch ( outChannels )
						{
						case 1:
							break;
						case 2:
							SetupStereoFullDuplexStream();
							break;
						default:
							break;
						};

					mFullDuplexStream->Start();
				}
			// Check for Half Duplex Ouput
			else if ( (inChannels == 0) && (outChannels > 0))
				{
					mIOModel = eHalfDuplexOut;
					mNChannels = outChannels;
					// depending on outChannels value
					switch ( outChannels )
						{
						case 1:
							SetupMonoOutputStream();
							break;
						case 2:
							SetupStereoOutputStream();
							break;
						default:
							SetupMultiOutputStream();
							break;
						};
					
					mOutStream->Start();
				}
			// Check for Half Duplex Input
			else if ( (outChannels == 0) && (inChannels > 0))
				{
					mIOModel = eHalfDuplexIn;
					// depending on the value of inChannels
					mNChannels = inChannels;
					switch( inChannels )
						{
						case 1:
							SetupMonoInputStream();
							break;
						case 2:
							SetupStereoInputStream();
							break;
						default:
							SetupMultiInputStream();
							break;
						};
					mInStream->Start();
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
		catch( Err& e )
		{
			ErrPortAudio new_e( "Error Starting PortAudio Device:\n" );
			new_e.Embed( e );
			throw ( new_e );
		}
	}

	void PAAudioDevice::SetupMonoOutputStream() throw ( ErrPortAudio )
	{
		PAAudioStreamConfig cfg;
		cfg.SetSampleRate( SampleRate() );
		cfg.SetChannelNumber( 1 );
		cfg.SetCallback( monoOutCallback );
		cfg.SetDeviceID( mDevID );
		mOutputIntBuffer.DeAllocate();
		mOutputIntBuffer.Allocate(  Latency() );
		cfg.SetOutputDblBuffer( &mOutputIntBuffer);
		
		mOutStream = new PAAudioOutputStream( cfg );
		
	}

	void PAAudioDevice::SetupStereoOutputStream() throw ( ErrPortAudio )
	{
		PAAudioStreamConfig cfg;
		cfg.SetSampleRate( SampleRate() );
		cfg.SetChannelNumber( 2 );
		cfg.SetCallback( stereoOutCallback );
		cfg.SetDeviceID( mDevID );
		mOutputIntBuffer.DeAllocate();
		mOutputIntBuffer.Allocate( 2 * Latency() );
		cfg.SetOutputDblBuffer( &mOutputIntBuffer);
		
		mOutStream = new PAAudioOutputStream( cfg );
		
	}

	void PAAudioDevice::SetupMultiOutputStream() throw( ErrPortAudio )
	{
		PAAudioStreamConfig cfg;
		cfg.SetSampleRate( SampleRate() );
		cfg.SetChannelNumber( mNChannels );
		cfg.SetCallback( multiOutCallback );
		cfg.SetDeviceID( mDevID );
		mOutputIntBuffer.DeAllocate();
		mOutputIntBuffer.Allocate( mNChannels * Latency() );
		cfg.SetOutputDblBuffer( &mOutputIntBuffer);
		
		mOutStream = new PAAudioOutputStream( cfg );

	}

	void PAAudioDevice::SetupMonoInputStream() throw ( ErrPortAudio )
	{
		PAAudioStreamConfig cfg;
		cfg.SetSampleRate( SampleRate() );
		cfg.SetChannelNumber( 1 );
		cfg.SetCallback( monoInCallback );
		cfg.SetDeviceID( mDevID );
		mInputIntBuffer.DeAllocate();
		mInputIntBuffer.Allocate( Latency() );
		cfg.SetInputDblBuffer( &mInputIntBuffer);
		
		mInStream = new PAAudioInputStream( cfg );

	}

	void PAAudioDevice::SetupStereoInputStream() throw ( ErrPortAudio )
	{
		PAAudioStreamConfig cfg;
		cfg.SetSampleRate( SampleRate() );
		cfg.SetChannelNumber( 2 );
		cfg.SetCallback( stereoInCallback );
		cfg.SetDeviceID( mDevID );
		mInputIntBuffer.DeAllocate();
		mInputIntBuffer.Allocate( 2*Latency() );
		cfg.SetInputDblBuffer( &mInputIntBuffer);
		
		mInStream = new PAAudioInputStream( cfg );

	}

	void PAAudioDevice::SetupMultiInputStream() throw ( ErrPortAudio )
	{
		PAAudioStreamConfig cfg;
		cfg.SetSampleRate( SampleRate() );
		cfg.SetChannelNumber( mInputs.size() );
		cfg.SetCallback( multiInCallback );
		cfg.SetDeviceID( mDevID );
		mInputIntBuffer.DeAllocate();
		mInputIntBuffer.Allocate( mInputs.size()*Latency() );
		cfg.SetInputDblBuffer( &mInputIntBuffer);
		
		mInStream = new PAAudioInputStream( cfg );

	}

	void PAAudioDevice::SetupStereoFullDuplexStream() throw ( ErrPortAudio )
	{
		PAAudioStreamConfig cfg;
		cfg.SetSampleRate( SampleRate() );
		cfg.SetChannelNumber( mNChannels );
		cfg.SetCallback( stereoFDCallback );
		cfg.SetDeviceID( mDevID );
		mInputIntBuffer.DeAllocate();
		mOutputIntBuffer.DeAllocate();
		mInputIntBuffer.Allocate( 2*Latency() );
		mOutputIntBuffer.Allocate( 2*Latency() );
		cfg.SetInputDblBuffer( &mInputIntBuffer);
		cfg.SetOutputDblBuffer( &mOutputIntBuffer );
		
		mFullDuplexStream = new PAAudioFullDuplexStream( cfg );

	}


	void PAAudioDevice::Stop() throw( ErrPortAudio )
	{
		switch ( mIOModel )
			{
			case eFullDuplex:
				mFullDuplexStream->Stop();
				break;
			case eHalfDuplexIn:
				mInStream->Stop();
				break;
			case eHalfDuplexOut:
				mOutStream->Stop();
				break;
			case eNoneYet:
				CLAM_ASSERT( false, "You cannot stop what hasn't been started" );
				break;
			}
		mStarted = false;
	}

	void PAAudioDevice::GetInfo( AudioDevice::TInfo& nfo )
	{
		AudioDevice::GetInfo( nfo );
	}

	void PAAudioDevice::Read( Audio& samples, const int channelID )
	{
		TData* dst = samples.GetBuffer().GetPtr();
		short* src = NULL;
		unsigned samples_to_read = (mInputIntBuffer.GetSize()/mInputs.size());

		// Some error checking - proof of concepts
		CLAM_ASSERT( !mChannelsRead[channelID], "Tried to read twice from the same channel in single time frame!" );
		CLAM_ASSERT( mInStream || mFullDuplexStream, "No Input stream over the device has been created yet!" );			 
		CLAM_ASSERT( samples_to_read == samples.GetSize(), "Inconsistent Audio size" );

		static TData inv_2_15 = 1 / TData(32767.);

		WaitForSingleObject( mInputIntBuffer.mBackBufferReady, INFINITE );

		src = mInputIntBuffer.mBackBuffer + channelID;

		while ( samples_to_read-- )
			{
				*dst++ = TData(*src)*inv_2_15;
				src+=mNChannels;
			}
		mChannelsRead[channelID] = true;
		mNChannelsRead++;

		if (mNChannelsRead==mInputs.size())
		{
			mNChannelsRead = 0;
//			for (int i=0;i<mInputs.size();i++)
	//			mChannelsRead[i] = false;
      std::fill(mChannelsRead, mChannelsRead + mInputs.size(), false);
			ResetEvent( mInputIntBuffer.mBackBufferReady );
		}


	}
	
	void PAAudioDevice::Write( const Audio& samples, const int channelID )
	{
		unsigned samples_to_write = (mOutputIntBuffer.GetSize()/mNChannels);

		CLAM_ASSERT( !mChannelsWritten[channelID], "Tried to read twice from the same channel in single time frame!" );
		CLAM_ASSERT( mOutStream || mFullDuplexStream, "No Input stream over the device has been created yet!" );			 
		CLAM_ASSERT( samples_to_write == samples.GetSize(), "Inconsistent Audio size" );

		short* dst = mOutputIntBuffer.mFrontBuffer + channelID;
		TData* src = samples.GetBuffer().GetPtr();

		while ( samples_to_write-- )
			{
				*dst = (short) ( 32767. * (*src++));
				dst+=mNChannels;
			}
		mChannelsWritten[channelID] = true;
		mNChannelsWritten++;

		if (mNChannelsWritten==mOutputs.size())
		{
		
			
			mNChannelsWritten = 0;
//			for (int i=0;i<mOutputs.size();i++)
//				mChannelsWritten[i] = false;
      std::fill(mChannelsWritten, mChannelsWritten + mOutputs.size(), false);

			mOutputIntBuffer.SwapBuffers();
		}


	}

	PAAudioDeviceList::PAAudioDeviceList()
		: AudioDeviceList( std::string("portaudio") )
	{
		Pa_Initialize();

		EnumerateAvailableDevices();

		AddMe();
	}

	PAAudioDeviceList::~PAAudioDeviceList()
	{
		Pa_Terminate();
	}

	void PAAudioDeviceList::EnumerateAvailableDevices()
	{
		int numDevs = Pa_GetDeviceCount();
		int k = 0;
		const PaDeviceInfo* devnfo = NULL;

		for ( k = 0; k < numDevs; k++ )
			{
				devnfo = Pa_GetDeviceInfo( k );
				mAvailableDevices.push_back( devnfo->name );
				mDevIDs.push_back( k );
			}
		
	}

	AudioDevice* PAAudioDeviceList::Create( const std::string& name, const std::string& device )
	{
		int i = 0;

		for ( i=0; i < mAvailableDevices.size(); i++ )
			{
				if ( device == mAvailableDevices[i] )
					return new PAAudioDevice( name, mDevIDs[i] );
			}
		
		return 0;
	}

	PAAudioDeviceList PAAudioDeviceList::mDevices;
}

