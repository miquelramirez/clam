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

#include "AudioGView.hxx"
using namespace CLAMGUI;

// Services declaration

Enum::tEnumValue ProcDataView<Audio>::Services::sEnumValues[] =
{
	{ ProcDataView<Audio>::Services::GetBuffer, "GetBuffer" },
	{ ProcDataView<Audio>::Services::GetSampleRate, "GetSampleRate" },
	{ ProcDataView<Audio>::Services::GetDuration, "GetDuration" },
	{ 0, NULL }
};

Enum::tValue ProcDataView<Audio>::Services::sDefault = ProcDataView<Audio>::Services::GetBuffer;

void ProcDataView<Audio>::Release()
{
	RenderFloatBuffer = NULL;
	RenderSamplingRate = NULL;
	RenderAudioDuration = NULL;
	mObservedObj = NULL;

	UnlockServices();
	UnlockModel();
}

void ProcDataView<Audio>::Refresh()
{
	if ( HasServicesLocked() && HasModelLocked() )
	{
		// Determine the Audio Object configuration
		if ( RenderAudioDuration != NULL )
		{
			TData duration = mObservedObj->GetSampleRate() * mObservedObj->GetBuffer().Size();
			( *RenderAudioDuration )( duration );
		}

		if ( RenderSamplingRate != NULL )
			(* RenderSamplingRate ) ( mObservedObj->GetSampleRate() );

		if ( RenderFloatBuffer!= NULL )
			( *RenderFloatBuffer )( mObservedObj->GetBuffer() );
	}
}

void ProcDataView< Audio >::Configure( Aspect& cfg )
{
	GenCb* cb_holder = NULL;

	UnlockServices();

	// Configuration code
	bool someone = false;
	// SamplingRate rendering service configuration
	cb_holder = cfg.GetRequest( Services("GetSampleRate") );

	someone |= Acknowledge( RenderSamplingRate, cb_holder );

	// Duration rendering service configuration
	cb_holder = cfg.GetRequest( Services("GetDuration") );

	someone |= Acknowledge( RenderAudioDuration, cb_holder );

	// Audio Buffer rendering service configuration ( float )
	cb_holder = cfg.GetRequest( Services("GetBuffer") );

	someone |= Acknowledge( RenderFloatBuffer, cb_holder );

	if ( someone ) 
	{
		LockServices();
	}
}
