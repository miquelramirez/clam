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
			mGrid = new Grid;
			mPlot->AddRenderer("grid", mGrid);
			mPlot->SendToBack("grid");
			backgroundWhite();
		}

		MultiBPFPlot::~MultiBPFPlot()
		{
		}

		void MultiBPFPlot::AddBPF(const QString& key, BPF* bpf)
		{
			if(ExistKey(key))
			{
				mBpfEditors[mKeyIndex[key]]->SetData(bpf);
			}
			else
			{
				BPFEditor * bpfEditor = new BPFEditor;
				mKeyIndex[key]=mBpfEditors.size();
				mBpfEditors.push_back(bpfEditor);
				mPlot->AddRenderer(key, bpfEditor);
				mPlot->SendToBack("grid");
				mPlot->BringToFront("segmentation");
				mPlot->BringToFront("locator");
				mPlot->GetRenderer(key)->SetGrid(mGrid);
				bpfEditor->SetData(bpf);
				if(mIsReadOnly) bpfEditor->SetFlags(0);
				mBPFKeys[key]=bpfEditor->GetFlags();
				setCurrentBPF(key);
				connect(bpfEditor, SIGNAL(xValueChanged(QString,unsigned,double)),
					this, SIGNAL(xValueChanged(QString,unsigned,double)));
				connect(bpfEditor, SIGNAL(yValueChanged(QString,unsigned,double)),
					this, SIGNAL(yValueChanged(QString,unsigned,double)));
				connect(bpfEditor, SIGNAL(elementAdded(QString,unsigned,double,double)),
					this, SIGNAL(elementAdded(QString,unsigned,double,double)));
				connect(bpfEditor, SIGNAL(elementRemoved(QString,unsigned)),
					this, SIGNAL(elementRemoved(QString,unsigned)));
			}
		}

		void MultiBPFPlot::SetColors(const QString& key, const QColor& cline, const QColor& chandler)
		{
			if(!ExistKey(key)) return;
			mBpfEditors[mKeyIndex[key]]->SetColors(cline,chandler);
		}

		void MultiBPFPlot::SetFlags(const QString& key, int flags)
		{
			if(mIsReadOnly) return;
			if(!ExistKey(key)) return;
			mBpfEditors[mKeyIndex[key]]->SetFlags(flags);
			mBPFKeys[key]=flags;
		}
			
		void MultiBPFPlot::SetGridSteps(double xstep, double ystep)
		{
			mGrid->SetGridSteps(xstep,ystep);
//			mXRuler->SetStep(xstep);
//			mYRuler->SetStep(ystep);
		}

		void MultiBPFPlot::backgroundWhite()
		{
			SegmentationPlot::backgroundWhite();
			mGrid->SetGridColor(QColor(152,102,32));
		}

		void MultiBPFPlot::backgroundBlack()
		{
			SegmentationPlot::backgroundBlack();
			mGrid->SetGridColor(QColor(255,255,0));
		}

		void MultiBPFPlot::showGrid(bool show)
		{
			mGrid->ShowGrid(show);
		}

		void MultiBPFPlot::snapToGrid(bool snap)
		{
			mGrid->SnapToGrid(snap);
		}

		void MultiBPFPlot::readOnly()
		{
			for (unsigned i=0; i<mBpfEditors.size(); i++)
				mBpfEditors[i]->SetFlags(0);
			mIsReadOnly = true;
		}

		void MultiBPFPlot::setCurrentBPF(QString key)
		{
			if(key == mCurrentBPF) return;
			if(!ExistKey(key)) return;
			for (unsigned i=0; i<mBpfEditors.size(); i++)
				mBpfEditors[i]->SetFlags(0);
			mBpfEditors[mKeyIndex[key]]->SetFlags(mBPFKeys[key]);
			mCurrentBPF = key;
			emit currentBPF(mCurrentBPF);
		}

		bool MultiBPFPlot::ExistKey(const QString& key)
		{
			return (mBPFKeys.find(key) != mBPFKeys.end());
		}

	}
}

// END

