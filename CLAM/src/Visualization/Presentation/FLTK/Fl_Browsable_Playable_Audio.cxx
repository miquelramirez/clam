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

#include "Fl_Browsable_Playable_Audio.hxx"
#include "Fl_X_Axis.hxx"
#include "Fl_Y_Axis.hxx"
#include "Fl_ZoomSlider.hxx"
#include "Fl_Gl_Single_Browsable_Display.hxx"
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>

#include "Audio.hxx"
#include "AudioPlayer.hxx"

using namespace CLAMVM;
using CLAM::AudioPlayer;
using CLAM::Audio;

Fl_Browsable_Playable_Audio::Fl_Browsable_Playable_Audio( int X, int Y, int W, int H, const char* label )
	: Fl_Window( X, Y, W, H, label ), AudioPresentation(  ), mCancel( false ), mIsThisPlaying( false )
{
	mXAxis = new Fl_X_Axis( 0, H-40, W-40, 20, "Time (sec)" );
	mXAxis->align( FL_ALIGN_BOTTOM );
	mXAxis->scale( FL_AXIS_LIN );
	mXAxis->minimum( 0.0f );
	mXAxis->maximum( 1.0f );
	mXAxis->label_format( "%g" );
	mXAxis->label_step( 10 );
	mXAxis->label_size( 9 );
	mXAxis->axis_color( FL_BLACK );
	mXAxis->axis_align( FL_AXIS_BOTTOM|FL_AXIS_LINE);
		
	mYAxis = new Fl_Y_Axis( W-50, 0, 30, H-40, "Amplitude" );
	mYAxis->align( FL_ALIGN_LEFT );
	mYAxis->scale( FL_AXIS_LIN );
	mYAxis->minimum( -1.0 );
	mYAxis->maximum( 1.0 );
	mYAxis->label_format( "%g" );
	mYAxis->label_step( 10 );
	mYAxis->label_size( 9 );
	mYAxis->axis_color( FL_BLACK );
	mYAxis->axis_align( FL_AXIS_RIGHT | FL_AXIS_LINE );

	mXSlider = new Fl_ZoomSlider( 0, H-20, W-40, 20, FL_HORIZONTAL );
	mYSlider = new Fl_ZoomSlider( W-20, 0, 20, H-40, FL_VERTICAL );

	mDisplay = new Fl_Gl_Single_Browsable_Display( 0, 0, W-50, H-40 );
	resizable( mDisplay );
	mDisplay->SetRenderer( mDrawMgr );
	mDisplay->EnableDoubleBuffering();
		
	mPlayButton = new Fl_Button ( W-40, H-20, 20, 20, "@>" );
	mPlayButton->callback( play, this );
	mPlayButton->labeltype(FL_SYMBOL_LABEL);

	mStopButton = new Fl_Button ( W-20, H-20, 20, 20, "@square" );
	mStopButton->callback( stop, this );
	mStopButton->labeltype(FL_SYMBOL_LABEL);

	// Signal and Slot connections
	mXSlider->SpanChanged.Connect( mXAxis->AdjustRange );
	mXSlider->SpanChanged.Connect( mDisplay->AdjustXAxis );				
	mYSlider->SpanChanged.Connect( mYAxis->AdjustRange );
	mYSlider->SpanChanged.Connect( mDisplay->AdjustYAxis );
				
	end();
	mDrawMgr.SetDetailThreshold( 500 );

	mStopSlot.Wrap( this, &Fl_Browsable_Playable_Audio::Stop );
	mMouseEvent.Wrap( this, &Fl_Browsable_Playable_Audio::OnMouseEvent );
	mRecievingNewFrame.Wrap( this, &Fl_Browsable_Playable_Audio::OnNewFrame );
	mNewFrame.Connect( mDisplay->mNewFrame );
	mDisplay->mMouseEvent.Connect( mMouseEvent );
}

Fl_Browsable_Playable_Audio::~Fl_Browsable_Playable_Audio( )
{
}

void Fl_Browsable_Playable_Audio::play( Fl_Widget*, void* data)
{
	((Fl_Browsable_Playable_Audio*)data)->Play();
}

void Fl_Browsable_Playable_Audio::stop( Fl_Widget*, void* data)
{
	((Fl_Browsable_Playable_Audio*)data)->Stop();
}

void Fl_Browsable_Playable_Audio::Play(  )
{ 
	AudioPlayer::StopFromGUIThread(  );
	Audio* tempAudio = new Audio;
	tempAudio->SetSampleRate( mDrawMgr.GetTimeInfo(  ).sampleRate );
	tempAudio->SetBeginTime( 0 );
	tempAudio->SetBuffer( mDrawMgr.GetDataCached(  ) );

	mIsThisPlaying = true;
	mAudioPlayer = new AudioPlayer( tempAudio, mStopSlot );
}

void Fl_Browsable_Playable_Audio::Stop(  )
{
	if( mIsThisPlaying )
	{
		AudioPlayer::StopFromGUIThread(  );
		mIsThisPlaying = false;
	}
}

void Fl_Browsable_Playable_Audio::OnNewAudio( const DataArray& array, TTime begin, TTime end, TData srate )
{
	tAudioTimeInfo timeNfo = { begin, begin+end, srate };
	mDrawMgr.CacheData( array, timeNfo );
	mDisplay->SetWorldSpace( array.Size() - 2, 0, 1.0, -1.0f );
	mXAxis->minimum( begin );
	mXAxis->maximum( begin+end );
	mDisplay->invalidate();
	redraw();
}
	
void Fl_Browsable_Playable_Audio::Show()
{
	show();
}

void Fl_Browsable_Playable_Audio::Hide()
{
	hide();
}

void Fl_Browsable_Playable_Audio::SetPaint() 
{ 
	mDisplay->SetPainting(); 
}
		
void Fl_Browsable_Playable_Audio::UnsetPaint() 
{ 
	mDisplay->UnsetPainting();
}

void Fl_Browsable_Playable_Audio::SetPos( CLAM::TData pos )
{ 
	mDisplay->SetPos(pos); 
}
		
void Fl_Browsable_Playable_Audio::OnMouseEvent( double place )
{
	mSendingMouseEvent.Emit( place );
}

void Fl_Browsable_Playable_Audio::OnNewFrame( CLAM::TData frameNum )
{
	mNewFrame.Emit( frameNum );
}
