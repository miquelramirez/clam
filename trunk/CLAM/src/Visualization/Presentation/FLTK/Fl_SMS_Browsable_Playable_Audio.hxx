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
#include <string>
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

	/** MRJ: This class needs heavy refactoring: 
	*   + First, the AudioPlayer should be a ProcessingComposite with an InControl
	*     PlaybackStartTime ( for adjusting the initial time )
	*   + Second, the presentation should keep internally an Audio object for keeping
	*     track of what the Audio player must play instead of messing around with pointers
	*   + Third, use the Thread class for a cleaner solution for MT
	*/

	class Fl_SMS_Browsable_Playable_Audio 
		: public Fl_Group, public AudioPresentation
	{
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

		void SetTooltipFormat( const char* fmtStr );

	protected:
		virtual void OnDisplaySelectedXValue( double value );
		virtual void OnSetSelectedXValue( double value );
		Signalv1< double > ChangeSelectedXValue;
		Slotv1< double >   HandleDisplaySelection;

		void Play(  );
		void Stop(  );

		void draw();

		static void play( Fl_Widget*, void* data);
		static void stop( Fl_Widget*, void* data);

		int handle( int event );
		virtual void OnNewAudio( const DataArray&, TTime, TTime, TData );

	protected:
		Fl_SMS_Gl_Single_Browsable_Display*   mDisplay;
		TooltipTracker2D                      mTooltipTracker;
		DataBoundBox                          mWorldSpaceCoords;
		Fl_Box*                               mImposterBox;
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
		CLAM::TTime                       mSelectedSampleTime;
		tAudioTimeInfo                    mAudioProperties;
		// for transforming the sample index into sample time	
		std::string                       mTooltipFormat;
	};

	// inlines 
	inline void Fl_SMS_Browsable_Playable_Audio::SetTooltipFormat( const char* fmtStr )
	{
		mTooltipFormat = fmtStr;
	}

}


#endif // Fl_Audio.hxx
