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

#ifndef __FrameDescriptors__
#define __FrameDescriptors__

#include "Descriptor.hxx"

#include "AudioDescriptors.hxx"
#include "SpectralDescriptors.hxx"
#include "SpectralPeakDescriptors.hxx"
#include "MorphologicalFrameDescriptors.hxx"

namespace CLAM{


class Frame;

/*
 * This class holds Descriptors computed from Frame data. Right now no specific
 * descriptors are computed for frame (only for spectrums, audios... that are held
 * inside the frame. TODO: add specific descriptors.
 *
 */
class FrameDescriptors : public Descriptor
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (FrameDescriptors, 10, Descriptor);
	/** Spectrum analyzed from the Audio input  */
	DYN_ATTRIBUTE (0, public, SpectralDescriptors, SpectrumD);
	/** Vector of peaks in spectral analysis  */
	DYN_ATTRIBUTE (1, public, SpectralPeakDescriptors, SpectralPeakD);
	/** Residual spectrum of the Audio  */
	DYN_ATTRIBUTE (2, public, SpectralDescriptors, ResidualSpecD);
	/** Sinusoidal spectrum of the Audio  */
	DYN_ATTRIBUTE (3, public, SpectralDescriptors, SinusoidalSpecD);
	/*Audio chunk that has been used for generating spectral data, will usually be
	a previously windowed audio chunk*/
	/** Audio chunk we want to analyze */
	DYN_ATTRIBUTE (4, public, AudioDescriptors, AudioFrameD);
	/** Sinusoidal synthesized Audio */
	DYN_ATTRIBUTE (5, public, AudioDescriptors, SinusoidalAudioFrameD);
	/** Residual synthesized Audio */
	DYN_ATTRIBUTE (6, public, AudioDescriptors, ResidualAudioFrameD);
	/** Global synthesized Audio */
	DYN_ATTRIBUTE (7, public, AudioDescriptors, SynthAudioFrameD);
	

	/** Morphological descriptors */
	DYN_ATTRIBUTE (8, public, MorphologicalFrameDescriptors, MorphologicalFrameD);

	/** Frame Center Time is some times interesting to also have it in descriptors */	
	DYN_ATTRIBUTE (9, public, TTime, CenterTime);	

	//Note: some specific frame descriptors should be added
public:
	FrameDescriptors(Frame* pFrame);
	FrameDescriptors(TData initVal);

	const Frame* GetpFrame() const;
	void SetpFrame(const Frame* pFrame);
	void Compute();
	void ConcreteCompute();
	
	TData GetFundamental() {return mpFrame->GetFundamental().GetFreq(0);}

private:
	void DefaultInit();
	void CopyInit(const FrameDescriptors & copied);

private:
	const Frame* mpFrame;

};

FrameDescriptors operator * (const FrameDescriptors& a,const FrameDescriptors& b);
FrameDescriptors operator + (const FrameDescriptors& a,const FrameDescriptors& b);
FrameDescriptors operator * (const FrameDescriptors& a,TData mult);
FrameDescriptors operator * (TData mult,const FrameDescriptors& a);
FrameDescriptors operator - (const FrameDescriptors& a,const FrameDescriptors& b);
FrameDescriptors operator / (const FrameDescriptors& a,TData div);

template<>
inline FrameDescriptors CLAM_max (const FrameDescriptors& a,const FrameDescriptors& b)
{
	FrameDescriptors  tmpD(a);
	if(a.HasSpectralPeakD() && b.HasSpectralPeakD())
	{
		tmpD.SetSpectralPeakD(CLAM_max(a.GetSpectralPeakD(),b.GetSpectralPeakD()));
	}
	if(a.HasSpectrumD() && b.HasSpectrumD())
	{
		tmpD.SetSpectrumD(CLAM_max(a.GetSpectrumD(),b.GetSpectrumD()));
	}
	if(a.HasResidualSpecD() && b.HasResidualSpecD())
	{
		tmpD.SetResidualSpecD(CLAM_max(a.GetResidualSpecD(),b.GetResidualSpecD()));
	}
	if(a.HasSinusoidalSpecD() && b.HasSinusoidalSpecD())
	{
		tmpD.SetSinusoidalSpecD(CLAM_max(a.GetSinusoidalSpecD(),b.GetSinusoidalSpecD()));
	}
	if(a.HasAudioFrameD() && b.HasAudioFrameD())
	{
		tmpD.SetAudioFrameD(CLAM_max(a.GetAudioFrameD(),b.GetAudioFrameD()));
	}
	if(a.HasSinusoidalAudioFrameD() && b.HasSinusoidalAudioFrameD())
	{
		tmpD.SetSinusoidalAudioFrameD(CLAM_max(a.GetSinusoidalAudioFrameD(),b.GetSinusoidalAudioFrameD()));
	}
	if(a.HasResidualAudioFrameD() && b.HasResidualAudioFrameD())
	{
		tmpD.SetResidualAudioFrameD(CLAM_max(a.GetResidualAudioFrameD(),b.GetResidualAudioFrameD()));
	}
	if(a.HasSynthAudioFrameD() && b.HasSynthAudioFrameD())
	{
		tmpD.SetSynthAudioFrameD(CLAM_max(a.GetSynthAudioFrameD(),b.GetSynthAudioFrameD()));
	}
	return tmpD;
}

template<>
inline FrameDescriptors CLAM_min (const FrameDescriptors& a,const FrameDescriptors& b)
{
	FrameDescriptors  tmpD(a);
	if(a.HasSpectralPeakD() && b.HasSpectralPeakD())
	{
		tmpD.SetSpectralPeakD(CLAM_min(a.GetSpectralPeakD(),b.GetSpectralPeakD()));
	}
	if(a.HasSpectrumD() && b.HasSpectrumD())
	{
		tmpD.SetSpectrumD(CLAM_min(a.GetSpectrumD(),b.GetSpectrumD()));
	}
	if(a.HasResidualSpecD() && b.HasResidualSpecD())
	{
		tmpD.SetResidualSpecD(CLAM_min(a.GetResidualSpecD(),b.GetResidualSpecD()));
	}
	if(a.HasSinusoidalSpecD() && b.HasSinusoidalSpecD())
	{
		tmpD.SetSinusoidalSpecD(CLAM_min(a.GetSinusoidalSpecD(),b.GetSinusoidalSpecD()));
	}
	if(a.HasAudioFrameD() && b.HasAudioFrameD())
	{
		tmpD.SetAudioFrameD(CLAM_min(a.GetAudioFrameD(),b.GetAudioFrameD()));
	}
	if(a.HasSinusoidalAudioFrameD() && b.HasSinusoidalAudioFrameD())
	{
		tmpD.SetSinusoidalAudioFrameD(CLAM_min(a.GetSinusoidalAudioFrameD(),b.GetSinusoidalAudioFrameD()));
	}
	if(a.HasResidualAudioFrameD() && b.HasResidualAudioFrameD())
	{
		tmpD.SetResidualAudioFrameD(CLAM_min(a.GetResidualAudioFrameD(),b.GetResidualAudioFrameD()));
	}
	if(a.HasSynthAudioFrameD() && b.HasSynthAudioFrameD())
	{
		tmpD.SetSynthAudioFrameD(CLAM_min(a.GetSynthAudioFrameD(),b.GetSynthAudioFrameD()));
	}
	return tmpD;
}




};//CLAM



#endif

