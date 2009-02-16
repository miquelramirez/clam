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

#include "FDFilterGen.hxx"
#include "CLAM_Math.hxx"
#include <iostream>

/** This is really ugly. We should use standard library "infinity()" instead */
#define MINUSINFINITY -99 //Value for an infinitely negative number

namespace CLAM {
	void FDFilterGenConfig::DefaultInit(void)
	{
		AddAll();
		UpdateData();

		SetGain(0);

		SetHighCutOff(0);
		SetLowCutOff(0);

		SetPassBandSlope(0);
		SetStopBandSlope(0);

		SetSpectralRange(22050); //default value for consistency between classes
		
	}

	FDFilterGen::FDFilterGen( const FDFilterGenConfig& c)
		: Output("Output",this)
		, Gain("Gain",this, &FDFilterGen::UpdateControlChangedFlag)
		, HighCutOff( "High Cutoff Frecuency",this, &FDFilterGen::UpdateControlChangedFlag)
		, LowCutOff( "Low Cutoff Frecuency",this, &FDFilterGen::UpdateControlChangedFlag)
		, PassBandSlope("Pass Band Slope",this, &FDFilterGen::UpdateControlChangedFlag)
		, StopBandSlope( "Stop Band Slope",this, &FDFilterGen::UpdateControlChangedFlag)
		, SpectralRange(0)
		, Type( EFDFilterType::eLowPass )
		, mControlChanged( false )
	{ 
		Configure(c); // here all controls are initialized
	};


	
	bool FDFilterGen::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);

		mControlChanged=true; //we want the next Do to perform an action
		if (mConfig.HasSpectralRange())
			SpectralRange=mConfig.GetSpectralRange();
		if (mConfig.HasGain())
			Gain.DoControl(mConfig.GetGain());
		if (mConfig.HasHighCutOff())
			HighCutOff.DoControl(mConfig.GetHighCutOff());
		if (mConfig.HasLowCutOff())
			LowCutOff.DoControl(mConfig.GetLowCutOff());
		if (mConfig.HasPassBandSlope())
			PassBandSlope.DoControl(mConfig.GetPassBandSlope());
		if (mConfig.HasStopBandSlope())
			StopBandSlope.DoControl(mConfig.GetStopBandSlope());
		if (mConfig.HasType())
			Type=mConfig.GetType();
		
		
		return true;
	}
  
	void FDFilterGen::UpdateControlChangedFlag(TControlData val)
	{
		mControlChanged=true;
	}
	
	
	bool FDFilterGen::Do()
	{
		return 0;
	}
	
	bool FDFilterGen::Do(Spectrum &out)
	{
		// Only do something after have received an incontrol.
		if (!mControlChanged) return false;
		mControlChanged=false; 
		
    // Instantiate the BPF buffer of the spectrum if necessary.
		out.AddMagBPF();
		out.AddPhaseBPF();
		if (out.UpdateData()) {
			out.SetBPFSize(10);
		}
		else if (out.GetBPFSize()<10) 
			out.SetBPFSize(10);
		
		
		EScale originalScale = out.GetScale();
		out.SetScale(EScale(EScale::eLog));
		
		TData g,flc,fhc,spb,ssb,fsr;
		g=Gain.GetLastValue();
		flc=LowCutOff.GetLastValue();
		fhc=HighCutOff.GetLastValue();
		spb=PassBandSlope.GetLastValue();
		ssb=StopBandSlope.GetLastValue();
		fsr=SpectralRange;
		
		// Legend:
		// g      stands for Gain
		// flc    stands for Lower cutoff frequency
		// fhc    stands for Higher cutoff frequency
		// spb    stands for Slope for the pass band
		// ssb    stands for Slope for the stop band
		// fsr    stands for Sampling Rate Frequency 

		switch((int)Type)
			{
			case EFDFilterType::eLowPass:
			{
				// We add four points to the BPF ( the four control points for the filter )
				out.SetBPFSize(4); 
				
				TData fsr_by_flc = fsr/flc;

				SetFilterPoint(out,0,0,g);
				SetFilterPoint(out,1,flc*CLAM_pow(2.0,-3.0/ssb),g);
				SetFilterPoint(out,2,flc,g-3);
				SetFilterPoint(out,3,fsr,g-3-ssb*(log(fsr_by_flc)/log(TData(2))));
				break;
			}
			case EFDFilterType::eHighPass:
			{
				out.SetBPFSize(4);
				SetFilterPoint(out,0,0,MINUSINFINITY);
				SetFilterPoint(out,1,fhc,g-3);
				SetFilterPoint(out,2,fhc*CLAM_pow(2.0,3.0/spb),g);
				SetFilterPoint(out,3,fsr,g);
				break;
			}
			case EFDFilterType::eBandPass:
			{
				out.SetBPFSize(6);
				SetFilterPoint(out,0,0,MINUSINFINITY);
				SetFilterPoint(out,1,flc,g-3);
				SetFilterPoint(out,2,flc*CLAM_pow(2.0,3.0/spb),g);
				SetFilterPoint(out,3,fhc*CLAM_pow(2.0,-3.0/ssb),g);
				SetFilterPoint(out,4,fhc,g-3);
				SetFilterPoint(out,5,fsr,MINUSINFINITY);
				
				break;
			}
			case EFDFilterType::eStopBand:
			{
				out.SetBPFSize(7);
				SetFilterPoint(out,0,0,g);
				SetFilterPoint(out,1,flc*CLAM_pow(2.0,-3.0/ssb),g);
				SetFilterPoint(out,2,flc,g-3);
				TData crossFreq=(CLAM_pow((double)flc,(1.0/(1.0-spb/ssb)))/CLAM_pow((double)fhc,(1.0/(ssb/spb-1.0))));
				TData crossf_by_flc = crossFreq/flc;
				SetFilterPoint(out,3,crossFreq,g-3-ssb*(log(crossf_by_flc)/log(TData(2))));
				SetFilterPoint(out,4,fhc,g-3);
				SetFilterPoint(out,5,fhc*CLAM_pow(2.0,3.0/spb),g);
				SetFilterPoint(out,6,fsr,g);
				break;
			}

			}
		if (originalScale==EScale::eLinear)
		{	
			int bpfSize=out.GetBPFSize();
			BPF& bpf=out.GetMagBPF();
		
			for (int i=0; i<bpfSize; i++) {
				
				TData tmpValue = bpf.GetValueFromIndex(i);
						
				tmpValue = (tmpValue==0.0001) ? 0 : CLAM_pow(10.0,tmpValue/20.0); 
				bpf.SetValue(i, tmpValue);
			}
			out.SetScale(EScale(EScale::eLinear));
		}

		return true; 
	}
	
	void FDFilterGen::SetFilterPoint(Spectrum& out,TIndex pos,TData freq,TData mag,TData phase)
	{
		out.GetMagBPF().SetValue(pos, mag);
		out.GetMagBPF().SetXValue(pos,freq);
		out.GetPhaseBPF().SetValue(pos,phase);
		out.GetPhaseBPF().SetXValue(pos,freq);
	}
	
}

