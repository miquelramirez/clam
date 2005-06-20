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

#ifndef __QTSPECTRUMPLOT__
#define __QTSPECTRUMPLOT__
#include "Spectrum.hxx"
#include "QtPresentation.hxx"

namespace CLAM
{
    namespace VM
    {	

		class SingleLabel;

		/**
		 * Allows viewing a single spectrum.
		 *
		 * @ingroup QTVM
		 */

		class QtSpectrumPlot : public QtPresentation
		{

			Q_OBJECT
		public:
			QtSpectrumPlot(QWidget* parent=0, const char * name = 0, WFlags f = 0);
			virtual ~QtSpectrumPlot();

			virtual void SetData(const Spectrum& spec);

			void SetMarks(std::vector<unsigned>& marks);
			std::vector<unsigned>& GetMarks();
			void SetMarksColor(Color c);

			std::vector<QString> GetSegmentationTags();

			void SetForegroundColor(Color c);
			void SetVLineColor(Color c);

        protected slots:
            void updateMagLabel(TData);
			void updateFreqLabel(TData);

		protected:				
			void keyPressEvent(QKeyEvent* e);
			void keyReleaseEvent( QKeyEvent* e);
			virtual void SetPlotController();
			virtual void Connect();
			virtual void DisplayBackgroundBlack();
			virtual void DisplayBackgroundWhite();

		private:
			SingleLabel* mMagLabel;
			SingleLabel* mFreqLabel;
	    
			void InitSpectrumPlot();
		};
    }
}

#endif



