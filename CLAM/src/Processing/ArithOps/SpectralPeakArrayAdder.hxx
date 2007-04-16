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

#ifndef _SpectralPeakArray_Adder_
#define _SpectralPeakArray_Adder_

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "SpectralPeakArray.hxx"
#include "InControl.hxx"
#include "Spectrum.hxx"

namespace CLAM {


	class PeaksAddConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (PeaksAddConfig, 1,ProcessingConfig);
		//TODO: As a matter of fact I wouldn't even need this attributte
		DYN_ATTRIBUTE(0, public, std::string, Name);
		
	};

/** This class performs the interpolation of two SpectralPeakArray processing data
 * objects.
 */
	class SpectralPeakArrayAdder: public Processing {
		
		typedef InControlTmpl<SpectralPeakArrayAdder> SpectralPeakArrayAdderCtl;	
		
		PeaksAddConfig mConfig;

		const char *GetClassName() const {return "SpectralPeakArrayAdder";}


		/** Config change method
		 * @pre argument should be an SpecInterpConfig
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

	public:
		SpectralPeakArrayAdder();

		SpectralPeakArrayAdder(const PeaksAddConfig &c);

		~SpectralPeakArrayAdder() {};

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void) {return false;}

		bool Do(const SpectralPeakArray& in1, const SpectralPeakArray& in2, SpectralPeakArray& out);

		/** Ports */
		InPort<SpectralPeakArray> mIn1;
		InPort<SpectralPeakArray> mIn2;
		OutPort<SpectralPeakArray> mOut;

	
	};

}

#endif // _SpectralPeakArray_Adder_

