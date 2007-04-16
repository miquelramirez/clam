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

#include "vmPlot2D.hxx"
#include "vmBPFEditor.hxx"
#include "vmPlayableBPFPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		PlayableBPFPlot::PlayableBPFPlot(QWidget* parent)
			: PlayableMultiBPFPlot(parent)
		{
			InitPlayableBPFPlot();
		}

		PlayableBPFPlot::~PlayableBPFPlot()
		{
		}

		void PlayableBPFPlot::SetData(BPF* bpf)
		{
			AddBPF("bpf_editor",bpf);
		}

		void PlayableBPFPlot::SetFlags(int f)
		{
			SetFlags("bpf_editor",f);
		}

		void PlayableBPFPlot::backgroundWhite()
		{
			PlayableMultiBPFPlot::backgroundWhite();
			SetColors("bpf_editor",Color(0,0,0),Color(0,0,255));
		}

		void PlayableBPFPlot::backgroundBlack()
		{
			PlayableMultiBPFPlot::backgroundBlack();
			SetColors("bpf_editor",Color(255,255,255),Color(255,0,0));
		}

		void PlayableBPFPlot::xvalue_changed(QString key, unsigned index, double value)
		{
			emit xValueChanged(index,value);
		}

		void PlayableBPFPlot::yvalue_changed(QString key, unsigned index, double value)
		{
			emit yValueChanged(index,value);
		}

		void PlayableBPFPlot::element_added(QString key, unsigned index, double xvalue, double yvalue)
		{
			emit elementAdded(index,xvalue,yvalue);
		}

		void PlayableBPFPlot::element_removed(QString key, unsigned index)
		{
			emit elementRemoved(index);
		}

		void PlayableBPFPlot::setCurrentBPF(QString key)
		{
			PlayableMultiBPFPlot::setCurrentBPF(key);
		}

		void PlayableBPFPlot::AddBPF(const QString& key, BPF* bpf)
		{
			PlayableMultiBPFPlot::AddBPF(key,bpf);
		}

		void PlayableBPFPlot::SetColors(const QString& key, const Color& cline, const Color& chandler)
		{
			PlayableMultiBPFPlot::SetColors(key,cline,chandler);
		}

		void PlayableBPFPlot::SetFlags(const QString& key, int flags)
		{
			PlayableMultiBPFPlot::SetFlags(key,flags);
		}

		void PlayableBPFPlot::InitPlayableBPFPlot()
		{
			AddBPF("bpf_editor",0);
			setCurrentBPF("bpf_editor");
			connect(static_cast<BPFEditor*>(mPlot->GetRenderer("bpf_editor")),
					SIGNAL(xValueChanged(QString,unsigned,double)),
					this,SLOT(xvalue_changed(QString,unsigned,double)));
			connect(static_cast<BPFEditor*>(mPlot->GetRenderer("bpf_editor")),
					SIGNAL(yValueChanged(QString,unsigned,double)),
					this,SLOT(yvalue_changed(QString,unsigned,double)));
			connect(static_cast<BPFEditor*>(mPlot->GetRenderer("bpf_editor")),
					SIGNAL(elementAdded(QString,unsigned,double,double)),
					this,SLOT(element_added(QString,unsigned,double,double)));
			connect(static_cast<BPFEditor*>(mPlot->GetRenderer("bpf_editor")),
					SIGNAL(elementRemoved(QString,unsigned)),this,SLOT(element_removed(QString,unsigned)));
		}
	}
}

// END

