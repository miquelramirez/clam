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


#ifndef _SMSPitchShift_
#define _SMSPitchShift_

#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "FrameTransformation.hxx"
#include "SpectralEnvelopeExtract.hxx"
#include "SpectralEnvelopeApply.hxx"
#include "FDCombFilter.hxx"
#include "SpectralPeakArray.hxx"
#include "Frame.hxx"
#include "SMSTransformationConfig.hxx"


namespace CLAM{


	/** 
	 *	Pitch shift with timbre preservation using the SMS model. In order to preserve timbre, 
	 *	the original spectral shape is extracted and then applied back.
	 */
	class SMSPitchShift: public FrameTransformation
	{
		
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "SMSPitchShift";}

		InPort<SpectralPeakArray> mInPeaks;
		OutPort<SpectralPeakArray> mOutPeaks;
		InPort<Spectrum> mInSpectrum;
		OutPort<Spectrum> mOutSpectrum;

		InControl mIsHarmonic;
		InControl mShiftAmount;

	public:
		/** Base constructor of class. Calls Configure method with a SegmentTransformationConfig initialised by default*/
		SMSPitchShift()
			: 
			mInPeaks("In SpectralPeaks", this), 
			mOutPeaks("Out SpectralPeaks", this), 
			mInSpectrum("In Spectrum", this), 
			mOutSpectrum("Out Spectrum", this), 
			mIsHarmonic("Harmonic",this),
			mShiftAmount("Shift Amount", this)
		{
			Configure( SegmentTransformationConfig() );
			mSpectralRange=22050;//default
		}
		/** Constructor with an object of SegmentTransformationConfig class by parameter
		 *  @param c SegmentTransformationConfig object created by the user
		*/
//		SMSPitchShift(const SegmentTransformationConfig &c) : SegmentTransformation(c), mIsHarmonic("Harmonic",this)
//		{
//		}
//
		/** Destructor of the class*/
 		~SMSPitchShift()
		{}

		const ProcessingConfig& GetConfig() const { throw 0; }

	    bool ConcreteConfigure(const ProcessingConfig& c) { return true; }

		
		bool Do(const SpectralPeakArray& inpeaks,const Spectrum& inRes, SpectralPeakArray& out,Spectrum& outRes);
		bool Do(const Frame& in, Frame& out);

		// Note that overriding this method breaks the processing chain functionality. 
		bool Do()
		{
			bool result = Do(mInPeaks.GetData(), mInSpectrum.GetData(), mOutPeaks.GetData(), mOutSpectrum.GetData());
			mInPeaks.Consume();
			mOutPeaks.Produce();
			mInSpectrum.Consume();
			mOutSpectrum.Produce();
			return result;
		}

	private:
		SpectralEnvelopeExtract mSpectralEnvelopeExtract;
		SpectralEnvelopeApply mSpectralEnvelopeApply;
		FDCombFilter mFDCombFilter;
		Spectrum mSpectralEnvelope;
		TSize mSpectralRange;
	
	};		
};//namespace CLAM

#endif // _SMSPitchShift_

