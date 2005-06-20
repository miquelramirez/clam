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
#include "QtPresentation.hxx"
#include "PlayablePlot.hxx"
#include "Slotv1.hxx"

using SigSlot::Slotv1;

namespace CLAM
{
    namespace VM
    {
		class TimeSegmentLabelsGroup;
		class SingleLabel;

		/**
		 * Allows viewing CLAM::Audio data.
		 *
		 * @ingroup QTVM
		 */
	
		class QtAudioPlot : public QtPresentation, public PlayablePlot
		{
			Q_OBJECT

		public:
			QtAudioPlot(QWidget* parent=0, const char * name = 0, WFlags f = 0 );
			~QtAudioPlot();

			void SetData(const Audio& audio, bool to_controller=true);

			void SetMarks(std::vector<unsigned>& marks);
			std::vector<unsigned>& GetMarks();
			void SetMarksColor(Color c);

			std::vector<QString> GetSegmentationTags();

			void SetForegroundColor(Color c);
			void SetDialColor(Color c);
			void SetRegionColor(Color c);

			void RemovePlayPanel();

			void SetKeyPressed(QKeyEvent* e);
			void SetKeyReleased(QKeyEvent* e);

		signals:
			void regionTime(MediaTime);
			void regionTime(float, float);
			void currentPlayingTime(float);
			void stopPlaying(float);
	    
		public slots:
			void setSelectedXPos(double);
			void setCurrentPlayingTime(float);
			void receivedStopPlaying(float);
	   
        protected slots:
			void initialYRulerRange(double,double);
			void updateRegion(MediaTime);

		protected:
			void keyPressEvent(QKeyEvent* e);
			void keyReleaseEvent( QKeyEvent* e);
			void hideEvent(QHideEvent* e);
			void closeEvent(QCloseEvent* e);
			void SetPlotController();
			void Connect();
			void DisplayBackgroundBlack();
			void DisplayBackgroundWhite();
			void SetPData(const Audio& audio, bool setTime);

		private:
			QBoxLayout*             mPanel;
			TimeSegmentLabelsGroup* mLabelsGroup;
			SingleLabel*            mLeftAmpLab; 
			SingleLabel*            mRightAmpLab;
			MediaTime               mPlayBounds;
			bool                    mShowRightAmp;
			Slotv1<TData>           mSlotPlayingTimeReceived;
			Slotv1<TData>           mSlotStopPlayingReceived;

			// holes
			QFrame *lefthole,*righthole;
				
			void UpdateAmpLabels(MediaTime time);
			void InitAudioPlot();

			void PlayingTime(TData time);
			void StopPlaying(TData time);
		};
    }
}

#endif

