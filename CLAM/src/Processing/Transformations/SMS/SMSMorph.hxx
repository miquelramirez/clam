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
#include "SMSMorphConfig.hxx"
#include "SDIFIn.hxx"
#include "FrameInterpolator.hxx"

namespace CLAM{


	
	/** @todo: introduce interpolation of spectral shapes, before that, interpolations may be
	 *	extracted to external Processing's*/
	class SMSMorph: public SMSTransformationTmpl<Frame>
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

		bool ConcreteConfigure(const ProcessingConfig& c)
			throw(std::bad_cast);
		bool ConcreteStart();
		
		/** Destructor of the class*/
 		~SMSMorph()
		{}

		
		bool Do(const Segment& in1, Segment& out);
		bool Do(const Segment& in1,Segment& in2, Segment& out);
		bool Do(const Frame& in1, Frame& out);
		
		virtual bool UpdateControlValueFromBPF(TData pos);
	
		
		bool mUseGlobalFactor;
		SMSMorphCtrl  mHybBPF;

		bool mUseSynchronizeTime;
		SMSMorphCtrl  mSynchronizeTime;
		
		bool mUseSinAmp;
		SMSMorphCtrl  mHybSinAmp;

		bool mUseSinSpectralShape;
		SMSMorphCtrl  mHybSinSpectralShape;
		SMSMorphCtrl  mHybSinShapeW1;
		SMSMorphCtrl  mHybSinShapeW2;
		
		bool mUsePitch;
		SMSMorphCtrl  mHybPitch;
		
		bool mUseSinFreq;
		SMSMorphCtrl  mHybSinFreq;
		
		bool mUseResAmp;
		SMSMorphCtrl  mHybResAmp;
		
		bool mUseResSpectralShape;
		SMSMorphCtrl  mHybResSpectralShape;
		SMSMorphCtrl  mHybResShapeW1;
		SMSMorphCtrl  mHybResShapeW2;
						
	protected:
		
		void UpdateFrameInterpolatorFactors(bool useFrameFactor);
		bool FindInterpolatedFrameFromSegment2Morph(Frame& interpolatedFrame);
		void UpdateSpectralShape(const BPF& weightBPF1, const BPF& weightBPF2, TData interpFactor, Spectrum& spectralShape);
		void InitializeFactorsToUse();

		bool LoadSDIF( std::string fileName, Segment& segment );
		
		SDIFIn mSDIFReader;
		SMSMorphConfig mConfig;
		

		/** Input Port. Note that all SMSTransformations will have segment as input and output, 
		 *	regartheless on what particular "unwrapped" Processing Data they implement the 
		 *	transformation. Here we add a second segment for the sound to morph.*/
		InPortTmpl<Segment> mInput2;
		
		bool mHarmSpectralShapeMorph;
		bool mHaveInternalSegment;

		Segment mSegment;

		Spectrum mSpectralShape;
		Spectrum mResSpectralShape;


		/** Child processings **/

		FrameInterpolator mPO_FrameInterpolator;
	};		
};//namespace CLAM

#endif // _SMSMorph_

