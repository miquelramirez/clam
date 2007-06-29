/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __VMQT_SPECGRAM_PANEL_H__
#define __VMQT_SPECGRAM_PANEL_H__

#include <vector>
#include <QtGui/QWidget>
#include "vmDataTypes.hxx"

class QLabel;

namespace CLAM
{
	namespace VM
	{
		class ColorScale;

		class SpecgramPanel : public QWidget
		{
			Q_OBJECT
		public:
			SpecgramPanel(QWidget* parent=0);
			~SpecgramPanel();

			void SetColorScale(const std::vector<QColor>& scale);
			
		signals:
			void colorScaleWidthChanged(int);

		public slots:
			void setFixedLabels(QString, QString);
			void updateLabels(QString, QString, QString, QString);
			
		private:
			QLabel* mFrequency;
			QLabel* mDecibels;
			QLabel* mSlice;
			QLabel* mSpectralRange;
			QLabel* mTotalSlices;
			QLabel* mTime;

			ColorScale* mColorScale;

			void InitSpecgramPanel();

			QWidget* GetLeftSide();
			QWidget* GetRightSide();
		};
	}
}

#endif

