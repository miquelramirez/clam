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

#include "Complex.hxx"
#include "SpectralEnvelopeExtract.hxx"
#include "Spectrum.hxx"
#include "SpecTypeFlags.hxx"
#include "SpectralPeakArray.hxx"

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
		SetMaxPeaks(200);
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
		mMagBPF.SetIntpType(mConfig.GetInterpolationType());
		mPhaseBPF.SetIntpType(EInterpolation::eLinear);
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
		CLAM_ASSERT(false,CLASS"::Do(): Supervised mode not implemented");
		return false;
	}

	/* The  unsupervised Do() function */

	bool  SpectralEnvelopeExtract::Do(const SpectralPeakArray& input, Spectrum& output)
	{
		output.SetScale(input.GetScale());
		
		TSize nPeaks=input.GetnPeaks();
				
		if (nPeaks<4) return false; //cannot extract an envelope out of only 3 peaks!

		CheckOutputType(output);

		DataArray& magBuffer=input.GetMagBuffer();
		DataArray& phaseBuffer=input.GetPhaseBuffer();
		DataArray& freqBuffer=input.GetFreqBuffer();

		Array<Point>& magPointArray=output.GetMagBPF().GetPointArray();
		Array<Point>& phasePointArray=output.GetPhaseBPF().GetPointArray();
	
		//Max number of points allowed: should be a config param
		magPointArray.Resize(mConfig.GetMaxPeaks());
		magPointArray.SetSize(mConfig.GetMaxPeaks());
		phasePointArray.Resize(mConfig.GetMaxPeaks());
		phasePointArray.SetSize(mConfig.GetMaxPeaks());
		
		
		for(int i=0;i<nPeaks;i++)
		{
			magPointArray[i+1].SetX(freqBuffer[i]);
			magPointArray[i+1].SetY(magBuffer[i]);

			phasePointArray[i+1].SetX(freqBuffer[i]);
			phasePointArray[i+1].SetY(phaseBuffer[i]);

		}
		
		//todo: a lot of duplicated code, should extract in different functions
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
			TData lastFreq=freqBuffer[nPeaks-2];
			TData freqGap=lastFreq-freqBuffer[nPeaks-3];
			TData currentFreq=lastFreq+freqGap;
			TData currentMag=magBuffer[nPeaks-2];
		
			while(currentMag>-200)
			{
				currentMag-=(currentFreq/lastFreq-1)*12;
				
				magPointArray[nPeaks].SetY(currentMag);
				magPointArray[nPeaks].SetX(currentFreq);
				phasePointArray[nPeaks].SetY(0);
				phasePointArray[nPeaks].SetX(currentFreq);
				
				currentFreq+=freqGap;
				nPeaks++;
				if(nPeaks==mConfig.GetMaxPeaks()) break;
				
			}
			//we resize arrays to final size
			magPointArray.Resize(nPeaks);
			magPointArray.SetSize(nPeaks);
			phasePointArray.Resize(nPeaks);
			phasePointArray.SetSize(nPeaks);
			

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
			TData lastFreq=freqBuffer[nPeaks-2];
			TData freqGap=lastFreq-freqBuffer[nPeaks-3];
			TData currentFreq=lastFreq+freqGap;
			TData currentMag=magBuffer[nPeaks-2];
		
		
			while(currentMag<0.0000000001)
			{
				currentMag*=CLAM_pow(0.06,(double)(currentFreq/lastFreq-1.0));
				
				magPointArray[nPeaks].SetY(currentMag);
				magPointArray[nPeaks].SetX(currentFreq);
				phasePointArray[nPeaks].SetY(0);
				phasePointArray[nPeaks].SetX(currentFreq);
				
				currentFreq+=freqGap;
				nPeaks++;
				if(nPeaks==mConfig.GetMaxPeaks()) break;
			}
			
			//we resize arrays to final size
			magPointArray.Resize(nPeaks);
			magPointArray.SetSize(nPeaks);
			phasePointArray.Resize(nPeaks);
			phasePointArray.SetSize(nPeaks);
			
		}
		
		output.SetSize(nPeaks);
		output.GetMagBPF().UpdateSplineTable();
		
		return true;
	}

	bool SpectralEnvelopeExtract::CheckOutputType(Spectrum& out) 
	{
		SpecTypeFlags tmpFlags;
		
		tmpFlags.bMagPhaseBPF=1;
		tmpFlags.bMagPhase=0;
		out.SetType(tmpFlags);

		
		out.SetMagBPF(mMagBPF);
		out.SetPhaseBPF(mPhaseBPF);
		
		
		return true;
	}
};//namespace CLAM

