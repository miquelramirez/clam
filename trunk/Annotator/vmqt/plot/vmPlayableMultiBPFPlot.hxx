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

#ifndef __VMQT_PLAYABLE_MULTIBPF_PLOT_H__
#define __VMQT_PLAYABLE_MULTIBPF_PLOT_H__

#include "vmMultiBPFPlot.hxx"

class QCheckBox;
class QComboBox;

namespace CLAM
{
	namespace VM
	{
		class WPlayer;
		class BPFPlayer;

		class PlayableMultiBPFPlot : public MultiBPFPlot
		{
			Q_OBJECT
		public:
			PlayableMultiBPFPlot(QWidget* parent=0);
			virtual ~PlayableMultiBPFPlot();

			void AddBPF(const QString& key, BPF* bpf);

			void SetXRange(double xmin, double xmax, ERulerScale scale=CLAM::VM::eLinearScale);
			void SetYRange(double ymin, double ymax, ERulerScale scale=CLAM::VM::eLinearScale);

		public slots:
			void backgroundWhite();
			void backgroundBlack();
			void readOnly();

		protected slots:
			virtual void setCurrentBPF(QString);

		private slots:		    
			void set_color_schema(int);
			void show_grid(int);
			void snap_to_grid(int);

		private:
			QCheckBox* mShowGrid;
			QCheckBox* mSnapGrid;
			QComboBox* mSchemaComboBox;
			QComboBox* mCurrentBPFComboBox;

			WPlayer*   mWPlayer;
			BPFPlayer* mBPFPlayer;

			void InitPlayableMultiBPFPlot();
		};
	}
}

#endif

