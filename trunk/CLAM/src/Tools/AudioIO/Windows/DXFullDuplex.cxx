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

#define STRICT
#include "CLAM_windows.h"
#undef GetClassName
#include <basetsd.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <dxerr8.h>
#include <dinput.h>
#include <cstdlib>
#include <iostream>
#include <algorithm>
using std::copy;
using std::cout;
using std::endl;
#include "DXFullDuplex.hxx"

using namespace CLAM;

HWND DXFullDuplex::shMainWnd = 0; // We initialize the handle

DXFullDuplex::DXFullDuplex( TUInt32 irate, TByte ichannels, TSize latency, LPGUID pGUID )
	: mSampleRate ( irate ), mChannels ( ichannels ), mLatency ( latency ), mGUID( pGUID ),
	  mDS( NULL ), mDSCapture( NULL ), mDSBPrimary( NULL ), mDSBOutput( NULL ), mDSBCapture( NULL ),
	  mOutputBufferSize( 0 ), mCaptureBufferSize( 0 ), mNextCaptureOffset( 0 ), mNextOutputOffset( 0 ),
	  mWritingToPrimary( false )
{
	HRESULT hr;

	hr = InitDSoundDevices();
	

	CheckResult( "At DXFullDuplex constructor - InitDSoundDevices", hr );

	hr = CreateDXBuffers();

	CheckResult( "At DXFullDuplex constructor - CreateDXBuffers", hr );

}

DXFullDuplex::~DXFullDuplex()
{

	// Releasing the buffers
	SafeRelease( mDSBPrimary );
	SafeRelease( mDSBOutput );
	SafeRelease( mDSBCapture );
	
	// Releasing the devices
	SafeRelease( mDSCapture );
	SafeRelease( mDS );
}

HRESULT DXFullDuplex::InitDSoundDevices( void )
{
	HRESULT hr;


	// Obtaining the DirectSound interface
	hr = DirectSoundCreate( mGUID, &mDS, NULL );
	CheckResult( "Failed while obtaining a DirectSound interface", hr );

	hr = mDS->SetCooperativeLevel( 	GetForegroundWindow(), DSSCL_WRITEPRIMARY );

	if ( hr == DS_OK)
	{
		mWritingToPrimary = true;
//		CLAM_WARNING( true, "Able to get the primary buffer for writing" );
	}
	else
	{
		hr = mDS->SetCooperativeLevel( shMainWnd, DSSCL_PRIORITY );
		CheckResult( "Failed while setting up the cooperative level", hr );
		mWritingToPrimary = false;
//		CLAM_WARNING( true, "Unable to get the primary buffer for writing" );


	}

	mDS->Compact(); // We compact on board memory

	// Obtaining the Capture Device interface
	
	hr = DirectSoundCaptureCreate( mGUID, &mDSCapture, NULL );
	CheckResult( "Failed while obtaining a DirectSoundCapture interface", hr );

	return S_OK;
}

