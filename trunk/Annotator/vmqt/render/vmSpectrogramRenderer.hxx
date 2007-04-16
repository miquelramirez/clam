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

#ifndef __VMQT_SPECTROGRAM_RENDERER_H__
#define __VMQT_SPECTROGRAM_RENDERER_H__

#include <vector>
#include "Spectrum.hxx"
#include "vmRenderer2D.hxx"

namespace CLAM
{
	namespace VM
	{
		class SpectrogramRenderer : public Renderer2D
		{
			Q_OBJECT
		public:
			SpectrogramRenderer();
			~SpectrogramRenderer();

			void SetData(const Array<Spectrum>& specMtx);

		signals:
			void fixedLabels(QString, QString);
			void labels(QString, QString, QString, QString);
			
		public slots:
			void colorSonogram();
			void blackWhiteSonogram();

		protected:
			void Render();
			void SetHBounds(double left, double right);
			void SetVBounds(double bottom, double top);
			void MouseMoveEvent(double x, double y);
			void LeaveEvent();

		private:
			int mglColorList;	
			int mglBlackWhiteList;
			int mColormap;

			std::vector<std::vector<float> > mComputedData;
			std::vector<std::vector<Color> > mColorData;
			std::vector<std::vector<Color> > mBlackWhiteData;

			GLView mLocalView;

			enum { COLOR_SONOGRAM=0, BW_SONOGRAM=1 };

			void DrawColorData();
			void DrawBlackWhiteData();
		};
	}
}

#endif

