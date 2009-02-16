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

#ifndef _Frame_Interpolator_
#define _Frame_Interpolator_

#include "ProcessingComposite.hxx"
#include "DynamicType.hxx"
#include "Frame.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
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

/**	
 *	\brief This class performs the interpolation of two Frame processing data objects.
 *
 *	Note that only spectral peak array for the sinusoidal component and  residual
 *	spectrum are interpolated.
 *	@todo: maybe other attributes in Frame could also be interpolated if so specified
 *	in the configuration.
 */
	class FrameInterpolator: public ProcessingComposite {
		typedef FrameInterpConfig Config;
		Config mConfig;


		const char *GetClassName() const {return "FrameInterpolator";}


		/** Config change method
		 * @pre argument should be an SpecInterpConfig
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

		void AttachChildren();

	public:
		FrameInterpolator(const FrameInterpConfig &c=Config());

		~FrameInterpolator() {};

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(const Frame& in1, const Frame& in2, Frame& out);

	
		/** Input control for interpolation factor */
		FloatInControl mFrameInterpolationFactorCtl;
		
		FloatInControl  mMagInterpolationFactorCtl;
		FloatInControl  mFreqInterpolationFactorCtl;
		FloatInControl  mPitchInterpolationFactorCtl;
		FloatInControl  mResidualInterpolationFactorCtl;

		/** Input controls for input pitch needed for harmonic interpolation */
		
		FloatInControl  mPitch1Ctl;
		FloatInControl  mPitch2Ctl;

		/** Input control for whether harmonic interpolation has to be performed*/
		FloatInControl mIsHarmonicCtl;

		/** Ports */
		InPort<Frame> mIn1;
		InPort<Frame> mIn2;
		OutPort<Frame> mOut;

		void AttachSpectralShape(Spectrum& spec) { mpSpectralShape = &spec; }
		
		Spectrum* mpSpectralShape;
	private:
		/** children processings */
		SpectrumInterpolator mPO_SpectrumInterpolator;
		SpectralPeakArrayInterpolator mPO_PeaksInterpolator;

		/** callbacks for controls */
		void DoFrameFactorControl(TControlData value);

		void DoMagFactorControl(TControlData value);
		void DoFreqFactorControl(TControlData value);
		void DoPitchFactorControl(TControlData value);
		void DoResidualFactorControl(TControlData value);

		void DoPitch1Control(TControlData value);
		void DoPitch2Control(TControlData value);

		void DoHarmonicControl(TControlData value);
	
	};

}

#endif // _Frame_Interpolator_

