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

#ifndef _Frame_Adder_
#define _Frame_Adder_

#include "ProcessingComposite.hxx"
#include "DynamicType.hxx"
#include "Frame.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "SpectrumAdder2.hxx"
#include "SpectralPeakArrayAdder.hxx"

namespace CLAM {


	class FrameAdderConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (FrameAdderConfig, 0,ProcessingConfig);
	};

/** This class performs the interpolation of two Frame processing data
 *	objects. Note that only spectral peak array for the sinusoidal component
 *	and  residual spectrum are interpolated.
 *	@todo: maybe other attributes in Frame could also be interpolated if so specified
 *	in the configuration.
 */
	class FrameAdder: public ProcessingComposite {
		
		FrameAdderConfig mConfig;


		const char *GetClassName() const {return "FrameAdder";}


		/** Config change method
		 * @pre argument should be an SpecInterpConfig
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

		void AttachChildren();

	public:
		FrameAdder();

		FrameAdder(const FrameAdderConfig &c);

		~FrameAdder() {};

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(const Frame& in1, const Frame& in2, Frame& out);

	

		/** Ports */
		InPort<Frame> mIn1;
		InPort<Frame> mIn2;
		OutPort<Frame> mOut;

	private:
		/** children processings */
		SpectrumAdder2 mPO_SpectrumAdder;
		SpectralPeakArrayAdder mPO_PeaksAdder;

	};

}

#endif // _Frame_Adder_

