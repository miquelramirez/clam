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

namespace CLAM
{
	namespace VM
	{
		class TimeSegmentLabelsGroup;
		class SingleLabel;
	
		class QtAudioPlot : public QtPresentation, public PlayablePlot
		{
			Q_OBJECT

			public:

				QtAudioPlot(QWidget* parent=0);
				virtual ~QtAudioPlot();

				void SetData(const Audio& audio);

				void SetMarks(std::vector<unsigned>& marks);
		                std::vector<unsigned>& GetMarks();
				void SetMarksColor(Color c);

				void SetForegroundColor(Color c);
				void SetDialColor(Color c);
				void SetRegionColor(Color c);

				void RemovePlayPanel();

				void SetKeyPressed(QKeyEvent* e);
				void SetKeyReleased(QKeyEvent* e);

				void UpdateRegion(MediaTime);

			signals:
				void regionTime(MediaTime);

			protected slots:
				void updateRegion(MediaTime);

			protected:
				virtual void keyPressEvent(QKeyEvent* e);
				virtual void keyReleaseEvent( QKeyEvent* e);

				virtual void closeEvent(QCloseEvent* e);

				virtual void SetPlotController();
				virtual void Connect();

				virtual void DisplayBackgroundBlack();
				virtual void DisplayBackgroundWhite();

				void SetPData(const Audio& audio);

			private:
				QBoxLayout* _panel;
				TimeSegmentLabelsGroup* _labelsGroup;
				SingleLabel *_leftAmpLab, *_rightAmpLab;
				MediaTime _playBounds;
				bool showRightAmp;

				// holes
				QFrame *lefthole,*righthole;
				
				void UpdateAmpLabels(MediaTime time);
				void InitAudioPlot();
		};
	}
}

#endif

