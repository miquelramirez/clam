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

#ifndef __DXFULLDUPLEX__
#define __DXFULLDUPLEX__

#include <dsound.h>
#include <dxerr8.h>
#include "Err.hxx"
#include "DataTypes.hxx"
#include "Assert.hxx"

namespace CLAM
{

class ErrDXFullDuplex : public Err
{
public:
	const char* mTxt;
	HRESULT mHR;
	ErrDXFullDuplex(const char* txt,HRESULT hr)
		: Err(), mTxt(txt),mHR(hr)
	{
		const char* dxErrStr = DXGetErrorString8(hr);
		mMsg = (char*) malloc(strlen(txt)+strlen(dxErrStr)+1);
		strcpy(mMsg,txt);
		strcat(mMsg,dxErrStr);
	}
};

// Helper template functions for safe pointer releasing and deleting

template < typename PointerType >
bool SafeDelete( PointerType& ptr )
{
	if ( ptr ) // ptr is not null
		{
			delete ptr;
			ptr = NULL;
		}
	else
		return false;

	return true;
}

template < typename DXObjectType >
bool SafeRelease( DXObjectType& obj )
{
	if ( obj ) // Obj is not null
		{
			obj->Release();
			obj = NULL;
		}
	else
		return false;

	return true;
}

class DXFullDuplex
{
public:

	// Handle to a Window's window. This allows us to claim some
	// OS resources ( such as access to audio devices ).
	static HWND       shMainWnd;

	DXFullDuplex( TUInt32 irate, TByte ichannels, TSize latency, LPGUID pGUID );
	~DXFullDuplex();

	HRESULT        Start( void );
	HRESULT        Poll( void );
	HRESULT        Write( short* buf, TSize size );
	HRESULT        Read ( short* buf, TSize size );

private:
	
	/** DXSound buffers may get lost somehow. This method tries to find it
	 *  and bring it back to the fold...
     */
	HRESULT        RestoreBuffer( LPDIRECTSOUNDBUFFER pDSBuffer,
								  bool&               wasRestored );

	/** This method completes the WAVEFORMATEX struct that defines the
	 *  format of the samples to be written or read...
     */
	virtual void   SetWaveFormat( WAVEFORMATEX* pwfx );

	/**  Here we do two important things:
     *     + Obtain an instance of a DirectSound Interface
	 *     + Obtain an instance of a Capture     Interface
     */
	HRESULT    InitDSoundDevices( void );

	/**  Here we create the three DX Buffers that we need:
     *     + The Input Buffer
	 *     + The Output ( Primary ) Buffer
	 *     + The Output ( Secondary ) Buffer
     */

	HRESULT    CreateDXBuffers( void );

	void       CheckResult( const char* msg, HRESULT hr );

private:

	LPGUID                             mGUID;

	LPDIRECTSOUND                      mDS;          // The DirectSound Object pointer
	LPDIRECTSOUNDCAPTURE               mDSCapture;   // The DirectSoundCapture Object pointer
	LPDIRECTSOUNDBUFFER                mDSBPrimary;  // The Primary Sound Buffer
	LPDIRECTSOUNDBUFFER                mDSBOutput;   // The Output (Secondary) Sound Buffer
	LPDIRECTSOUNDCAPTUREBUFFER         mDSBCapture;  // The Input ( SoundCapture ) Device Buffer 
	LPDIRECTSOUNDNOTIFY                mDSNotify;    // The DirectSound Notify Object

	DWORD                              mOutputBufferSize;
	DWORD                              mCaptureBufferSize;
	DWORD                              mNextCaptureOffset;
    DWORD                              mNextOutputOffset;
    
	WAVEFORMATEX                       mCaptureWaveFormat;
	WAVEFORMATEX                       mOutputFormat;
	
	TUInt32                            mSampleRate;
	TByte                              mChannels;

	bool                               mWritingToPrimary;

public:
	DWORD                              mLatency;

};

}

#endif // DXFullDuplex.hxx

