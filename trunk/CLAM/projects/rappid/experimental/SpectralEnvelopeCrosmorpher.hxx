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

#ifndef _SPECTRAL_ENVELOPE_CROSMORPHER_
#define _SPECTRAL_ENVELOPE_CROSMORPHER_

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "Port.hxx"
#include "Spectrum.hxx"

namespace CLAM {


	class SpectralEnvelopeCrosmorpherConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpectralEnvelopeCrosmorpherConfig, 1,ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
	protected:
		void DefaultInit();

	};

	class SpectralEnvelopeCrosmorpher: public ProcessingComposite {

		SpectralEnvelopeCrosmorpherConfig mConfig;

		int mSize;

		InPortTmpl<Spectrum>  Input1;
		InPortTmpl<Spectrum>  Input2;
		OutPortTmpl<Spectrum> Output;

		InControl MorphFactor1;

		InControl MorphFactor2;

		// Processing methods

		void CheckData(Spectrum&,Spectrum&,Spectrum&);

		ExtractEnvelope(Spectrum&,Spectrum&);

		MorphEnvelopes(Spectrum&,Spectrum&,Spectrum&);

		MultiplyMag(Spectrum&,Spectrum&,Spectrum&);

		MultiplyMag(mIn2,mMorphedEnvelope2,mOut2);

		AddMagPhase(Spectrum&,Spectrum&,Spectrum&);



		const char *GetClassName() {return "SpectralEnvelopeCrosmorpher";}

		bool ConcreteConfigure(const ProcessingConfig&)

	public:
		SpectralEnvelopeCrosmorpher();

		SpectralEnvelopeCrosmorpher(const SpectralEnvelopeCrosmorpherConfig &c);

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(Spectrum& in1, Spectrum& in2, Spectrum& out);


	};

}

#endif // _SPECTRAL_ENVELOPE_CROSMORPHER_
