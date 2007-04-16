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
#include "SpectralEnvelopeApply.hxx"

#define CLASS "SpectralEnvelopeApply"

namespace CLAM {

	/* Processing  object Method  implementations */

	SpectralEnvelopeApply::SpectralEnvelopeApply()
	{
		Configure(SpectralEnvelopeApplyConfig());
	}

	SpectralEnvelopeApply::SpectralEnvelopeApply(const SpectralEnvelopeApplyConfig &c = SpectralEnvelopeApplyConfig())
	{
		Configure(c);
	}

	SpectralEnvelopeApply::~SpectralEnvelopeApply()
	{}


	/* Configure the Processing Object according to the Config object */

	bool SpectralEnvelopeApply::ConcreteConfigure(const ProcessingConfig& c)
	{

		CopyAsConcreteConfig(mConfig, c);
		return true;
	}

	/* Setting Prototypes for faster processing */

	bool SpectralEnvelopeApply::SetPrototypes(const SpectralPeakArray& input,Spectrum& output)
	{
		return true;
	}

	bool SpectralEnvelopeApply::SetPrototypes()
	{
		return true;
	}
	
	bool SpectralEnvelopeApply::UnsetPrototypes()
	{
		return true;
	}

	/* The supervised Do() function */

	bool  SpectralEnvelopeApply::Do(void) 
	{
		CLAM_ASSERT(false,CLASS"::Do(): Supervised mode not implemented");
		return false;
	}

	/* The  unsupervised Do() function */
	bool  SpectralEnvelopeApply::Do(const SpectralPeakArray& input, const Spectrum& spectralEnvelope, SpectralPeakArray& output)
	{
		output.SetScale(input.GetScale());
		CLAM_ASSERT(input.GetScale()==spectralEnvelope.GetScale(),"You are trying to apply an envelope that has a different scale");
				
		TSize nPeaks=input.GetnPeaks();
		//Unused variable: DataArray& imagBuffer=input.GetMagBuffer();
		DataArray& iphaseBuffer=input.GetPhaseBuffer();
		DataArray& ifreqBuffer=input.GetFreqBuffer();

		if(output.GetnMaxPeaks()!=input.GetnMaxPeaks())
			output.SetnMaxPeaks(input.GetnMaxPeaks());
		if(output.GetnPeaks()!=input.GetnPeaks())
			output.SetnPeaks(input.GetnPeaks());
		
		DataArray& omagBuffer=output.GetMagBuffer();
		DataArray& ophaseBuffer=output.GetPhaseBuffer();
		DataArray& ofreqBuffer=output.GetFreqBuffer();

		int i;
		
		BPF& env = spectralEnvelope.GetMagBPF();
		for(i=0;i<nPeaks;i++)
		{
			ophaseBuffer[i]=iphaseBuffer[i];
			omagBuffer[i]=env.GetValue(ifreqBuffer[i]);
			ofreqBuffer[i]=ifreqBuffer[i];
		}
		
		return true;
	}

	/* The  unsupervised Do() function */
	bool  SpectralEnvelopeApply::Do(const Spectrum& input, const Spectrum& spectralEnvelope, Spectrum& output)
	{
		output.SetScale(input.GetScale());
		CLAM_ASSERT(input.GetScale()==spectralEnvelope.GetScale(),"You are trying to apply an envelope that has a different scale");
		
		output=input;
		
		int spectrumSize=input.GetSize();
		
		int i;
		TData delta = input.GetSpectralRange()/(spectrumSize-1);
		DataArray& outMag = output.GetMagBuffer();
		BPF& env = spectralEnvelope.GetMagBPF();
		
		TData currentFreq = 0.;
		for(i=0;i<spectrumSize;i++, currentFreq+=delta)
		//for(i=0;currentFreq<10000;i++, currentFreq+=delta)
		{
			//std::cout<<outMag[i] << " will be " << env.GetValue(currentFreq) <<std::endl;
			outMag[i] = env.GetValue(currentFreq);
			
			//output.SetMag(i,spectralEnvelope.GetMag((TData)i*delta));
		}


		return true;
	}


};//namespace CLAM

