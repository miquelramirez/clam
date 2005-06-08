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

#ifndef __QTFUNDFREQPLOT__
#define __QTFUNDFREQPLOT__

#include "Segment.hxx"
#include "MediaTime.hxx"
#include "QtPresentation.hxx"
#include "PlayablePlot.hxx"
#include "Slotv1.hxx"

using SigSlot::Slotv1;

class QFrame;

namespace CLAM
{
    namespace VM
    {
	/**
	 *
	 * Allows viewing Fundamental evolution along time.
	 *
	 * @ingroup QTVM
	 */

	class TimeSegmentLabelsGroup;
	class SingleLabel;
	
	class QtFundFreqPlot : public QtPresentation, public PlayablePlot
	{
	    Q_OBJECT

	public:
	    QtFundFreqPlot(QWidget* parent=0, const char * name = 0, WFlags f = 0 );
	    ~QtFundFreqPlot();

	    void SetData(const Segment& segment);

	    void SetMarks(std::vector<unsigned>& marks);
	    std::vector<unsigned>& GetMarks();
	    void SetMarksColor(Color c);

	    std::vector<QString> GetSegmentationTags();

	    void SetForegroundColor(Color c);
	    void SetDialColor(Color c);
	    void SetRegionColor(Color c);

	signals:
	    void regionTime(MediaTime);

	    void currentPlayingTime(float);
	    void stopPlaying(float);

	public slots:
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

	    void SetPData(const Segment& seg);

	private:
	    Slotv1<TData> mSlotPlayingTimeReceived;
	    Slotv1<TData> mSlotStopPlayingReceived;

	    TimeSegmentLabelsGroup* _labelsGroup;
	    SingleLabel *_leftFreqLab, *_rightFreqLab;

	    // holes
	    QFrame *lefthole, *righthole;
				
	    void InitFundFreqPlot();
	    void UpdateFreqLabels(MediaTime time);
	    
	    void PlayingTime(TData time);
	    void StopPlaying(TData time);

	};
    }
}

#endif

