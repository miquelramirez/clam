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

#ifndef _SMSMorph_
#define _SMSMorph_

#include "SMSMorphConfig.hxx"

#include "Processing.hxx"
#include "Frame.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "SpectrumInterpolator.hxx"
#include "SpectralPeakArrayInterpolator.hxx"


namespace CLAM{

	/**	\brief Morph using the SMS model
	 *
	 *	Allows to 'morph' or hybridize two sounds, so the resulting one has intermediate characteristics
	 */
	class SMSMorph: public Processing
	{
		const char *GetClassName() const {return "SMSMorph";}

		InPort<SpectralPeakArray> mInPeaks1;
		InPort<Fundamental> mInFund1;
		InPort<Spectrum> mInSpectrum1;

		InPort<SpectralPeakArray> mInPeaks2;
		InPort<Fundamental> mInFund2;
		InPort<Spectrum> mInSpectrum2;

		//TODO add many sources morph option? (with configurable amount)

		OutPort<SpectralPeakArray> mOutPeaks;
		OutPort<Fundamental> mOutFund;
		OutPort<Spectrum> mOutSpectrum;

		/**  An interpolation factor of 0.0 means that the resulting sound sinusoidal envelope will match the source's one, and 
		*   factor of 1.0 means that the resulting sound sinusoidal envelope matches exactly the target's one.
		*/
		FloatInControl mInterpolationFactor; 

	public:
		SMSMorph()
			:
			mInPeaks1("In SpectralPeaks 1", this),
			mInFund1("In Fundamental 1", this),
			mInSpectrum1("In Spectrum 1", this),
		
			mInPeaks2("In SpectralPeaks 2", this),
			mInFund2("In Fundamental 2", this),
			mInSpectrum2("In Spectrum 2", this),
		
			mOutPeaks("Out SpectralPeaks", this),
			mOutFund("Out Fundamental", this),
			mOutSpectrum("Out Spectrum", this),

			mInterpolationFactor("Interpolation Factor", this)
		{
			Configure( mConfig );
		}

 		~SMSMorph() {}

		bool Do()
		{
			bool result = Do(mInPeaks1.GetData(),
					 mInFund1.GetData(),
					 mInSpectrum1.GetData(),
					 mInPeaks2.GetData(),
					 mInFund2.GetData(),
					 mInSpectrum2.GetData(),
					 mOutPeaks.GetData(),
					 mOutFund.GetData(),
					 mOutSpectrum.GetData()
					 );

			mInPeaks1.Consume();
			mInFund1.Consume();
			mInSpectrum1.Consume();

			mInPeaks2.Consume();
			mInFund2.Consume();
			mInSpectrum2.Consume();

			mOutPeaks.Produce();
			mOutFund.Produce();
			mOutSpectrum.Produce();

			return result;
		}

		bool Do(const SpectralPeakArray& inPeaks1,
			const Fundamental& inFund1,
			const Spectrum& inSpectrum1,
			const SpectralPeakArray& inPeaks2,
			const Fundamental& inFund2,
			const Spectrum& inSpectrum2,
			SpectralPeakArray& outPeaks,
			Fundamental& outFund,
			Spectrum& outSpectrum
			);

		//TODO - check if it's still useful
		bool Do(const Frame& in1, const Frame& in2, Frame& out);

		typedef SMSMorphConfig Config;

		const ProcessingConfig& GetConfig() const
		{
			return mConfig;
		}

	private:
		Config mConfig;

	protected:
		bool ConcreteConfigure(const ProcessingConfig& c);


		/** Child processings **/
		SpectrumInterpolator mSpectrumInterpolator;
		SpectralPeakArrayInterpolator mPeaksInterpolator;

	};		
};//namespace CLAM

#endif // _SMSMorph_

