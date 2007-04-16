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

#ifndef __VMQT_PLAYABLE_FUNDAMENTAL_PLOT_H__
#define __VMQT_PLAYABLE_FUNDAMENTAL_PLOT_H__

#include "vmFundamentalPlot.hxx"

class QComboBox;

namespace CLAM
{
	namespace VM
	{
		class WPlayer;
		class FundFreqPlayer;

		class PlayableFundamentalPlot : public FundamentalPlot
		{
			Q_OBJECT
		public:
			PlayableFundamentalPlot(QWidget* parent=0);
			~PlayableFundamentalPlot();

			void SetData(const Segment& segment, bool update=false);

		public slots:
			void backgroundWhite();
			void backgroundBlack();

		private slots:
			void setColorSchema(int);
			void setPlayingFlags(int);

		private:
			QComboBox* mSchemaComboBox;
			QComboBox* mPlayModeComboBox;

			WPlayer*        mWPlayer;
			FundFreqPlayer* mFundPlayer;

			void InitPlayableFundamentalPlot();
		};
	}
}

#endif

