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

#ifndef _Frame_Interpolator_
#define _Frame_Interpolator_

#include "ProcessingComposite.hxx"
#include "DynamicType.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"
#include "Frame.hxx"
#include "SpectrumInterpolator.hxx"
#include "SpectralPeakArrayInterpolator.hxx"

namespace CLAM {


	class FrameInterpConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (FrameInterpConfig, 6,ProcessingConfig);
		DYN_ATTRIBUTE(0, public, TData, MagInterpolationFactor);
		DYN_ATTRIBUTE(1, public, TData, FreqInterpolationFactor);
		DYN_ATTRIBUTE(2, public, TData, PitchInterpolationFactor);
		DYN_ATTRIBUTE(3, public, TData, ResidualInterpolationFactor);
		DYN_ATTRIBUTE(4, public, bool, Harmonic);
		DYN_ATTRIBUTE(5, public, bool, UseSpectralShape);
	protected:
		void DefaultInit();
		void DefaultValues();

	};

/** This class performs the interpolation of two Frame processing data
 *	objects. Note that only spectral peak array for the sinusoidal component
 *	and  residual spectrum are interpolated.
 *	@todo: maybe other attributes in Frame could also be interpolated if so specified
 *	in the configuration.
 */
	class FrameInterpolator: public ProcessingComposite {
		
		typedef InControlTmpl<FrameInterpolator> FrameInterpolatorCtl;	
		
		FrameInterpConfig mConfig;


		const char *GetClassName() const {return "FrameInterpolator";}


		/** Config change method
		 * @pre argument should be an SpecInterpConfig
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

		bool ConcreteStart();

	public:
		FrameInterpolator();

		FrameInterpolator(const FrameInterpConfig &c);

		~FrameInterpolator() {};

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(const Frame& in1, const Frame& in2, Frame& out);

	
		/** Input control for interpolation factor */
		FrameInterpolatorCtl mFrameInterpolationFactorCtl;
		
		FrameInterpolatorCtl  mMagInterpolationFactorCtl;
		FrameInterpolatorCtl  mFreqInterpolationFactorCtl;
		FrameInterpolatorCtl  mPitchInterpolationFactorCtl;
		FrameInterpolatorCtl  mResidualInterpolationFactorCtl;

		/** Input controls for input pitch needed for harmonic interpolation */
		
		FrameInterpolatorCtl  mPitch1Ctl;
		FrameInterpolatorCtl  mPitch2Ctl;

		/** Input control for whether harmonic interpolation has to be performed*/
		FrameInterpolatorCtl mIsHarmonicCtl;

		/** Ports */
		InPortTmpl<Frame> mIn1;
		InPortTmpl<Frame> mIn2;
		OutPortTmpl<Frame> mOut;

		InPortTmpl<Spectrum> mSpectralShape;
	private:
		/** children processings */
		SpectrumInterpolator mPO_SpectrumInterpolator;
		SpectralPeakArrayInterpolator mPO_PeaksInterpolator;

		/** callbacks for controls */
		int DoFrameFactorControl(TData value);

		int DoMagFactorControl(TData value);
		int DoFreqFactorControl(TData value);
		int DoPitchFactorControl(TData value);
		int DoResidualFactorControl(TData value);

		int DoPitch1Control(TData value);
		int DoPitch2Control(TData value);

		int DoHarmonicControl(TData value);
	
	};

}

#endif // _Frame_Interpolator_
