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

#ifndef __QTSTEREOAUDIOPLOT__
#define __QTSTEREOAUDIOPLOT__

#include <vector>
#include "Audio.hxx"
#include "MediaTime.hxx"
#include "QtPlot.hxx"
#include "PlayablePlot.hxx"

class QPushButton;

namespace CLAM
{
    namespace VM
    {
	class Ruler;
	class HScrollGroup;
	class QtAudioPlot;
	class TimeSegmentLabelsGroup;

	class QtStereoAudioPlot : public QtPlot, public PlayablePlot
	{
	    Q_OBJECT

	public:
	    QtStereoAudioPlot(QWidget* parent=0);
	    virtual ~QtStereoAudioPlot();

	    void SetData(std::vector<Audio> data);

	    void SetMarks(std::vector<unsigned>& marks);
	    std::vector<unsigned>& GetMarks();
	    void SetMarksColor(Color c);

	    void SetBackgroundColor(Color c);
	    void SetForegroundColor(Color c);
	    void SetDialColor(Color c);
	    void SetRegionColor(Color c);

	    void Label(const std::string& label);
	    void Geometry(int x,int y,int w,int h);
	    void Show();
	    void Hide();

	    void SwitchDisplayColors(bool b);

        public slots:
	    void hZoomIn();
	    void hZoomOut();
	    void hScrollValue(int);
	    void hMaxScroll(int);
	    void updateHScroll(int);
	    void receivedHZoomRatio(double);
				
	    void switchColors();

        protected slots:
	    void updateRegion(MediaTime);

	private slots:
	    void leftChannelClicked();
	    void rightChannelClicked();

	protected:
	    virtual void keyPressEvent(QKeyEvent* e);
	    virtual void keyReleaseEvent( QKeyEvent* e);

	    virtual void closeEvent(QCloseEvent* e);

	    void SetPData(std::vector<Audio> data);

	private:
	    QtAudioPlot *_leftChannel,*_rightChannel;
	    Ruler* _xRuler;
	    HScrollGroup* _hs;
	    QPushButton *_muteLeft,*_muteRight;
	    QPushButton* _btoggle_color;
	    TimeSegmentLabelsGroup *_labelsGroup;

	    double _maxHZRatio;
	    double _currentHZRatio;

	    void InitStereoAudioPlot();
	    void DirtyTrick();
	};
    }
}

#endif

