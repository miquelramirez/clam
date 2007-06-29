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

#include "vmRuler.hxx"
#include "vmPlot2D.hxx"
#include "vmGrid.hxx"
#include "vmBPFEditor.hxx"
#include "vmMultiBPFPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		MultiBPFPlot::MultiBPFPlot(QWidget* parent)
			: SegmentationPlot(parent)
			, mIsReadOnly(false)
			, mCurrentBPF("")
		{
			InitMultiBPFPlot();
		}

		MultiBPFPlot::~MultiBPFPlot()
		{
		}

		void MultiBPFPlot::AddBPF(const QString& key, BPF* bpf)
		{
			if(ExistKey(key))
			{
				static_cast<BPFEditor*>(mPlot->GetRenderer(key))->SetData(bpf);
			}
			else
			{
				mPlot->AddRenderer(key, new BPFEditor());
				mPlot->SendToBack("grid");
				mPlot->BringToFront("segmentation");
				mPlot->BringToFront("locator");
				mPlot->GetRenderer(key)->SetGrid(static_cast<Grid*>(mPlot->GetRenderer("grid")));
				static_cast<BPFEditor*>(mPlot->GetRenderer(key))->SetData(bpf);
				if(mIsReadOnly) static_cast<BPFEditor*>(mPlot->GetRenderer(key))->SetFlags(0);
				mBPFKeys[key]=static_cast<BPFEditor*>(mPlot->GetRenderer(key))->GetFlags();
				setCurrentBPF(key);
				connect(static_cast<BPFEditor*>(mPlot->GetRenderer(key)),
						SIGNAL(xValueChanged(QString,unsigned,double)),
						this,SIGNAL(xValueChanged(QString,unsigned,double)));
				connect(static_cast<BPFEditor*>(mPlot->GetRenderer(key)),
						SIGNAL(yValueChanged(QString,unsigned,double)),
						this,SIGNAL(yValueChanged(QString,unsigned,double)));
				connect(static_cast<BPFEditor*>(mPlot->GetRenderer(key)),
						SIGNAL(elementAdded(QString,unsigned,double,double)),
						this,SIGNAL(elementAdded(QString,unsigned,double,double)));
				connect(static_cast<BPFEditor*>(mPlot->GetRenderer(key)),
						SIGNAL(elementRemoved(QString,unsigned)),this,SIGNAL(elementRemoved(QString,unsigned)));
			}
		}

		void MultiBPFPlot::SetColors(const QString& key, const QColor& cline, const QColor& chandler)
		{
			if(!ExistKey(key)) return;
			static_cast<BPFEditor*>(mPlot->GetRenderer(key))->SetColors(cline,chandler);
		}

		void MultiBPFPlot::SetFlags(const QString& key, int flags)
		{
			if(mIsReadOnly) return;
			if(!ExistKey(key)) return;
			static_cast<BPFEditor*>(mPlot->GetRenderer(key))->SetFlags(flags);
			mBPFKeys[key]=static_cast<BPFEditor*>(mPlot->GetRenderer(key))->GetFlags();
		}
			
		void MultiBPFPlot::SetGridSteps(double xstep, double ystep)
		{
			static_cast<Grid*>(mPlot->GetRenderer("grid"))->SetGridSteps(xstep,ystep);
			mXRuler->SetStep(xstep);
			mYRuler->SetStep(ystep);
		}

		void MultiBPFPlot::backgroundWhite()
		{
			SegmentationPlot::backgroundWhite();
			static_cast<Grid*>(mPlot->GetRenderer("grid"))->SetGridColor(QColor(152,102,32));
		}

		void MultiBPFPlot::backgroundBlack()
		{
			SegmentationPlot::backgroundBlack();
			static_cast<Grid*>(mPlot->GetRenderer("grid"))->SetGridColor(QColor(255,255,0));
		}

		void MultiBPFPlot::showGrid(bool show)
		{
			static_cast<Grid*>(mPlot->GetRenderer("grid"))->ShowGrid(show);
		}

		void MultiBPFPlot::snapToGrid(bool snap)
		{
			static_cast<Grid*>(mPlot->GetRenderer("grid"))->SnapToGrid(snap);
		}

		void MultiBPFPlot::readOnly()
		{
			std::map<QString,int>::iterator it = mBPFKeys.begin();
			for(; it != mBPFKeys.end(); it++)
			{
				static_cast<BPFEditor*>(mPlot->GetRenderer(it->first))->SetFlags(0);
			}
			mIsReadOnly = true;
		}

		void MultiBPFPlot::setCurrentBPF(QString key)
		{
			if(key == mCurrentBPF) return;
			if(!ExistKey(key)) return;
			std::map<QString,int>::iterator it = mBPFKeys.begin();
			for(; it != mBPFKeys.end(); it++)
			{
				static_cast<BPFEditor*>(mPlot->GetRenderer(it->first))->SetFlags(0);
			}
			static_cast<BPFEditor*>(mPlot->GetRenderer(key))->SetFlags(mBPFKeys[key]);
			mCurrentBPF = key;
			emit currentBPF(mCurrentBPF);
		}

		void MultiBPFPlot::InitMultiBPFPlot()
		{
			mPlot->AddRenderer("grid", new Grid());
			mPlot->SendToBack("grid");
			backgroundWhite();
		}

		bool MultiBPFPlot::ExistKey(const QString& key)
		{
			return (mBPFKeys.find(key) != mBPFKeys.end());
		}

	}
}

// END