HRESULT DXFullDuplex::CreateDXBuffers( void )
{
	HRESULT       hr;
	DSBUFFERDESC  buff_descriptor;
	DSCBUFFERDESC capt_buff_descriptor;
	DSBCAPS       buff_caps;        // Buffer Capabilities Descriptor
	

	// Let's build the primary buffer
	// We obtain the desired format
	SetWaveFormat( &mOutputFormat );
	
	memset(&buff_descriptor, 0, sizeof(DSBUFFERDESC));
	buff_descriptor.dwSize = sizeof(DSBUFFERDESC); 
	buff_descriptor.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_LOCHARDWARE ; 
	// Buffer size is determined by sound hardware. 
	buff_descriptor.dwBufferBytes = 0; 
	buff_descriptor.lpwfxFormat = NULL; // Must be NULL for primary buffers

	hr = mDS->CreateSoundBuffer( &buff_descriptor, &mDSBPrimary, NULL );
	CheckResult( "Creating the Primary buffer", hr );

	hr = mDSBPrimary->SetFormat( &mOutputFormat );
	CheckResult( "Setting up the Primary buffer format", hr );
	
	
	if ( mWritingToPrimary )
	{
		memset(&buff_caps, 0, sizeof(DSBUFFERDESC));
		buff_caps.dwSize = sizeof(DSBCAPS); 

//		CLAM_WARNING( true, "PrimaryBuffer configuration start" );
		// We need to find out which was the primary buffer size
		hr = mDSBPrimary->GetCaps( &buff_caps );
		CheckResult( "Getting the buffer capabilities", hr );

		mOutputBufferSize = mCaptureBufferSize = buff_caps.dwBufferBytes;

		mDSBOutput = mDSBPrimary; // We will try using the primary buffer
//		CLAM_WARNING( true, "PrimaryBuffer configuration end" );

	}
	else
	{
		// Let's see which is the largest contigous audio hardware
		// memory block
		DSCAPS  dsound_caps;

		memset( &dsound_caps, 0, sizeof(DSCAPS) );
		dsound_caps.dwSize = sizeof( DSCAPS );

		hr = mDS->GetCaps( &dsound_caps );

		mOutputBufferSize = mCaptureBufferSize = 32768;

		// We must create a secondary buffer
		
		memset( &buff_descriptor, 0, sizeof( DSBUFFERDESC ) );
		buff_descriptor.dwSize = sizeof( DSBUFFERDESC );
		buff_descriptor.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCHARDWARE;
		buff_descriptor.dwBufferBytes = mOutputBufferSize;
		buff_descriptor.guid3DAlgorithm = GUID_NULL;
		buff_descriptor.lpwfxFormat = &mOutputFormat;

		hr = mDS->CreateSoundBuffer( &buff_descriptor, &mDSBOutput, NULL );
		CheckResult( "Creating the Secondary buffer", hr );
	
	}

	// Now it is the moment for creating the capture buffer;

	SetWaveFormat( &mCaptureWaveFormat );

	memset( &capt_buff_descriptor, 0, sizeof( DSCBUFFERDESC ) );
	capt_buff_descriptor.dwSize = sizeof( DSCBUFFERDESC );
	capt_buff_descriptor.dwBufferBytes = mCaptureBufferSize; // 1sec
	capt_buff_descriptor.lpwfxFormat = &mCaptureWaveFormat;

	hr = mDSCapture->CreateCaptureBuffer( &capt_buff_descriptor, &mDSBCapture, NULL );
	CheckResult( "Creating the Capture (Input) buffer", hr );

	return S_OK;
}

void DXFullDuplex::CheckResult ( const char* msg, HRESULT hr )
{
	if (FAILED(hr))
	{
		throw ErrDXFullDuplex( msg, hr);
	}
}

HRESULT DXFullDuplex::Read( short* buf, TSize size)
{
	HRESULT hr;
	VOID*        pDSLockedBuffer1 = NULL;
	DWORD        dwDSLockedBufferSize1;
	VOID*        pDSLockedBuffer2 = NULL;
	DWORD        dwDSLockedBufferSize2;

	DWORD blocksize = size*mChannels*2; /* size is in samples, blocksize in bytes */
	DWORD tmpPos = (mNextCaptureOffset - mLatency + mCaptureBufferSize)%mCaptureBufferSize;
	/* MDB: directx voodoo: Poll positions the nextcaptureoffset at mLatency before the capture
	   current position. but we need to go even more back in time (mLatency), if not we get
	   xruns...
	*/


	hr = mDSBCapture->Lock( tmpPos, blocksize, 
						   &pDSLockedBuffer1, 
						   &dwDSLockedBufferSize1, 
						   &pDSLockedBuffer2, 
						   &dwDSLockedBufferSize2, 
						   0L );

	CheckResult("DirectSoundBuffer::Lock", hr );

	TSize midpoint = dwDSLockedBufferSize1 >> 1;
	TSize endpoint = dwDSLockedBufferSize2 >> 1;

	buf = copy( (short*)pDSLockedBuffer1, (short*)pDSLockedBuffer1 + midpoint, buf); 
	
	copy( (short*)pDSLockedBuffer2, (short*)pDSLockedBuffer2+endpoint, buf );

	mDSBCapture->Unlock(
		pDSLockedBuffer1, dwDSLockedBufferSize1, 
		pDSLockedBuffer2, dwDSLockedBufferSize2
	);

	mNextCaptureOffset += blocksize;
	mNextCaptureOffset %= mCaptureBufferSize;

	return S_OK;
}

