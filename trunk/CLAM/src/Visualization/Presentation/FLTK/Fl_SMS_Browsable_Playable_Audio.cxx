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
#include <FL/Fl_Box.H>
#include "Fl_SMS_Browsable_Playable_Audio.hxx"
#include "Fl_X_Axis.hxx"
#include "Fl_Y_Axis.hxx"
#include "Fl_ZoomSlider.hxx"
#include "Fl_SMS_Gl_Single_Browsable_Display.hxx"
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <iostream>

#include "Audio.hxx"
#include "AudioPlayer.hxx"

using namespace CLAMVM;
using CLAM::AudioPlayer;
using CLAM::Audio;


Fl_SMS_Browsable_Playable_Audio::Fl_SMS_Browsable_Playable_Audio( int X, int Y, int W, int H, const char* label )
	: Fl_Group( X, Y, W, H, label ), mCancel( false ), mIsThisPlaying( false ), mDisplay( NULL ), mSelectedSampleTime( 0 ),
	  mTooltipFormat( "t=%.4f s, x(t)=%.2f" )
{
	mImposterBox = new Fl_Box( X, Y, W-50, H-50 );

	resizable( mImposterBox );

	mXAxis = new Fl_X_Axis( X, Y+H-50, W-50, 30 );
	mXAxis->align( FL_ALIGN_BOTTOM );
	mXAxis->scale( FL_AXIS_LIN );
	mXAxis->minimum( 0.0f );
	mXAxis->maximum( 1.0f );
	mXAxis->label_format( "%g" );
	mXAxis->label_step( 10 );
	mXAxis->label_size( 9 );
	mXAxis->axis_color( FL_BLACK );
	mXAxis->axis_align( FL_AXIS_BOTTOM|FL_AXIS_LINE);
		
	mYAxis = new Fl_Y_Axis( X+W-50, Y, 30, H-50 );
	mYAxis->align( FL_ALIGN_LEFT );
	mYAxis->scale( FL_AXIS_LIN );
	mYAxis->minimum( -1.0 );
	mYAxis->maximum( 1.0 );
	mYAxis->label_format( "%g" );
	mYAxis->label_step( 10 );
	mYAxis->label_size( 9 );
	mYAxis->axis_color( FL_BLACK );
	mYAxis->axis_align( FL_AXIS_RIGHT | FL_AXIS_LINE );

	mXSlider = new Fl_ZoomSlider( X, Y + H-20, W-50, 20, FL_HORIZONTAL );
	mYSlider = new Fl_ZoomSlider( X + W-20, Y, 20, H-50, FL_VERTICAL );

	
	mPlayButton = new Fl_Button ( X+W-40, Y+H-20, 20, 20, "@>" );
	mPlayButton->callback( play, this );
	mPlayButton->labeltype(FL_SYMBOL_LABEL);
	mPlayButton->tooltip( "Play sound" );

	mStopButton = new Fl_Button ( X+W-20, Y+H-20, 20, 20, "@square" );
	mStopButton->callback( stop, this );
	mStopButton->labeltype(FL_SYMBOL_LABEL);
	mStopButton->tooltip( "Stop playing" );

	// Signal and Slot connections
	mXSlider->SpanChanged.Connect( mXAxis->AdjustRange );
	mYSlider->SpanChanged.Connect( mYAxis->AdjustRange );
				
	end();
	mDrawMgr.SetDetailThreshold( 500 );

	mStopSlot.Wrap( this, &Fl_SMS_Browsable_Playable_Audio::Stop );
	mTooltipTracker.RenderTooltipText.Wrap( this, &Fl_SMS_Browsable_Playable_Audio::OnRefreshTooltip );
				
	end();
	mDrawMgr.SetDetailThreshold( 500 );

	mStopSlot.Wrap( this, &Fl_SMS_Browsable_Playable_Audio::Stop );
	mTooltipTracker.RenderTooltipText.Wrap( this, &Fl_SMS_Browsable_Playable_Audio::OnRefreshTooltip );
	HandleDisplaySelection.Wrap( this, &Fl_SMS_Browsable_Playable_Audio::OnDisplaySelectedXValue );
	SetSelectedXValue.Wrap( this, &Fl_SMS_Browsable_Playable_Audio::OnSetSelectedXValue );


	mWorldSpaceCoords.mLeft = -1.0;
	mWorldSpaceCoords.mRight = 1.0;
	mWorldSpaceCoords.mTop = 1.0;
	mWorldSpaceCoords.mBottom = -1.0;

}

void Fl_SMS_Browsable_Playable_Audio::OnRefreshTooltip( int x, int y, char* textBuffer, int maxLen )
{
	double wX = (((double)x / (double)mDisplay->w())*(fabs(mXAxis->maximum()-mXAxis->minimum()))) + mXAxis->minimum();
	double wY = mYAxis->maximum() - (((double)y / (double)mDisplay->h())*(fabs(mYAxis->maximum()-mYAxis->minimum())));

	snprintf( textBuffer, maxLen,  mTooltipFormat.c_str(),  wX, wY );
}

void Fl_SMS_Browsable_Playable_Audio::OnDisplaySelectedXValue( double value )
{
	// towards the outer world
	mSelectedSampleTime = (value / mAudioProperties.sampleRate ) + mAudioProperties.startTime;
	SelectedXValue.Emit( mSelectedSampleTime );
}

