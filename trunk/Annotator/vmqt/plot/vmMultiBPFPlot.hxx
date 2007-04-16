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

#ifndef __VMQT_MULTIBPF_PLOT_H__
#define __VMQT_MULTIBPF_PLOT_H__

#include <map>
#include "BPF.hxx"
#include "vmSegmentationPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class MultiBPFPlot : public SegmentationPlot
		{
			Q_OBJECT
		public:
			MultiBPFPlot(QWidget* parent=0);
			virtual ~MultiBPFPlot();

			virtual void AddBPF(const QString& key, BPF* bpf);
			virtual void SetColors(const QString& key, const Color& cline, const Color& chandler);
			virtual void SetFlags(const QString& key, int flags);
			
			void SetGridSteps(double xstep, double ystep);
		   
		signals:
			void xValueChanged(QString, unsigned, double);
			void yValueChanged(QString, unsigned, double);
			void elementAdded(QString, unsigned, double, double);
			void elementRemoved(QString, unsigned);
			void currentBPF(QString);

		public slots:
		    virtual void backgroundWhite();
			virtual void backgroundBlack();
			virtual void readOnly();
			virtual void setCurrentBPF(QString);

			void showGrid(bool);
			void snapToGrid(bool);
			
		protected:
			bool ExistKey(const QString& key);

		private:
			bool    mIsReadOnly;
			QString mCurrentBPF;

			std::map<QString,int> mBPFKeys;

			void InitMultiBPFPlot();
		};
	}
}

#endif

