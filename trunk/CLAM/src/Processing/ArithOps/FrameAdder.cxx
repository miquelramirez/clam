/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "Complex.hxx"
#include "SpecTypeFlags.hxx"
#include "FrameAdder.hxx"
#include "BPF.hxx"
#include "Point.hxx"

namespace CLAM {

	FrameAdder::FrameAdder()
		: mIn1("Input 1",this),
		  mIn2("Input 2",this),
		  mOut("Output",this)
	{
		AttachChildren();
		Configure(FrameAdderConfig());
	}

	FrameAdder::FrameAdder(const FrameAdderConfig &c)
		: mIn1("Input 1",this),
		  mIn2("Input 2",this),
		  mOut("Output",this)
	{
		AttachChildren();
		Configure(c);
	}


	bool FrameAdder::ConcreteConfigure(const ProcessingConfig&c)
	{
		CopyAsConcreteConfig(mConfig, c);

		return true;
	}

	void FrameAdder::AttachChildren()
	{
		mPO_SpectrumAdder.SetParent(this);
		mPO_PeaksAdder.SetParent(this);
	}


	// Unsupervised Do() function.
	bool FrameAdder::Do(const Frame& in1, const Frame& in2, Frame& out)
	{
		CLAM_DEBUG_ASSERT(IsRunning(),
			"FrameAdder::Do(): Not in execution mode");

		/** we set the output fundamental to zero. Note that when adding two frames
		 * we have a polyphonic signal and the representation of the resulting
		 * sum of harmonics is not trivial.*/
		if(out.GetFundamental().GetnCandidates()==0)
			out.GetFundamental().AddElem(0,0);
		else
			out.GetFundamental().SetFreq(0,0);
		out.GetFundamental().SetnCandidates(1);

		/** Now we add spectral peaks */
		//SpectralPeakArrayAdder cannot process inplace, by making this temporal
		//copy we allow FrameAdder to process inplace
		SpectralPeakArray tmpPeakArray;
		
		mPO_PeaksAdder.Do(in1.GetSpectralPeakArray(),in2.GetSpectralPeakArray(),tmpPeakArray);
		out.SetSpectralPeakArray(tmpPeakArray);
		/** Finally we add residual spectrum */
		mPO_SpectrumAdder.Do(in1.GetResidualSpec(),in2.GetResidualSpec(),out.GetResidualSpec());
		
		return true;
	}

	bool FrameAdder::Do(void)
	{
		CLAM_ASSERT(false,"FrameAdder::Do(): Not implemented");

		return true;
	}

	



}

