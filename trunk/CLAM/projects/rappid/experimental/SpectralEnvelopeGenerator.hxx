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

#ifndef _SPECTRAL_ENVELOPE_GENERATOR_HXX
#define _SPECTRAL_ENVELOPE_GENERATOR_HXX

#include "ControlConfig.hxx"
#include "Processing.hxx"
#include "Port.hxx"
#include "SpectralPeakArray.hxx"
#include "Spectrum.hxx"

namespace CLAM
{

	class SpectralEnvelopeGeneratorConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpectralEnvelopeGeneratorConfig, 6, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string,   Name);
		DYN_ATTRIBUTE (1, public, ControlConfig, MemoryFactor);
		DYN_ATTRIBUTE (2, public, int,           nMaxPeaks);
		DYN_ATTRIBUTE (3, public, TData,         SpectralRange);
		DYN_ATTRIBUTE (4, public, int,           SpectralSize);
		DYN_ATTRIBUTE (5, public, EInterpolation,InterpolationType);
	protected:
		void DefaultInit(void);
	};

	class SpectralEnvelopeGenerator: public Processing
	{
	public:

		InPortTmpl<SpectralPeakArray>  Input;
		OutPortTmpl<Spectrum>          Output;

		InControlTmpl<SpectralEnvelopeGenerator>  MemoryFactor;

		BPF_ mBPF;
		bool mIsSpline;
		int   mSize;
		TData mDelta;
		TData mMinMemoryFactor;
		TData mMaxMemoryFactor;

		Array<TData> mMemory;

		// Control value.
		float mMemoryFactor; // likely atomic.

		SpectralEnvelopeGenerator(const SpectralEnvelopeGeneratorConfig& c = 
								  SpectralEnvelopeGeneratorConfig());

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(const SpectralPeakArray& inp, Spectrum &out);

		void StoreOn(Storage &s) {};

	private:

		SpectralEnvelopeGeneratorConfig mConfig;

		void ConfigureBPF(int n_peaks, const Array<TData> &mags);

		void CheckData(const SpectralPeakArray& inp, Spectrum &out);

		bool ConcreteConfigure(const ProcessingConfig& cfg) throw(std::bad_cast);

		int MemoryFactorChange(TControlData value);

	public:

	};
	
}

#endif
