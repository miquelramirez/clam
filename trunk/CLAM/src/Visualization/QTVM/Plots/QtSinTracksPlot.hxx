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

#ifndef __QTSINTRACKSPLOT__
#define __QTSINTRACKSPLOT__

#include "Segment.hxx"
#include "SpectralPeakArray.hxx"
#include "MediaTime.hxx"
#include "QtPresentation.hxx"

namespace CLAM
{
    namespace VM
    {
		/**
	 *
	 * Allows viewing sinusoidal tracks.
	 *
	 * @ingroup QTVM
	 */

	class TimeSegmentLabelsGroup;

	class QtSinTracksPlot : public QtPresentation
	{
	    Q_OBJECT

	public:
	    QtSinTracksPlot(QWidget* parent=0, const char * name = 0, WFlags f = 0);
	    ~QtSinTracksPlot();

	    void SetData(const Segment& segment);
	    void SetData(	const Array< SpectralPeakArray >& peakMtx, 
								const TData& sr, const TData& dur	);

	    void SetMarks(std::vector<unsigned>& marks);
	    std::vector<unsigned>& GetMarks();
	    void SetMarksColor(Color c);
	    
	    std::vector<QString> GetSegmentationTags();

	    void SetDialColor(Color c);
	    void SetRegionColor(Color c);

        protected slots:
	    void updateRegion(MediaTime);

	protected:
	    void keyPressEvent(QKeyEvent* e);
	    void keyReleaseEvent( QKeyEvent* e);
	    void SetPlotController();				virtual void Connect();

	    void DisplayBackgroundBlack();
	    void DisplayBackgroundWhite();

	private:
	    TimeSegmentLabelsGroup* _labelsGroup;

	    void InitSinTracksPlot();
	};
    }
}

#endif



