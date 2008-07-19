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

#ifndef __VMQT_AUDIO_STEREO_PLOT_H__
#define __VMQT_AUDIO_STEREO_PLOT_H__

#include "Audio.hxx"
#include "vmMultiDisplayPlot.hxx"

class QGridLayout;

namespace CLAM
{
	namespace VM
	{
		class Grid;
		class Locator;
		class DataArrayRenderer;

		class AudioStereoPlot : public MultiDisplayPlot
		{
			Q_OBJECT
		public:
			AudioStereoPlot(QWidget* parent=0);
			virtual ~AudioStereoPlot();

			virtual void SetData(const Audio& chn0, const Audio& chn1, bool update=false);

		signals:
			void selectedRegion(double,double);

		public slots:
			virtual void backgroundWhite();
			virtual void backgroundBlack();

			void updateLocator(double);
			void updateLocator(double,bool);

		protected:
			QGridLayout* mLayout;
			
			enum { MASTER=0, SLAVE=1 };

		private slots:
			void setMaxVScroll(int);

		private:
			Ruler*       mYRuler0;
			Ruler*       mYRuler1;
			ScrollGroup* mVScroll;

			void CreateDisplay();
			void InitAudioStereoPlot();
			std::pair<int,int> GetZoomSteps(TSize size);
			Grid * _grid0;
			Grid * _grid1;
			DataArrayRenderer * _channel0;
			DataArrayRenderer * _channel1;
		protected:
			Locator * _locator0;
			Locator * _locator1;
		};
	}
}

#endif

