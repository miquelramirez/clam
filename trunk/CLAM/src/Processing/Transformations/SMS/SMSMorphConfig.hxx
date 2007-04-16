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
#ifndef __SMSMORPHCONFIG__
#define __SMSMORPHCONFIG__

#include "Processing.hxx"
#include "BPF.hxx"


namespace CLAM
{
	class SMSMorphConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SMSMorphConfig, 16,ProcessingConfig);
		/** FileName where pre-analyzed sound to morph is to be found */
		DYN_ATTRIBUTE (0, public, std::string, FileName);
		/** BPF (envelope-like) Parameter. Defines how much of each sound is being used from 0 to 1  */
		DYN_ATTRIBUTE (1, public, BPF, HybBPF);
		/** boolean Parameter. Defines whether intermediate frames should be interpolated from
			existing ones. */
		DYN_ATTRIBUTE (2, public, bool, InterpolateFrame);
		/** BPF (envelope-like) Parameter. Defines temporal relation between input sound and sound
			to hybridize */
		DYN_ATTRIBUTE (3, public, BPF, SynchronizeTime);
		/** BPF (envelope-like) Parameter. Amount of amplitude of sinusoids to take from input sound (0)
			or sound to hybridize (1) */
		DYN_ATTRIBUTE (4, public, BPF, HybSinAmp);
		/** BPF (envelope-like) Parameter. Defines whether HybSinShapeW1 or HybSinShapeW2 should be used */
		DYN_ATTRIBUTE (5, public, BPF, HybSinSpectralShape);
		/** BPF (envelope-like) Parameter. Weight given to input spectral shape in relation to sound to
			hybridize spectral shape */
		DYN_ATTRIBUTE (6, public, BPF, HybSinShapeW1);
		/** BPF (envelope-like) Parameter. Weight given to sound to hybridize spectral shape in relation to 
			input sound spectral shape */
		DYN_ATTRIBUTE (7, public, BPF, HybSinShapeW2);
		/** BPF (envelope-like) Parameter. Pitch to use: 0 input, 1 sound to hybridize */
		DYN_ATTRIBUTE (8, public, BPF, HybPitch);
		/** BPF (envelope-like) Parameter. Hybridize Sinusoidal Frequency */
		DYN_ATTRIBUTE (9, public, BPF, HybSinFreq);
		/** BPF (envelope-like) Parameter. Hybridize Residual Frequency */
		DYN_ATTRIBUTE (10, public, BPF, HybResAmp);
		/** BPF (envelope-like) Parameter. Hybridize Residual Spectral Shape */
		DYN_ATTRIBUTE (11, public, BPF, HybResSpectralShape);
		/** BPF (envelope-like) Parameter. Weight for Residual Spectral Shape */
		DYN_ATTRIBUTE (12, public, BPF, HybResShapeW1 );
		DYN_ATTRIBUTE (13, public, BPF, HybResShapeW2 );
		/** Whether to use spectral Shapes or not */
		DYN_ATTRIBUTE (14, public, bool, UseSpectralShapes );
		/** Sampling rate of the input audios. WARNING: It must be the same for both audios! */
		DYN_ATTRIBUTE (15, public, TData, SamplingRate);


	protected:
		void DefaultInit();

		void DefaultValues();
	};
	
}

#endif // SMSMorphConfig.hxx

