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

#ifndef _SPECTRAL_ENVELOPE_EXTRACTOR_HXX
#define _SPECTRAL_ENVELOPE_EXTRACTOR_HXX

#include "ProcessingComposite.hxx"
#include "Port.hxx"
#include "MaskingPeakDetect.hxx"
#include "SpectralEnvelopeGenerator.hxx"

namespace CLAM
{

	class SpectralEnvelopeExtractorConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpectralEnvelopeExtractorConfig, 6, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string,                     Name);
		DYN_ATTRIBUTE (1, public, TSize,                           SpectralSize);
		DYN_ATTRIBUTE (2, public, TData,                           SpectralRange);
		DYN_ATTRIBUTE (3, public, TSize,                           nMaxPeaks);
		DYN_ATTRIBUTE (4, public, MaskingPeakDetectConfig,         DetectConfig);
		DYN_ATTRIBUTE (5, public, SpectralEnvelopeGeneratorConfig, GeneratorConfig);
		void Synchronise();
	protected:
		void DefaultInit(void);
	};

	class SpectralEnvelopeExtractor: public ProcessingComposite
	{
	public:

		InPortTmpl<Spectrum>  Input;
		OutPortTmpl<Spectrum> Output;

		InControlTmpl<SpectralEnvelopeExtractor>  MemoryFactor;

		SpectralEnvelopeExtractor(const SpectralEnvelopeExtractorConfig& c = 
								  SpectralEnvelopeExtractorConfig());

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(const Spectrum& inp, Spectrum &out);

		void StoreOn(Storage &s) {};

	private:

		SpectralEnvelopeExtractorConfig mConfig;

		MaskingPeakDetect         mDetector;

		SpectralEnvelopeGenerator mGenerator;

		SpectralPeakArray mPeaks;

		void CheckData(const Spectrum& inp, Spectrum &out);

		bool ConcreteConfigure(const ProcessingConfig& cfg) throw(std::bad_cast);

		int MemoryFactorChange(TControlData value);

	public:

		const SpectralPeakArray &DebugGetPeaks() const { return mPeaks; }

	};
	
}

#endif
