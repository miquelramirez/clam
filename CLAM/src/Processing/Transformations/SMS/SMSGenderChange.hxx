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


#ifndef _SMSGenderChange_
#define _SMSGenderChange_

#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "FrameTransformation.hxx"
#include "SMSPitchShift.hxx"
#include "SMSSpectralShapeShift.hxx"
#include "SpectralPeakArray.hxx"
#include "FrameTransformationConfig.hxx"

namespace CLAM{


	class SMSGenderChange: public FrameTransformation
	{
		
		const char *GetClassName() const {return "SMSGenderChange";}

		InPort<SpectralPeakArray> mInPeaks;
		OutPort<SpectralPeakArray> mOutPeaks;
		InPort<Fundamental> mInFund;
		OutPort<Fundamental> mOutFund;
		InPort<Spectrum> mInSpectrum;
		OutPort<Spectrum> mOutSpectrum;

		FloatInControl mControl;
	public:

		SMSGenderChange()
		:
		mInPeaks("In SpectralPeaks", this),	
		mOutPeaks("Out SpectralPeaks", this),
		mInFund("In Fundamental", this),
		mOutFund("Out Fundamental", this),
		mInSpectrum("In Spectrum", this),
		mOutSpectrum("Out Spectrum", this),
		mControl("Control", this)
		{
			Configure( SegmentTransformationConfig() );
		}

 		~SMSGenderChange() {}

		bool ConcreteConfigure( const ProcessingConfig& config )
		{
			mControl.SetBounds(0,1);
			mControl.DoControl(0);
			return true;
		}

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
			mInFund.Consume(); 
			mInSpectrum.Consume(); 
			mOutPeaks.Produce(); 
			mOutFund.Produce();
			mOutSpectrum.Produce();
			return result;
		}
		
		bool Do(const Frame& in, Frame& out);

		bool Do(const SpectralPeakArray& inPeaks,
				const Fundamental& inFund,
				const Spectrum& inSpectrum,
				SpectralPeakArray& outPeaks,
				Fundamental& outFund,
				Spectrum& outSpectrum);

	private:

		SMSPitchShift mPitchShift;
		SMSSpectralShapeShift mSpectralShapeShift;
	};		
}	//namespace CLAM

#endif // _SMSGenderChange_

