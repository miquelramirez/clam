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

#ifndef __FrameDescriptors__
#define __FrameDescriptors__

#include "Flags.hxx"
#include "DynamicType.hxx"
#include "ProcessingData.hxx"
#include "DataTypes.hxx"

#include "AudioDescriptors.hxx"
#include "SpectralDescriptors.hxx"
#include "SpectralPeakDescriptors.hxx"

namespace CLAM{


class Frame;

/*
 * This class holds Descriptors computed from Frame data. Right now no specific
 * descriptors are computed for frame (only for spectrums, audios... that are held
 * inside the frame. TODO: add specific descriptors.
 *
 */
class FrameDescriptors : public ProcessingData
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (FrameDescriptors, 8, ProcessingData);
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

public:
	void DefaultInit();

	FrameDescriptors(Frame* pFrame);
		
	const Frame* GetpFrame() const{return mpFrame;}
	void SetpFrame(Frame* pFrame) {mpFrame=pFrame;}

	
private:
	Frame* mpFrame;

};


};//CLAM



#endif
