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




#include "ProcessingData.hxx"
#include "FrameDescriptors.hxx"
#include "Frame.hxx"


namespace CLAM{

FrameDescriptors::FrameDescriptors(Frame* pFrame): Descriptor(eNumAttr)
{
	MandatoryInit();
	mpFrame=pFrame;
}

FrameDescriptors::FrameDescriptors(TData initVal): Descriptor(eNumAttr)
{
	MandatoryInit();
	AddAll();
	UpdateData();
	SetSpectralPeakD(SpectralPeakDescriptors(initVal));
	SetSpectrumD(SpectralDescriptors(initVal));
	SetResidualSpecD(SpectralDescriptors(initVal));
	SetSinusoidalSpecD(SpectralDescriptors(initVal));
	SetAudioFrameD(AudioDescriptors(initVal));
	SetSinusoidalAudioFrameD(AudioDescriptors(initVal));
	SetResidualAudioFrameD(AudioDescriptors(initVal));
	SetSynthAudioFrameD(AudioDescriptors(initVal));
}

void FrameDescriptors::DefaultInit() {
	mpFrame=0;
}

void FrameDescriptors::CopyInit(const FrameDescriptors & copied) {
	mpFrame=copied.mpFrame;
}

const Frame* FrameDescriptors::GetpFrame() const {
	return mpFrame;
}

void FrameDescriptors::SetpFrame(const Frame* pFrame){
	mpFrame=pFrame;
	//first we add dynamic attributes doing a single UpdateData
	if(mpFrame->HasSpectrum())
		AddSpectrumD();
	if(mpFrame->HasSpectralPeakArray())
		AddSpectralPeakD();
	if(mpFrame->HasResidualSpec())
		AddResidualSpecD();
	if(mpFrame->HasSinusoidalSpec())
		AddSinusoidalSpecD();
	if(mpFrame->HasAudioFrame())
		AddAudioFrameD();
	if(mpFrame->HasSinusoidalAudioFrame())
		AddSinusoidalAudioFrameD();
	if(mpFrame->HasResidualAudioFrame())
		AddResidualAudioFrameD();
	if(mpFrame->HasSynthAudioFrame())
		AddSynthAudioFrameD();
	UpdateData();
	//now we set the data of each descriptor
	if(mpFrame->HasSpectrum())
		GetSpectrumD().SetpSpectrum(&mpFrame->GetSpectrum());
	if(mpFrame->HasSpectralPeakArray())
		GetSpectralPeakD().SetpSpectralPeakArray(&mpFrame->GetSpectralPeakArray());
	if(mpFrame->HasResidualSpec())
		GetResidualSpecD().SetpSpectrum(&mpFrame->GetResidualSpec());
	if(mpFrame->HasSinusoidalSpec())
		GetSinusoidalSpecD().SetpSpectrum(&mpFrame->GetSinusoidalSpec());
	if(mpFrame->HasAudioFrame())
		GetAudioFrameD().SetpAudio(&mpFrame->GetAudioFrame());
	if(mpFrame->HasSinusoidalAudioFrame())
		GetSinusoidalAudioFrameD().SetpAudio(&mpFrame->GetSinusoidalAudioFrame());
	if(mpFrame->HasResidualAudioFrame())
		GetResidualAudioFrameD().SetpAudio(&mpFrame->GetResidualAudioFrame());
	if(mpFrame->HasSynthAudioFrame())
		GetSynthAudioFrameD().SetpAudio(&mpFrame->GetSynthAudioFrame());

}

void FrameDescriptors::Compute()
{
	/*Overriding compute method in base class because right now I don't know
	what to do with member statistics.*/
	ConcreteCompute();
}


void FrameDescriptors::ConcreteCompute()
{
	if(mpFrame->HasSpectrum())
		GetSpectrumD().Compute();
	if(mpFrame->HasSpectralPeakArray())
		GetSpectralPeakD().Compute();
	if(mpFrame->HasResidualSpec())
		GetResidualSpecD().Compute();
	if(mpFrame->HasSinusoidalSpec())
		GetSinusoidalSpecD().Compute();
	if(mpFrame->HasAudioFrame())
		GetAudioFrameD().Compute();
	if(mpFrame->HasSinusoidalAudioFrame())
		GetSinusoidalAudioFrameD().Compute();
	if(mpFrame->HasResidualAudioFrame())
		GetResidualAudioFrameD().Compute();
	if(mpFrame->HasSynthAudioFrame())
		GetSynthAudioFrameD().Compute();
	if(HasCenterTime())
		SetCenterTime(mpFrame->GetCenterTime());	


}

FrameDescriptors operator * (const FrameDescriptors& a,const FrameDescriptors& b)
{
	FrameDescriptors tmpD;
	if(a.HasSpectralPeakD() && b.HasSpectralPeakD())
	{
		tmpD.AddSpectralPeakD();
		tmpD.UpdateData();
		tmpD.SetSpectralPeakD(a.GetSpectralPeakD()*b.GetSpectralPeakD());
	}
	if(a.HasSpectrumD() && b.HasSpectrumD())
	{
		tmpD.AddSpectrumD();
		tmpD.UpdateData();
		tmpD.SetSpectrumD(a.GetSpectrumD()*b.GetSpectrumD());
	}
	if(a.HasResidualSpecD() && b.HasResidualSpecD() )
	{
		tmpD.AddResidualSpecD();
		tmpD.UpdateData();
		tmpD.SetResidualSpecD(a.GetResidualSpecD()*b.GetResidualSpecD());
	}
	if(a.HasSinusoidalSpecD() && b.HasSinusoidalSpecD())
	{
		tmpD.AddSinusoidalSpecD();
		tmpD.UpdateData();
		tmpD.SetSinusoidalSpecD(a.GetSinusoidalSpecD()*b.GetSinusoidalSpecD());
	}
	if(a.HasAudioFrameD() && b.HasAudioFrameD() )
	{
		tmpD.AddAudioFrameD();
		tmpD.UpdateData();
		tmpD.SetAudioFrameD(a.GetAudioFrameD()*b.GetAudioFrameD());
	}
	if(a.HasSinusoidalAudioFrameD() && b.HasSinusoidalAudioFrameD() )
	{
		tmpD.AddSinusoidalAudioFrameD();
		tmpD.UpdateData();
		tmpD.SetSinusoidalAudioFrameD(a.GetSinusoidalAudioFrameD()*b.GetSinusoidalAudioFrameD());
	}
	if(a.HasResidualAudioFrameD() && b.HasResidualAudioFrameD() )
	{
		tmpD.AddResidualAudioFrameD();
		tmpD.UpdateData();
		tmpD.SetResidualAudioFrameD(a.GetResidualAudioFrameD()*b.GetResidualAudioFrameD());
	}
	if(a.HasSynthAudioFrameD() && b.HasSynthAudioFrameD() )
	{
		tmpD.AddSynthAudioFrameD();
		tmpD.UpdateData();
		tmpD.SetSynthAudioFrameD(a.GetSynthAudioFrameD()*b.GetSynthAudioFrameD());
	}

	return tmpD;
}


FrameDescriptors operator + (const FrameDescriptors& a,const FrameDescriptors& b) 
{
	FrameDescriptors tmpD;
	if(a.HasSpectralPeakD() && b.HasSpectralPeakD())
	{
		tmpD.AddSpectralPeakD();
		tmpD.UpdateData();
		tmpD.SetSpectralPeakD(a.GetSpectralPeakD()+b.GetSpectralPeakD());
	}
	if(a.HasSpectrumD() && b.HasSpectrumD())
	{
		tmpD.AddSpectrumD();
		tmpD.UpdateData();
		tmpD.SetSpectrumD(a.GetSpectrumD()+b.GetSpectrumD());
	}
	if(a.HasResidualSpecD() && b.HasResidualSpecD() )
	{
		tmpD.AddResidualSpecD();
		tmpD.UpdateData();
		tmpD.SetResidualSpecD(a.GetResidualSpecD()+b.GetResidualSpecD());
	}
	if(a.HasSinusoidalSpecD() && b.HasSinusoidalSpecD())
	{
		tmpD.AddSinusoidalSpecD();
		tmpD.UpdateData();
		tmpD.SetSinusoidalSpecD(a.GetSinusoidalSpecD()+b.GetSinusoidalSpecD());
	}
	if(a.HasAudioFrameD() && b.HasAudioFrameD() )
	{
		tmpD.AddAudioFrameD();
		tmpD.UpdateData();
		tmpD.SetAudioFrameD(a.GetAudioFrameD()+b.GetAudioFrameD());
	}
	if(a.HasSinusoidalAudioFrameD() && b.HasSinusoidalAudioFrameD() )
	{
		tmpD.AddSinusoidalAudioFrameD();
		tmpD.UpdateData();
		tmpD.SetSinusoidalAudioFrameD(a.GetSinusoidalAudioFrameD()+b.GetSinusoidalAudioFrameD());
	}
	if(a.HasResidualAudioFrameD() && b.HasResidualAudioFrameD() )
	{
		tmpD.AddResidualAudioFrameD();
		tmpD.UpdateData();
		tmpD.SetResidualAudioFrameD(a.GetResidualAudioFrameD()+b.GetResidualAudioFrameD());
	}
	if(a.HasSynthAudioFrameD() && b.HasSynthAudioFrameD() )
	{
		tmpD.AddSynthAudioFrameD();
		tmpD.UpdateData();
		tmpD.SetSynthAudioFrameD(a.GetSynthAudioFrameD()+b.GetSynthAudioFrameD());
	}

	return tmpD;
}


FrameDescriptors operator * (const FrameDescriptors& a,TData mult)
{
	FrameDescriptors tmpD(a);
	if(a.HasSpectralPeakD())
	{
		tmpD.SetSpectralPeakD(a.GetSpectralPeakD()*mult);
	}
	if(a.HasSpectrumD())
	{
		tmpD.SetSpectrumD(a.GetSpectrumD()*mult);
	}
	if(a.HasResidualSpecD())
	{
		tmpD.SetResidualSpecD(a.GetResidualSpecD()*mult);
	}
	if(a.HasSinusoidalSpecD())
	{
		tmpD.SetSinusoidalSpecD(a.GetSinusoidalSpecD()*mult);
	}
	if(a.HasAudioFrameD())
	{
		tmpD.SetAudioFrameD(a.GetAudioFrameD()*mult);
	}
	if(a.HasSinusoidalAudioFrameD())
	{
		tmpD.SetSinusoidalAudioFrameD(a.GetSinusoidalAudioFrameD()*mult);
	}
	if(a.HasResidualAudioFrameD())
	{
		tmpD.SetResidualAudioFrameD(a.GetResidualAudioFrameD()*mult);
	}
	if(a.HasSynthAudioFrameD())
	{
		tmpD.SetSynthAudioFrameD(a.GetSynthAudioFrameD()*mult);
	}

	return tmpD;

}

FrameDescriptors operator * (TData mult,const FrameDescriptors& a)
{
	return a*mult;
}

FrameDescriptors operator - (const FrameDescriptors& a,const FrameDescriptors& b)
{
	return a+((-1.0)*b);
}
FrameDescriptors operator / (const FrameDescriptors& a,TData div) 
{ 
	return a*(1/div);
}


}//CLAM

