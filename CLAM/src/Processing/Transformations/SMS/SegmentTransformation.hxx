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

#ifndef _SegmentTransformation_
#define _SegmentTransformation_


#include "Processing.hxx"
#include "ProcessingData.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "OutControl.hxx"
#include "InControl.hxx"
#include "InControlPublisher.hxx"
#include "SpectralPeakArray.hxx"
#include "Frame.hxx"
#include "Segment.hxx"
#include "SegmentTransformationConfig.hxx"

#include "FrameTransformation.hxx"

namespace CLAM {

	class SegmentTransformation: public Processing
	{
		
		FrameTransformation* mFrameTransformation;

	protected:
//		OutControl mSendAmount; //TODO privatize
		/** Internally stored configuration */
		SegmentTransformationConfig mConfig;
		/** Boolean member that indicates whether BPF or single value is to be used. This is not
		 *	a control because it is not supposed to change un run-time. It is rather a configuration
		 *	parameter that can be automatically extracted from mConfig but it is placed here to make
		 *	it more explicit.
		 */
		bool mUseTemporalBPF;
		/** Control to state whether a particular transformation is on or off. This control may be
		 *	used as a bypass when the transformation is connected in a Chain.
		 */
		InControl mOnCtrl;
		
		Segment* mInput;
		Segment* mOutput;
	public:
		/** Control for the amount of the concrete transformation that will be applied. This control
		 *	value can be manually updated or automatically from the values in the BPF envelope-like
		 *	configuration parameter.
		 */
		InControlPublisher mAmountCtrl;
		

	public:
		void AttachIn( Segment& data ){ mInput = &data; }
		void AttachOut( Segment& data ){ mOutput = &data; }

		void WrapFrameTransformation( FrameTransformation*);
		
		/** Configuration change method. Note that the Amount Control is initialized from the
		 *	the values in the configuration. Appart from that the member boolean variable that
		 *	indicates whether a BPF is used is also initialized and the On/Off control also.
		 *  @param The ProcessingConfig object
		 *  @return True if the cast has been commited correctly		 
		 */
		virtual bool ConcreteConfigure(const ProcessingConfig& c);

		const ProcessingConfig& GetConfig() const
		{
			return mConfig;
		}

		/** Base constructor of class. Calls Configure method with a SegmentTransformationConfig initialised by default*/
		SegmentTransformation();

		/** Constructor with an object of SegmentTransformationConfig class by parameter
		 *  @param c SegmentTransformationConfig object created by the user
		*/
		SegmentTransformation(const SegmentTransformationConfig& c);
		
 		virtual ~SegmentTransformation();

		/** Supervised Do() function. It calls the non-supervised Do that receives Segment as
		 *	input and output.
		 */
		virtual bool DoWithSegments(void)
		{
			return Do(*mInput, *mOutput);
		}

		/** Method to update the Amount control from an existing BPF configured in the
		 *	configuration phase.
		 */
		virtual bool UpdateControlValueFromBPF(TData pos);

		/** Returns true if there are no more frames to read from input */
		virtual bool IsLastFrame();
		
		/** Overriding default method to initialize input frame counter */
		bool ConcreteStart();

	protected:


		virtual bool Do(const Frame& in,Frame& out) { return true; } 
		bool Do() { return true; } 
		const char* GetClassName() const { return "SegmentTransformation"; } 


		void AddFramesToOutputIfInputIsLonger(int frameindex, const Segment& in, Segment& out)
		{
			if(frameindex==out.GetnFrames() && in.GetnFrames()>out.GetnFrames())
					out.AddFrame(out.GetFrame((TIndex)out.GetnFrames()-1));
		}
		
		/** Unsupervised Do function, receives a Segment as input and output. This overload is
		 *	the one called from the supervised Do(). Note that if BPF parameter is used, the
		 *	Amount control is also updated.
		 *  @param input the Segment that is input to the transformation
		 *  @param out the Segment that is output from the transformation.
		 *  @return Boolean value, whether the process has finished successfully or not.
		 */
		virtual bool Do(const Segment& in, Segment& out);

		//TODO remove. but now is used from Time-Stretch
		//! formerly corresponded to UnwrappedProcessingData
		virtual const Frame& GetCurrentFrame(const Segment& in)
		{
			return in.GetFrame(mCurrentInputFrame);
		}
		
		//! formerly corresponded to UnwrappedProcessingData
		virtual Frame& GetCurrentFrame( Segment& out)
		{
			if(mCurrentInputFrame==out.GetnFrames() && mInput->GetnFrames()>out.GetnFrames())
				out.AddFrame(out.GetFrame((TIndex)out.GetnFrames()-1));
			return out.GetFrame(mCurrentInputFrame);
		}

		//TODO remove. but now is used by SegmentSMSMorph
		int mCurrentInputFrame;

	};
	
};//namespace CLAM

#endif // _SegmentTransformation_

