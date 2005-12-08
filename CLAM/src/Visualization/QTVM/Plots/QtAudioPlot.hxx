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

#ifndef __QTAUDIOPLOT__
#define __QTAUDIOPLOT__

#include "Audio.hxx"
#include "MediaTime.hxx"
#include "SingleDisplayPlot.hxx"
#include "PlayablePlot.hxx"
#include "Slotv1.hxx"

using SigSlot::Slotv1;

class QFrame;

namespace CLAM
{
    namespace VM
    {
		class TimeSegmentLabelsGroup;
		/**
		 * Allows viewing CLAM::Audio data.
		 *
		 * @ingroup QTVM
		 */
	
		class QtAudioPlot : public SingleDisplayPlot, public PlayablePlot
		{
			Q_OBJECT

		public:
			QtAudioPlot(QWidget* parent=0, const char * name = 0, WFlags f = 0 );
			~QtAudioPlot();

			void SetData(const Audio& audio, bool to_controller=true);
			void UpdateData(const Audio& audio);

			void SetForegroundColor(Color c);
			void SetRegionColor(Color c);

			void ShowPlayer();
			void HidePlayer();

		signals:
			void regionTime(MediaTime);
			void regionTime(float, float);
			void currentPlayingTime(float);
			void stopPlayingTime(float);
	    
		public slots:
			void setCurrentPlayingTime(float);
			void receivedStopPlaying(float);
	   
		protected:
			void hideEvent(QHideEvent* e);
			void closeEvent(QCloseEvent* e);
			void SetPlotController();
			void Connect();
			void DisplayBackgroundBlack();
			void DisplayBackgroundWhite();
			void SetPData(const Audio& audio, bool setTime);

		private slots:
			void updateRegion(MediaTime);

		private:
			TimeSegmentLabelsGroup* mLabelsGroup;
			MediaTime               mPlayBounds;
			Slotv1<TData>           mSlotPlayingTimeReceived;
			Slotv1<TData>           mSlotStopPlayingReceived;

			QFrame *lefthole, *righthole;

			void InitAudioPlot();

			void PlayingTime(TData time);
			void StopPlaying(TData time);
		};
    }
}

#endif

