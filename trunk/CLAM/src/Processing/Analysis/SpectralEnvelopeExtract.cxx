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
#include "SpectralEnvelopeExtract.hxx"
#include "ErrProcessingObj.hxx"

#define CLASS "SpectralEnvelopeExtract"

namespace CLAM {

	/* The  Configuration object has at least to have a name */

	void SpectralEnvelopeExtractConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		DefaultValues();
	}

	
	void SpectralEnvelopeExtractConfig::DefaultValues()
	{
		SetInterpolationType(EInterpolation::eSpline);
	}


	/* Processing  object Method  implementations */

	SpectralEnvelopeExtract::SpectralEnvelopeExtract()
	{
		Configure(SpectralEnvelopeExtractConfig());
	}

	SpectralEnvelopeExtract::SpectralEnvelopeExtract(const SpectralEnvelopeExtractConfig &c = SpectralEnvelopeExtractConfig())
	{
		Configure(c);
	}

	SpectralEnvelopeExtract::~SpectralEnvelopeExtract()
	{}


	/* Configure the Processing Object according to the Config object */

	bool SpectralEnvelopeExtract::ConcreteConfigure(const ProcessingConfig& c)
	{

		CopyAsConcreteConfig(mConfig, c);
		return true;
	}

	/* Setting Prototypes for faster processing */

	bool SpectralEnvelopeExtract::SetPrototypes(const SpectralPeakArray& input,Spectrum& output)
	{
		return true;
	}

	bool SpectralEnvelopeExtract::SetPrototypes()
	{
		return true;
	}
	
	bool SpectralEnvelopeExtract::UnsetPrototypes()
	{
		return true;
	}

	/* The supervised Do() function */

	bool  SpectralEnvelopeExtract::Do(void) 
	{
		throw(ErrProcessingObj(CLASS"::Do(): Supervised mode not implemented"),this);
		return false;
	}

	/* The  unsupervised Do() function */

	bool  SpectralEnvelopeExtract::Do(const SpectralPeakArray& input, Spectrum& output)
	{
		output.SetScale(input.GetScale());
		
		int i;
		TSize nPeaks=input.GetnPeaks();
				
		if (nPeaks<4) return false; //cannot extract an envelope out of only 3 peaks!

		CheckOutputType(output);

		DataArray& magBuffer=input.GetMagBuffer();
		DataArray& phaseBuffer=input.GetPhaseBuffer();
		DataArray& freqBuffer=input.GetFreqBuffer();

		Array<Point>& magPointArray=output.GetMagBPF().GetPointArray();
		Array<Point>& phasePointArray=output.GetPhaseBPF().GetPointArray();

		magPointArray.Resize(nPeaks+1);
		magPointArray.SetSize(nPeaks+1);
		phasePointArray.Resize(nPeaks+1);
		phasePointArray.SetSize(nPeaks+1);
		for(i=0;i<nPeaks;i++)
		{
			magPointArray[i+1].SetX(freqBuffer[i]);
			magPointArray[i+1].SetY(magBuffer[i]);

			phasePointArray[i+1].SetX(freqBuffer[i]);
			phasePointArray[i+1].SetY(phaseBuffer[i]);

		}
	
		if(input.GetScale()==EScale::eLog)
		{
		
			//we now set first point (maybe we should do the same as with last point?)
			magPointArray[0].SetX(0);
			magPointArray[0].SetY(magBuffer[0]-3);
			phasePointArray[0].SetX(0);
			phasePointArray[0].SetY(0);
			nPeaks++;
			
			/* we keep adding points to bpf until magnitude is insignificant 
			(note that we add points outside the spectral range) */
			TData spectralRange=output.GetSpectralRange();
			TData lastFreq=freqBuffer[nPeaks-2];
			TData freqGap=lastFreq-freqBuffer[nPeaks-3];
			TData currentFreq=lastFreq+freqGap;
			TData currentMag=magBuffer[nPeaks-2];

			while(currentMag>-200)
			{
				currentMag-=(currentFreq/lastFreq-1)*12;
				magPointArray.AddElem(Point(currentFreq,currentMag));
				phasePointArray.AddElem(Point(currentFreq,0));//check phase!!! this is not correct although may not be necessary
				currentFreq+=freqGap;
				nPeaks++;
				
			}

		}
		else
		{
			//we now set first point (maybe we should do the same as with last point?)
			magPointArray[0].SetX(0);
			magPointArray[0].SetY(magBuffer[0]*0.5);
			phasePointArray[0].SetX(0);
			phasePointArray[0].SetY(0);
			nPeaks++;
			
			/* we keep adding points to bpf until magnitude is insignificant 
			(note that we add points outside the spectral range) */
			TData spectralRange=output.GetSpectralRange();
			TData lastFreq=freqBuffer[nPeaks-2];
			TData freqGap=lastFreq-freqBuffer[nPeaks-3];
			TData currentFreq=lastFreq+freqGap;
			TData currentMag=magBuffer[nPeaks-2];
			while(currentMag<0.0000000001)
			{
				currentMag*=pow(0.06,(double)(currentFreq/lastFreq-1.0));
				magPointArray.AddElem(Point(currentFreq,currentMag));
				phasePointArray.AddElem(Point(currentFreq,0));//check phase!!! this is not correct although may not be necessary
				currentFreq+=freqGap;
				nPeaks++;
				
			}
		}
		
		output.SetSize(nPeaks);
		output.GetMagBPF().UpdateSplineTable();
		output.GetPhaseBPF().UpdateSplineTable();

		return true;
	}

	bool SpectralEnvelopeExtract::CheckOutputType(Spectrum& out) 
	{
		SpecTypeFlags tmpFlags;
		tmpFlags.bMagPhaseBPF=1;
		tmpFlags.bMagPhase=0;
		out.SetType(tmpFlags);

		out.SetMagBPF(BPF(mConfig.GetInterpolationType()));
		out.SetPhaseBPF(BPF(EInterpolation::eLinear));
		
		/* Clear the  output Array */

		
		return true;
	}
};//namespace CLAM