void Fl_SMS_Browsable_Playable_Audio::OnSetSelectedXValue( double value )
{
	// towards the display
	double sampleIndex = ( value * mAudioProperties.sampleRate );
	ChangeSelectedXValue.Emit( sampleIndex );
}


int Fl_SMS_Browsable_Playable_Audio::handle( int event )
{

	if ( event == FL_ENTER  )
	{

		if ( !mTooltipTracker.HandleMotion( Fl::event_x(), Fl::event_y() ) )
			mTooltipTracker.ForceText( "idle" );

		return 1;
	}
	else if (  event == FL_MOVE )
	{
		if ( !mTooltipTracker.HandleMotion( Fl::event_x(), Fl::event_y() ) )
			mTooltipTracker.ForceText( "idle" );

		return 1;

	}
	else if ( event == FL_SHOW )
	{
		CLAM_ASSERT( mDisplay == NULL, "Precondition violation" );
		mImposterBox->hide();
		begin();
		mDisplay = new Fl_SMS_Gl_Single_Browsable_Display( x(), y(), w()-50, h()-50 );
		mDisplay->SetRenderer( mDrawMgr );
		mDisplay->EnableDoubleBuffering();
		mDisplay->SetPainting();
		mDisplay->SetWorldSpace( mWorldSpaceCoords.mRight,
								mWorldSpaceCoords.mLeft,
								mWorldSpaceCoords.mTop, 
								mWorldSpaceCoords.mBottom );
		mDisplay->end();
		end();//add( mDisplay );
		resizable( mDisplay );
		mTooltipTracker.Track( mDisplay );
		mTooltipTracker.ForceText( "idle" );
		mXSlider->SpanChanged.Connect( mDisplay->AdjustXAxis );				
		mYSlider->SpanChanged.Connect( mDisplay->AdjustYAxis );
		mDisplay->SelectedXValue.Connect( HandleDisplaySelection );
		ChangeSelectedXValue.Connect( mDisplay->SetSelectedXValue );
		mXAxis->resize( x(), y()+h()-50,w()-50, 30);
		mYAxis->resize( x()+w()-50, y(), 30, h() - 50);
		mXSlider->resize( x(), y() +h() -20, w() -50, 20);
		mYSlider->resize( x()+w()-20, y(), 20, h() -50 );
		mPlayButton->resize( x()+w()-40, y()+h()-20,20,20 );
		mStopButton->resize( x()+w()-20, y()+h()-20,20,20 );
	}
	else if ( event == FL_HIDE )
	{
		if ( mDisplay )
		{
			remove( mDisplay );
			delete mDisplay;
			mDisplay = NULL;
		}
	}
	

	return Fl_Group::handle( event );

}

void Fl_SMS_Browsable_Playable_Audio::draw()
{
	for ( int i = 0; i < children(); i++ )
	{
		child(i)->draw();
	}
}

Fl_SMS_Browsable_Playable_Audio::~Fl_SMS_Browsable_Playable_Audio( )
{
}

void Fl_SMS_Browsable_Playable_Audio::play( Fl_Widget*, void* data)
{
	((Fl_SMS_Browsable_Playable_Audio*)data)->Play();
}

void Fl_SMS_Browsable_Playable_Audio::stop( Fl_Widget*, void* data)
{
	((Fl_SMS_Browsable_Playable_Audio*)data)->Stop();
}

void Fl_SMS_Browsable_Playable_Audio::Play(  )
{ 
	AudioPlayer::StopFromGUIThread(  );
	Audio* tempAudio = new Audio;
	tempAudio->SetSampleRate( mAudioProperties.sampleRate );
	tempAudio->SetBeginTime( mAudioProperties.startTime );
	tempAudio->SetBuffer( mDrawMgr.GetDataCached(  ) );

	mIsThisPlaying = true;
	mAudioPlayer = new AudioPlayer( tempAudio, mStopSlot, mSelectedSampleTime );
}

void Fl_SMS_Browsable_Playable_Audio::Stop(  )
{
	if( mIsThisPlaying )
	{
		AudioPlayer::StopFromGUIThread(  );
		mIsThisPlaying = false;
	}
}

void Fl_SMS_Browsable_Playable_Audio::OnNewAudio( const DataArray& array, TTime begin, TTime end, TData srate )
{
	
	mAudioProperties.startTime = begin;
	mAudioProperties.endTime = end;
	mAudioProperties.sampleRate = srate;
	mSelectedSampleTime = begin;
	mDrawMgr.CacheData( array, mAudioProperties );
	mWorldSpaceCoords.mRight = array.Size() - 2;
	mWorldSpaceCoords.mLeft = 0;
	mWorldSpaceCoords.mTop = 1.0;
	mWorldSpaceCoords.mBottom = -1.0;
	mXAxis->minimum( begin );
	mXAxis->maximum( end );
	if ( mDisplay )
		mDisplay->invalidate();
	mXSlider->Reset();
	mYSlider->Reset();
	redraw();
}
	
void Fl_SMS_Browsable_Playable_Audio::Show()
{
	show();
}

void Fl_SMS_Browsable_Playable_Audio::Hide()
{
	hide();
}

void Fl_SMS_Browsable_Playable_Audio::SetPaint() 
{ 
	mDisplay->SetPainting(); 
}
		
void Fl_SMS_Browsable_Playable_Audio::UnsetPaint() 
{ 
	mDisplay->UnsetPainting();
}









