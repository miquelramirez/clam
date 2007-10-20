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

#ifndef _SMSTimeStretch_
#define _SMSTimeStretch_

#include "Processing.hxx"
#include "Frame.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "SMSTimeStretchConfig.hxx"
#include "SpectrumInterpolator.hxx"
#include "SpectralPeakArrayInterpolator.hxx"

// FIXME
// #include "FrameInterpolator.hxx"

namespace CLAM{

	/**	\brief Time Stretch using the SMS model
	 *
	 *	@todo: introduce interpolation of spectral shapes, before that, interpolations may be
	 *	extracted to external Processing's
	 */
	class SMSTimeStretch: public Processing
	{

	//TODO introduce interpolation of spectral shapes, before that, interpolations may be extracted to external Processing's (this comment is also in doxygen class description)
		const char *GetClassName() const {return "SMSTimeStretch";}

		InPort<SpectralPeakArray> mInPeaks;
		OutPort<SpectralPeakArray> mOutPeaks;
		InPort<Fundamental> mInFund;
		OutPort<Fundamental> mOutFund;
		InPort<Spectrum> mInSpectrum;
		OutPort<Spectrum> mOutSpectrum;


	public:
		SMSTimeStretch()
			:
			mInPeaks("In SpectralPeaks", this),
			mOutPeaks("Out SpectralPeaks", this),
			mInFund("In Fundamental", this),
			mOutFund("Out Fundamental", this),
			mInSpectrum("In Spectrum", this),
			mOutSpectrum("Out Spectrum", this)
		{
			Configure( mConfig );
		}

 		~SMSTimeStretch() {}

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
			Spectrum& outSpectrum
			);


//FIXME
		bool ConcreteStart();
		bool ConcreteStop();

// 		bool CanProcessInplace() {return false;}
// 		bool IsLastFrame();
// 		
// 		Frame& GetCurrentFrame(Segment& out);
// 		const Frame& GetCurrentFrame(const Segment& in);
// 		

// 		InControl mFactor;

		typedef SMSTimeStretchConfig Config;

		const ProcessingConfig & GetConfig() const
		{
			return mConfig;
		}

	protected:
		bool ConcreteConfigure(const ProcessingConfig& config);

	private:
		Config mConfig;

	protected:
// 		bool HaveFinished();
// 		void UpdateTimeAndIndex(const Segment& in);
		
		TTime mSynthesisTime; ///<
		TTime mAnalysisTime;
		TIndex mnSynthesisFrames; ///<
		
		Frame mLeftFrame;

		/** Child processings **/
// 		FrameInterpolator mFrameInterpolator; // FIXME
		SpectrumInterpolator mSpectrumInterpolator;
		SpectralPeakArrayInterpolator mPeaksInterpolator;
	};
};//namespace CLAM

#endif // _SMSTimeStretch_
