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

#ifndef _SpectralPeakArray_Interpolator_
#define _SpectralPeakArray_Interpolator_

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "SpectralPeakArray.hxx"
#include "InControl.hxx"
#include "Spectrum.hxx"

namespace CLAM {


	class PeaksInterpConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (PeaksInterpConfig, 5,ProcessingConfig);
		DYN_ATTRIBUTE(0, public, TData, MagInterpolationFactor);
		DYN_ATTRIBUTE(1, public, TData, FreqInterpolationFactor);
		DYN_ATTRIBUTE(2, public, TData, PitchInterpolationFactor);
		DYN_ATTRIBUTE(3, public, bool, Harmonic);
		DYN_ATTRIBUTE(4,public, bool, UseSpectralShape);
	protected:
		void DefaultInit();
		void DefaultValues();

	};

/** This class performs the interpolation of two SpectralPeakArray processing data
 * objects.
 */
	class SpectralPeakArrayInterpolator: public Processing {
		
		typedef InControlTmpl<SpectralPeakArrayInterpolator> SpectralPeakArrayInterpolatorCtl;	
		
		PeaksInterpConfig mConfig;

		const char *GetClassName() const {return "SpectralPeakArrayInterpolator";}


		/** Config change method
		 * @pre argument should be an SpecInterpConfig
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

	public:
		SpectralPeakArrayInterpolator();

		SpectralPeakArrayInterpolator(const PeaksInterpConfig &c);

		~SpectralPeakArrayInterpolator() {};

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(const SpectralPeakArray& in1, const SpectralPeakArray& in2, SpectralPeakArray& out);
		bool Do(const SpectralPeakArray& in1, const SpectralPeakArray& in2,const Spectrum& spectralShape, SpectralPeakArray& out);
	
		/** Input control for interpolation factor */
		SpectralPeakArrayInterpolatorCtl  mMagInterpolationFactorCtl;
		SpectralPeakArrayInterpolatorCtl  mFreqInterpolationFactorCtl;
		SpectralPeakArrayInterpolatorCtl  mPitchInterpolationFactorCtl;

		/** Input controls for input pitch needed for harmonic interpolation */
		
		SpectralPeakArrayInterpolatorCtl  mPitch1Ctl;
		SpectralPeakArrayInterpolatorCtl  mPitch2Ctl;

		/** Input control for whether harmonic interpolation has to be performed*/
		SpectralPeakArrayInterpolatorCtl mIsHarmonicCtl;

		/** Ports */
		InPort<SpectralPeakArray> mIn1;
		InPort<SpectralPeakArray> mIn2;
		OutPort<SpectralPeakArray> mOut;

		Spectrum* mpSpectralShape;
		void AttachSpectralShape( Spectrum& spec ) { mpSpectralShape = &spec; }
	private:
		bool FindHarmonic(const IndexArray& indexArray,int index,int& lastPosition);

	
	};

}

#endif // _SpectralPeakArray_Interpolator_

