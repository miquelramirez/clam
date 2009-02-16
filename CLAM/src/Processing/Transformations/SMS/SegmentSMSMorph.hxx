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


#ifndef _SEGMENTSMSMORPH_
#define _SEGMENTSMSMORPH_

#include "SegmentTransformation.hxx"
#include "SegmentSMSMorphConfig.hxx"
#include "SDIFIn.hxx"
#include "FrameInterpolator.hxx"

namespace CLAM{


	
	/** @todo: introduce interpolation of spectral shapes, before that, interpolations may be
	 *	extracted to external Processing's*/
	class SegmentSMSMorph: public SegmentTransformation
	{
		
	public:
		/** Base constructor of class. Calls Configure method with a SegmentTransformationConfig initialised by default*/
		SegmentSMSMorph();
		/** Constructor with an object of SegmentTransformationConfig class by parameter
		 *  @param c SegmentTransformationConfig object created by the user
		*/
		SegmentSMSMorph(const SegmentSMSMorphConfig &c);
		
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "SegmentSMSMorph";}

		const ProcessingConfig& GetConfig() const
		{
			return mConfig;
		}

		bool ConcreteConfigure(const ProcessingConfig& c);
		bool ConcreteStart();
		
		/** Destructor of the class*/
 		~SegmentSMSMorph()
		{}

		
		bool Do(const Segment& in1, Segment& out);
		bool Do(const Segment& in1,Segment& in2, Segment& out);
		bool Do(const Frame& in1, Frame& out);
		bool Do()
		{
			CLAM_ASSERT(false, "Do with ports not implemented");
			return false;
		}
	
		virtual bool UpdateControlValueFromBPF(TData pos);
	
		void SetSegmentToMorph(Segment&segmentToMorph);

		bool mUseGlobalFactor;
		FloatInControl  mHybBPF;

		bool mUseSynchronizeTime;
		FloatInControl  mSynchronizeTime;
		
		bool mUseSinAmp;
		FloatInControl  mHybSinAmp;

		bool mUseSinSpectralShape;
		FloatInControl  mHybSinSpectralShape;
		FloatInControl  mHybSinShapeW1;
		FloatInControl  mHybSinShapeW2;
		
		bool mUsePitch;
		FloatInControl  mHybPitch;
		
		bool mUseSinFreq;
		FloatInControl  mHybSinFreq;
		
		bool mUseResAmp;
		FloatInControl  mHybResAmp;
		
		bool mUseResSpectralShape;
		FloatInControl  mHybResSpectralShape;
		FloatInControl  mHybResShapeW1;
		FloatInControl  mHybResShapeW2;
						
	protected:
		
		void UpdateFrameInterpolatorFactors(bool useFrameFactor);
		bool FindInterpolatedFrameFromSegment2Morph(Frame& interpolatedFrame);
		void UpdateSpectralShape(const BPF& weightBPF1, const BPF& weightBPF2, TData interpFactor, Spectrum& spectralShape);
		void InitializeFactorsToUse();

		bool LoadSDIF( std::string fileName, Segment& segment );
		
		SDIFIn mSDIFReader;
		SegmentSMSMorphConfig mConfig;
		

		/** Input Port. Note that all SegmentTransformations will have segment as input and output, 
		 *	regartheless on what particular "unwrapped" Processing Data they implement the 
		 *	transformation. Here we add a second segment for the sound to morph.*/
		Segment* mpInput2;
		
		bool mHarmSpectralShapeMorph;
		bool mHaveInternalSegment;

		Segment mSegment;

		Spectrum mSpectralShape;
		Spectrum mResSpectralShape;


		/** Child processings **/

		FrameInterpolator mPO_FrameInterpolator;
	};		
};//namespace CLAM

#endif // _SegmentSMSMorph_

