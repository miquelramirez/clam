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

#include "FDFilterGen.hxx"
#include "CLAM_Math.hxx"
#include <iostream>

/** This is really ugly. We should use standard library "infinity()" instead */
#define MINUSINFINITY -99 //Value for an infinitely negative number

namespace CLAM {

	Enum::tEnumValue EFDFilterGenControls::sEnumValues[] =
	{
		{ EFDFilterGenControls::gain, "gain" },
		{ EFDFilterGenControls::highcutoff, "highcutoff" },
		{ EFDFilterGenControls::lowcutoff, "lowcutoff" },
		{ EFDFilterGenControls::passbandslope, "passbandslope" },
		{ EFDFilterGenControls::stopbandslope, "stopbandslope" },
		{ 0, NULL }
	};
	
	Enum::tValue EFDFilterGenControls::sDefault = EFDFilterGenControls::gain;
	
	
	Enum::tEnumValue EFDFilterType::sEnumValues[] = {
		{EFDFilterType::eLowPass,"Low-pass"},
		{EFDFilterType::eHighPass,"High-pass"},
		{EFDFilterType::eBandPass,"Band-pass"},
		{EFDFilterType::eStopBand,"Stop-Band"},
		{0,NULL}
	};
	
	Enum::tValue EFDFilterType::sDefault = EFDFilterType::eLowPass;
	
	
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

	

	FDFilterGen::FDFilterGen() :
		Output("Output",this,1),
		Gain("Gain",this, &FDFilterGen::UpdateControlChangedFlag), 
		HighCutOff( "High Cutoff Frecuency",this, &FDFilterGen::UpdateControlChangedFlag),
		LowCutOff( "Low Cutoff Frecuency",this, &FDFilterGen::UpdateControlChangedFlag),
		PassBandSlope("Pass Band Slope",this, &FDFilterGen::UpdateControlChangedFlag),
		StopBandSlope( "Stop Band Slope",this, &FDFilterGen::UpdateControlChangedFlag),
		SpectralRange(0),
		Type( EFDFilterType::eLowPass ),
		mControlChanged( false )

	{ 
		Configure(FDFilterGenConfig()); // here all controls are initialized
	};

	FDFilterGen::FDFilterGen( const FDFilterGenConfig& c) :
		Output("Output",this,1),
		SpectralRange(0),
		mControlChanged( false ),
		Type( EFDFilterType::eLowPass ),
		Gain("Gain",this, &FDFilterGen::UpdateControlChangedFlag), 
		HighCutOff( "High Cutoff Frecuency",this, &FDFilterGen::UpdateControlChangedFlag),
		LowCutOff( "Low Cutoff Frecuency",this, &FDFilterGen::UpdateControlChangedFlag),
		PassBandSlope("Pass Band Slope",this, &FDFilterGen::UpdateControlChangedFlag),
		StopBandSlope( "Stop Band Slope",this, &FDFilterGen::UpdateControlChangedFlag)
	{ 
		Configure(c); // here all controls are initialized
	};


	
	bool FDFilterGen::ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
	{
		mConfig = dynamic_cast<const FDFilterGenConfig&>(c);
		
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
  
	int FDFilterGen::UpdateControlChangedFlag(TControlData val)
	{
		mControlChanged=true;
		return 0;
	}
	
	
	bool FDFilterGen::Do(void)
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
		if (out.UpdateData())
			{
				out.SetBPFSize(10);
			}
		else
			if (out.GetBPFSize()<10) 
				out.SetBPFSize(10);
		
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
		// fsr    stands for Sampling Rate Frequency (?) MRJ: Sampling Rate = Frecuencia de Muestro isn't it?
		
		// MRJ: Why this? if it was intended to not evaluate the indirections more than once it is quite unnecessary
		// since below there is a switch each branch will ONLY be evaluated if the preceding case evaluates to true.
		// In my opinion this only obscures the code for no reason ( and variable names are translated into symbols
		// (i.e. indexes in the program symbol table) so there is NO overhead at runtime about being descriptive
		// with variable names )
		
		switch((int)Type)
			{
			case EFDFilterType::eLowPass:
			{
				// We add four points to the BPF ( the four control points for the filter )
				out.SetBPFSize(4); 
				
				TData fsr_by_flc = fsr/flc;

				SetFilterPoint(out,0,0,g);
				SetFilterPoint(out,1,flc*pow(2,-3/ssb),g);
				SetFilterPoint(out,2,flc,g-3);
				SetFilterPoint(out,3,fsr,g-3-ssb*(log(fsr_by_flc)/log(TData(2))));
				break;
			}
			case EFDFilterType::eHighPass:
			{
				out.SetBPFSize(4);
				SetFilterPoint(out,0,0,MINUSINFINITY);
				SetFilterPoint(out,1,fhc,g-3);
				SetFilterPoint(out,2,fhc*pow(2,3/spb),g);
				SetFilterPoint(out,3,fsr,g);
				break;
			}
			case EFDFilterType::eBandPass:
			{
				out.SetBPFSize(6);
				SetFilterPoint(out,0,0,MINUSINFINITY);
				SetFilterPoint(out,1,flc,g-3);
				SetFilterPoint(out,2,flc*pow(2,3/spb),g);
				SetFilterPoint(out,3,fhc*pow(2,-3/ssb),g);
				SetFilterPoint(out,4,fhc,g-3);
				SetFilterPoint(out,5,fsr,MINUSINFINITY);
				
				break;
			}
			case EFDFilterType::eStopBand:
			{
				out.SetBPFSize(7);
				SetFilterPoint(out,0,0,g);
				SetFilterPoint(out,1,flc*pow(2,-3/ssb),g);
				SetFilterPoint(out,2,flc,g-3);
				TData crossFreq=(pow(flc,(1/(1-spb/ssb)))/pow(fhc,(1/(ssb/spb-1))));
				TData crossf_by_flc = crossFreq/flc;
				SetFilterPoint(out,3,crossFreq,g-3-ssb*(log(crossf_by_flc)/log(TData(2))));
				SetFilterPoint(out,4,fhc,g-3);
				SetFilterPoint(out,5,fhc*pow(2,3/spb),g);
				SetFilterPoint(out,6,fsr,g);
				break;
			}

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
