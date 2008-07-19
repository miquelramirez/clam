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

#ifndef __VMQT_PLAYABLE_BPF_PLOT_H__
#define __VMQT_PLAYABLE_BPF_PLOT_H__

#include "vmPlayableMultiBPFPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class BPFEditor;

		class PlayableBPFPlot : public PlayableMultiBPFPlot
		{
			Q_OBJECT
		public:
			PlayableBPFPlot(QWidget* parent=0);
			~PlayableBPFPlot();

			void SetData(BPF* bpf);
			void SetFlags(int f);

		signals:
			void xValueChanged(unsigned, double);
			void yValueChanged(unsigned, double);
			void elementAdded(unsigned, double, double);
			void elementRemoved(unsigned);

		public slots:
			void backgroundWhite();
			void backgroundBlack();

		private slots:
			void xvalue_changed(QString, unsigned, double);
			void yvalue_changed(QString, unsigned, double);
			void element_added(QString, unsigned, double, double);
			void element_removed(QString, unsigned);

		private:
			void InitPlayableBPFPlot();

			BPFEditor * _bpfEditor;
		};
	}
}

#endif

