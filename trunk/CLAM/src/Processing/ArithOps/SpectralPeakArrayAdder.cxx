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
#include "SpectralPeakArrayAdder.hxx"
#include "BPF.hxx"
#include "Point.hxx"
#include "Spectrum.hxx"

namespace CLAM {

	SpectralPeakArrayAdder::SpectralPeakArrayAdder(const Config &c)
		: mIn1("Input 1",this),
		  mIn2("Input 2",this),
		  mOut("Output",this)
	{
		Configure(c);
	}


	// Unsupervised Do() function.
	bool SpectralPeakArrayAdder::Do(const SpectralPeakArray& in1, const SpectralPeakArray& in2, SpectralPeakArray& out)
	{
		CLAM_DEBUG_ASSERT(IsRunning(),
			"SpectralPeakArrayAdder::Do(): Not in execution mode");

		CLAM_ASSERT((&out)!=(&in1) && (&out)!=(&in1), "SpectralPeakAdder cannot process inplace");
			
		//we initialize output peak array making sure index array is present
		out.AddIndexArray();
		out.UpdateData();
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

		IndexArray& in1Index = in1.GetIndexArray();
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
				out.AddSpectralPeak(in1.GetSpectralPeak(nSelected1), true, in1Index[nSelected1]);
				//std::cout<<"peak index"<<in1Index[nSelected1]<<std::endl;
				nSelected1++;
			}
			else
			{
				out.AddSpectralPeak(in2.GetSpectralPeak(nSelected2), true, in2Index[nSelected2]);
				nSelected2++;
			}
			
		}while(nPeaks1-nSelected1 >0 && nPeaks2-nSelected2>0);
		
		return true;
	}

};