HRESULT DXFullDuplex::Write(short* buf,TSize size)
{
	HRESULT hr;
	VOID*        pDSLockedBuffer1 = NULL;
	DWORD        dwDSLockedBufferSize1;
	VOID*        pDSLockedBuffer2 = NULL;
	DWORD        dwDSLockedBufferSize2;

	DWORD blocksize = size*mChannels*2; /* size is in samples, blocksize in bytes */
	bool were_restored;

	hr = RestoreBuffer( mDSBOutput, were_restored );
	CheckResult("Restoring Output Buffer",hr);

	hr = mDSBOutput->Lock( mNextOutputOffset, blocksize, 
						   &pDSLockedBuffer1, 
						   &dwDSLockedBufferSize1, 
						   &pDSLockedBuffer2, 
						   &dwDSLockedBufferSize2, 
						   0L );

	CheckResult("DirectSoundBuffer::Lock", hr );

	TSize midpoint = dwDSLockedBufferSize1 >> 1;
	TSize endpoint = dwDSLockedBufferSize2 >> 1;

	copy( buf, buf + midpoint, (short*)pDSLockedBuffer1 ); 
	
	copy( buf+midpoint, buf+midpoint+endpoint, (short*)pDSLockedBuffer2 );


	// Unlock the play buffer
	mDSBOutput->Unlock(
		pDSLockedBuffer1, dwDSLockedBufferSize1, 
		pDSLockedBuffer2, dwDSLockedBufferSize2
	);

	mNextOutputOffset += blocksize;
	mNextOutputOffset %= mOutputBufferSize;

	return S_OK;
}

// Output buffers format
void DXFullDuplex::SetWaveFormat( WAVEFORMATEX* pwfx )
{
	memset( pwfx, 0, sizeof( WAVEFORMATEX ) );

	pwfx->wFormatTag = WAVE_FORMAT_PCM;
	pwfx->nChannels = mChannels;
	pwfx->nSamplesPerSec = mSampleRate;
	pwfx->wBitsPerSample = 16;
	pwfx->nBlockAlign = pwfx->nChannels * ( pwfx->wBitsPerSample / 8 );
	pwfx->nAvgBytesPerSec = pwfx->nBlockAlign * pwfx->nSamplesPerSec;
}

HRESULT DXFullDuplex::Poll( void )
{
/* We poll on the input cursor: we check if we have latency data available.
*/
	DWORD dist;
	
	do
	{
		
		DWORD pos;
		mDSBCapture->GetCurrentPosition(&pos,0);
		dist = pos >= mNextCaptureOffset ? pos - mNextCaptureOffset : pos + mCaptureBufferSize- mNextCaptureOffset;

/* MDB: This might be an alternative solution using realtime priority settings for the
        threads ( see BaseAudioApplication::SAudioThread method )
*/		
		if ( dist < mLatency )
		{
			Sleep( 1 );
		}
		
	} while (dist<mLatency);

	return S_OK;		
}

HRESULT DXFullDuplex::Start( void )
{
	HRESULT hr;
	
	mNextCaptureOffset = 0;
	mNextOutputOffset = mLatency*3; 
	// MDB: More DirectX voodoo. Why do we need to position 
	// the output cursor mLatency*3 forward? With smaller values, we get xruns....

	// Restore lost buffers
	bool were_restored;

	hr = RestoreBuffer( mDSBOutput, were_restored );
	CheckResult("Restoring Output Buffer",hr);

	mDSBOutput->SetCurrentPosition(0);
	mDSBCapture->GetCurrentPosition(&mNextCaptureOffset,0);

	hr = mDSBCapture->Start( DSCBSTART_LOOPING );
	CheckResult("Start Capture",hr);

	hr = mDSBOutput->Play( 0, 0, DSBPLAY_LOOPING );
	CheckResult("Start Output",hr);

	return S_OK;
}

HRESULT DXFullDuplex::RestoreBuffer( LPDIRECTSOUNDBUFFER pDSBuffer, bool& pbRestored )
{
	HRESULT hr;

	pbRestored = false;

	if( !pDSBuffer )
		return S_FALSE;

	DWORD dwStatus;
	hr = pDSBuffer->GetStatus( &dwStatus );
	CheckResult("At DXFullDuplex::RestoreBuffer GetStatus", hr );

	if( dwStatus & DSBSTATUS_BUFFERLOST )
	{
	// Since the app could have just been activated, then
	// DirectSound may not be giving us control yet, so 
	// the restoring the buffer may fail.  
	// If it does, sleep until DirectSound gives us control.
		hr = pDSBuffer->Restore();
		while ( hr != DS_OK )
			{
				Sleep( 10 );
				hr = pDSBuffer->Restore();
			}

		
		pbRestored = true;

		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

