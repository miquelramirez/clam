/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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
#include "SpectralPeakArrayAdder.hxx"
#include "BPF.hxx"
#include "Point.hxx"
#include "Spectrum.hxx"

namespace CLAM {


	SpectralPeakArrayAdder::SpectralPeakArrayAdder()
		: mIn1("Input 1",this),
		  mIn2("Input 2",this),
		  mOut("Output",this)
	{
		Configure(PeaksInterpConfig());
	}

	SpectralPeakArrayAdder::SpectralPeakArrayAdder(const PeaksInterpConfig &c)
		: mIn1("Input 1",this),
		  mIn2("Input 2",this),
		  mOut("Output",this)
	{
		Configure(c);
	}


	bool SpectralPeakArrayAdder::ConcreteConfigure(const ProcessingConfig&c)
	{
		CopyAsConcreteConfig(mConfig, c);

		return true;
	}

	// Unsupervised Do() function.
	bool SpectralPeakArrayAdder::Do(const SpectralPeakArray& in1, const SpectralPeakArray& in2, SpectralPeakArray& out)
	{
		CLAM_DEBUG_ASSERT(GetExecState() != Unconfigured &&
		                  GetExecState() != Ready,
		                  "SpectralPeakArrayAdder::Do(): Not in execution mode");

		CLAM_ASSERT((&out)!=(&in1) && (&out)!=(&in1), "SpectralPeakAdder cannot process inplace");
			
		if (GetExecState() == Disabled)
			return true;
		
		//we initialize output peak array
		out.SetnPeaks(0);
		
		int nPeaks1=in1.GetnPeaks();
		int nPeaks2=in2.GetnPeaks();

		if(nPeaks1==0)
		{
			out=in2;
			return true;
		}
		if(nPeaks2==0)
		{
			out=in1;
			return true;
		}

				
		/*we first multiply indices in second input by 1000 in order
		to avoid aliasing between indices. Note though that if this
		process is applied recursively indices may end up getting out
		of bounds*/
		IndexArray& in2Index = in2.GetIndexArray();
		int i;
		for (i=0; i<nPeaks2;i++) in2Index[i]*=1000;
				
		int nSelected1, nSelected2;
		nSelected1 = nSelected2 = 0;
		/** This loop may be a little more efficient by ussing the
		 * buffers directly. But it would surely become muc less readable
		 * and much longer. TODO?*/
		do
		{
			/* TODO?: if peaks have exactly the same frequency we could think
			 * on adding the magnitudes and adding a single peak. The problem
			 * would then be that indices would get mixed up. Appart from that,
			 * the synthesis process will work by adding their energy */
			if(in1.GetFreq(nSelected1)<in2.GetFreq(nSelected2))
			{
				out.AddSpectralPeak(in1.GetSpectralPeak(nSelected1));
				nSelected1++;
			}
			else
			{
				out.AddSpectralPeak(in2.GetSpectralPeak(nSelected2));
				nSelected2++;
			}
			
		}while(nPeaks1-nSelected1 >0 && nPeaks2-nSelected2>0);
		
		return true;
	}

	// Unsupervised Do() function.
	bool SpectralPeakArrayAdder::Do(const SpectralPeakArray& in1, const SpectralPeakArray& in2,const Spectrum& spectralShape, SpectralPeakArray& out)
	{
		CLAM_DEBUG_ASSERT(GetExecState() != Unconfigured &&
		                  GetExecState() != Ready,
		                  "SpectralPeakArrayAdder::Do(): Not in execution mode");

		if (GetExecState() == Disabled)
			return true;

		out.SetnPeaks(0);
		
		int nPeaks1=in1.GetnPeaks();
		int nPeaks2=in2.GetnPeaks();

		if(nPeaks1==0)
		{
			out=in2;
			return true;
		}
		if(nPeaks2==0)
		{
			out=in1;
			return true;
		}

			
		
		/*In order to avoid confusion with indices we multiply the ones in the 
		 * second peak array by a large number. Note that this has the problem that
		 * if the process is applied recursively indices will grow out of bounds.*/
		int i;
		for(i=0; i< nPeaks2)
		{
			in2Index[i] *= 1000;
		} 
		
		DataArray freq1 = in1.GetFreqBuffer();
		DataArray freq2 = in2.GetFreqBuffer();
		
		/* We can now add up every peak array in both peakarray1 and 2*/
		int nAdded1,nAdded2;
		nAdded1=nAdded2=0;
		while(nAddedPeaks1<=nPeaks1 && nAddedPeaks2<=nPeaks1)
		{
			if(freq1[nAdded1]<freq2[nAdded2])
			{
				out.AddSpectralPeak(in1.GetSpectralPeak(nAdded);
				nAdded1++;
			}
			else
			{
				out.AddSpectralPeak(in1.GetSpectralPeak(nAdded2);
				nAdded2++;
			}
		}
		
		return true;
	}

	bool SpectralPeakArrayAdder::Do(void)
	{
		return Do(mIn1.GetData(),mIn2.GetData(),mOut.GetData());
	}

	

}

