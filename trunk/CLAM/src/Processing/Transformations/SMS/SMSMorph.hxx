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


#ifndef _SMSMorph_
#define _SMSMorph_

#include "SMSTransformation.hxx"
#include "SDIFIn.hxx"

namespace CLAM{


	class SMSMorphConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SMSMorphConfig, 15,ProcessingConfig);
		/** Name of the SMSTransformation object*/
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/** FileName where pre-analyzed sound to morph is to be found */
		DYN_ATTRIBUTE (1, public, std::string, FileName);
		/** BPF (envelope-like) Parameter. Defines how much of each sound is being used from 0 to 1  */
		DYN_ATTRIBUTE (2, public, BPF, HybBPF);
		/** boolean Parameter. Defines whether intermediate frames should be interpolated from
			existing ones. */
		DYN_ATTRIBUTE (3, public, bool, InterpolateFrame);
		/** BPF (envelope-like) Parameter. Defines temporal relation between input sound and sound
			to hybridize */
		DYN_ATTRIBUTE (4, public, BPF, SynchronizeTime);
		/** BPF (envelope-like) Parameter. Amount of amplitude of sinusoids to take from input sound (0)
			or sound to hybridize (1) */
		DYN_ATTRIBUTE (5, public, BPF, HybSinAmp);
		/** BPF (envelope-like) Parameter. Defines whether HybSinShapeW1 or HybSinShapeW2 should be used */
		DYN_ATTRIBUTE (6, public, BPF, HybSinSpectralShape);
		/** BPF (envelope-like) Parameter. Weight given to input spectral shape in relation to sound to
			hybridize spectral shape */
		DYN_ATTRIBUTE (7, public, BPF, HybSinShapeW1);
		/** BPF (envelope-like) Parameter. Weight given to sound to hybridize spectral shape in relation to 
			input sound spectral shape */
		DYN_ATTRIBUTE (8, public, BPF, HybSinShapeW2);
		/** BPF (envelope-like) Parameter. Pitch to use: 0 input, 1 sound to hybridize */
		DYN_ATTRIBUTE (9, public, BPF, HybPitch);
		/** BPF (envelope-like) Parameter. Hybridize Sinusoidal Frequency */
		DYN_ATTRIBUTE (10, public, BPF, HybSinFreq);
		/** BPF (envelope-like) Parameter. Hybridize Residual Frequency */
		DYN_ATTRIBUTE (11, public, BPF, HybResAmp);
		/** BPF (envelope-like) Parameter. Hybridize Residual Spectral Shape */
		DYN_ATTRIBUTE (12, public, BPF, HybResSpectralShape);
		/** BPF (envelope-like) Parameter. Weight for Residual Spectral Shape */
		DYN_ATTRIBUTE (13, public, BPF, HybResShapeW1);
		/** BPF (envelope-like) Parameter. What residual phase to take, input sound or sound to hybridize */
		DYN_ATTRIBUTE (14, public, BPF, HybResPhase);
		


	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			DefaultValues();
		}

		void DefaultValues()
		{
			BPF tmpBPF(2);
			tmpBPF.SetValue(0,0);
			tmpBPF.SetXValue(0,0);
			tmpBPF.SetValue(1,0);
			tmpBPF.SetXValue(1,1);
			SetHybBPF(tmpBPF);
		}
	public:
		~SMSMorphConfig(){}
	};
	
	/////TODO: Don't know if it has to derive from Transformation or Processing
	class SMSMorph: public Processing
	{
		
		typedef InControlTmpl<SMSMorph> SMSMorphCtrl;	
	
	public:
		/** Base constructor of class. Calls Configure method with a SMSTransformationConfig initialised by default*/
		SMSMorph();
		/** Constructor with an object of SMSTransformationConfig class by parameter
		 *  @param c SMSTransformationConfig object created by the user
		*/
		SMSMorph(const SMSMorphConfig &c);
		
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "SMSMorph";}

		const ProcessingConfig& GetConfig() const
		{
			return mConfig;
		}

		virtual bool ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast);
		
		/** Destructor of the class*/
 		~SMSMorph()
		{}

		
		bool Do();
		bool Do(const Segment& in1, Segment& out);
		bool Do(const Segment& in1,const Segment& in2, Segment& out);
		
		virtual bool UpdateControlValueFromBPF(TData pos);
	
		SMSMorphCtrl  mAmountCtrl;
		
		SMSMorphCtrl  mHybBPF;
		SMSMorphCtrl  mSynchronizeTime;
		SMSMorphCtrl  mHybSinAmp;
		SMSMorphCtrl  mHybSinSpectralShape;
		SMSMorphCtrl  mHybSinShapeW1;
		SMSMorphCtrl  mHybSinShapeW2;
		SMSMorphCtrl  mHybPitch;
		SMSMorphCtrl  mHybSinFreq;
		SMSMorphCtrl  mHybResAmp;
		SMSMorphCtrl  mHybResSpectralShape;
		SMSMorphCtrl  mHybResShapeW;
		SMSMorphCtrl  mHybResPhase;
	protected:
		
		void UpdateMorphFactors();
		
		bool MorphFrames(const Frame& f1, const Frame& f2, Frame& fout);
		
		bool ResidualMorph(const Frame& in1,const Frame& in2, Frame& out);
		bool SinusoidalMorph(const Frame& in1,const Frame& in2, Frame& out);

		bool InterpolateSpectralPeaks(const SpectralPeakArray& in1,const SpectralPeakArray& in2, SpectralPeakArray& out, TData pitch1, TData pitch2);
				
		bool FindHarmonic(const IndexArray& indexArray,int index,int& lastPosition);

		bool LoadSDIF( std::string fileName, Segment& segment );
		
		SDIFIn mSDIFReader;
		SMSMorphConfig mConfig;
		

		/** Input Port. Note that all SMSTransformations will have segment as input and output, 
		 *	regartheless on what particular "unwrapped" Processing Data they implement the 
		 *	transformation*/
		InPortTmpl<Segment> mInput1;
		InPortTmpl<Segment> mInput2;
		/** Output Port. Note that all SMSTransformations will have segment as input and output, 
		 *	regartheless on what particular "unwrapped" Processing Data they implement the 
		 *	transformation*/
		OutPortTmpl<Segment> mOutput;

		TData mPitch1;
		TData mPitch2;
		TData mNewPitch;
		bool mHarmonicMorph;
		bool mHarmSpectralShapeMorph;
		
		//Morph Factors
		TData mPitchFactor;
		TData mMagFactor;
		TData mFreqFactor;
		TData mFrameFactor;
		TData mResMagFactor;

		bool mHaveInternalSegment;

		Segment mSegment;
	};		
};//namespace CLAM

#endif // _SMSMorph_

