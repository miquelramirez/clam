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

#ifndef __FL_BROWSABLE_PLAYABLE_AUDIO__
#define __FL_BROWSABLE_PLAYABLE_AUDIO__

#include <FL/Fl_Group.H>
#include "AudioRenderingManager.hxx"
#include "AudioPresentation.hxx"
#include "AudioPlayer.hxx"
#include "Signalv1.hxx"
#include "Slotv0.hxx"
#include "Slotv1.hxx"
#include "TooltipTracker2D.hxx"

class Fl_Button;
class Fl_Box;

namespace CLAMVM
{
	// forward declarations
	class Fl_X_Axis;
	class Fl_Y_Axis;
	class Fl_ZoomSlider;
	class Fl_SMS_Gl_Single_Browsable_Display;
	using SigSlot::Signalv1;
	using SigSlot::Slotv1;
	using SigSlot::Slotv0;

	class Fl_SMS_Browsable_Playable_Audio 
		: public Fl_Group, public AudioPresentation
	{
		Fl_X_Axis*                        mXAxis;
		Fl_Y_Axis*                        mYAxis;
		Fl_ZoomSlider*                    mXSlider;
		Fl_ZoomSlider*                    mYSlider;
		AudioRenderingManager             mDrawMgr;
		Fl_Button*                        mPlayButton;
		Fl_Button*                        mStopButton;
		bool                              mCancel;
		bool                              mIsThisPlaying;
		CLAM::AudioPlayer*                mAudioPlayer;
		// for transforming the sample index into sample time

		void Play(  );
		void Stop(  );

		void draw();

		static void play( Fl_Widget*, void* data);
		static void stop( Fl_Widget*, void* data);

		int handle( int event );

	protected:
		void OnNewAudio( const DataArray&, TTime, TTime, TData );
		Fl_SMS_Gl_Single_Browsable_Display*   mDisplay;
		CLAM::TData                       mAudioOffset;
		CLAM::TData                       mSampleRate;
		TooltipTracker2D                  mTooltipTracker;
		DataBoundBox                      mWorldSpaceCoords;
		Fl_Box*                           mImposterBox;
		
	public:
		Fl_SMS_Browsable_Playable_Audio( int X, int Y, int W, int H, const char* label = 0 );
		~Fl_SMS_Browsable_Playable_Audio();
		
		void OnRefreshTooltip( int x, int y, char* txtBuffer, int maxLen );
		void SetPaint(); 
		void UnsetPaint(); 
		void SetPos( CLAM::TData pos );



		Slotv0                mStopSlot;

		void Show();
		void Hide();

		Slotv1<double>        SetSelectedXValue;
		Signalv1<double>      SelectedXValue;


	protected:
		virtual void OnDisplaySelectedXValue( double value );
		virtual void OnSetSelectedXValue( double value );


		Signalv1< double > ChangeSelectedXValue;
		Slotv1< double >   HandleDisplaySelection;


		
	};

}


#endif // Fl_Audio.hxx
