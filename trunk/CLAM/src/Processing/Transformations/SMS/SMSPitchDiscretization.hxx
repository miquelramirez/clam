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


#ifndef _SMSPitchDiscretization_
#define _SMSPitchDiscretization_

#include "InPort.hxx"
#include "OutPort.hxx"
#include "SMSPitchShift.hxx"
#include "SpectrumAdder2.hxx"
#include "FrameTransformation.hxx"
#include "FrameTransformationConfig.hxx"


namespace CLAM{


	class SMSPitchDiscretization: public FrameTransformation
	{
		InPort<SpectralPeakArray> mInPeaks;
		InPort<Fundamental> mInFund;
		InPort<Spectrum> mInSpectrum;
		OutPort<SpectralPeakArray> mOutPeaks;
		OutPort<Fundamental> mOutFund;
		OutPort<Spectrum> mOutSpectrum;

		SMSPitchShift mPitchShift;
		
	public:

		SMSPitchDiscretization()
		:
		mInPeaks("In SpectralPeaks", this),
		mInFund("In Fundamental", this),
		mInSpectrum("In Spectrum", this),
		mOutPeaks("Out SpectralPeaks", this),
		mOutFund("Out Fundamental", this),
		mOutSpectrum("Out Spectrum", this)
		{
			Configure( FrameTransformationConfig() );
		}

 		~SMSPitchDiscretization() {}

		bool Do()
		{	
			bool result = Do(mInPeaks.GetData(),
					 mInFund.GetData(),
					 mInSpectrum.GetData(),
					 mOutPeaks.GetData(),
					 mOutFund.GetData(),
					 mOutSpectrum.GetData()
					 );

			mInPeaks.Consume();
			mOutPeaks.Produce();
			mInFund.Consume();
			mOutFund.Produce();
			mInSpectrum.Consume();
			mOutSpectrum.Produce();

			return result;
		}

		bool Do(const Frame& in, Frame& out);

		bool Do(const SpectralPeakArray& inPeaks,
			const Fundamental& inFund,
			const Spectrum& inSpectrum,
			SpectralPeakArray& outPeaks,
			Fundamental& outFund,
			Spectrum& outSpectrum
			);

	private:
		const char *GetClassName() const {return "SMSPitchDiscretization";}
		
	};
} //namespace CLAM

#endif // _SMSPitchDiscretization_

