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

#ifndef _SMSTransformation_
#define _SMSTransformation_


#include "Processing.hxx"
#include "ProcessingData.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "SpectralPeakArray.hxx"
#include "Frame.hxx"
#include "Segment.hxx"
#include "SMSTransformationConfig.hxx"

namespace CLAM {



	/** Abstract base class for all SMS Transformations. It implements all basic behaviour for
	 *	SMS Transformations such as Configuration and Control handling but defers the selection
	 *	of a particular Do overload to its template subclass SMSTransformation. 
	 *	@see SMSTransformation
	 */
	class SMSTransformation:public Processing
	{
		
		typedef InControl SMSTransformationCtrl;
	
	public:
		void AttachIn( Segment& data ){ mInput = &data; }
		void AttachOut( Segment& data ){ mOutput = &data; }
		
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

		/** Base constructor of class. Calls Configure method with a SMSTransformationConfig initialised by default*/
		SMSTransformation();

		/** Constructor with an object of SMSTransformationConfig class by parameter
		 *  @param c SMSTransformationConfig object created by the user
		*/
		SMSTransformation(const SMSTransformationConfig& c);
		
		/** Destructor of the class*/
 		virtual ~SMSTransformation(){};

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
		
		/** Internally stored configuration */
		SMSTransformationConfig mConfig;
		/** Boolean member that indicates whether BPF or single value is to be used. This is not
		 *	a control because it is not supposed to change un run-time. It is rather a configuration
		 *	parameter that can be automatically extracted from mConfig but it is placed here to make
		 *	it more explicit.
		 */
		bool mUseTemporalBPF;
	public:
		/** Control for the amount of the concrete transformation that will be applied. This control
		 *	value can be manually updated or automatically from the values in the BPF envelope-like
		 *	configuration parameter.
		 */
		SMSTransformationCtrl mAmountCtrl;
		/** Control to state whether a particular transformation is on or off. This control may be
		 *	used as a bypass when the transformation is connected in a Chain.
		 */
		SMSTransformationCtrl mOnCtrl;
		/** Input Port. Note that all SMSTransformations will have segment as input and output, 
		 *	regartheless on what particular "unwrapped" Processing Data they implement the 
		 *	transformation*/
		Segment* mInput;
		/** Output Port. Note that all SMSTransformations will have segment as input and output, 
		 *	regartheless on what particular "unwrapped" Processing Data they implement the 
		 *	transformation*/
		Segment* mOutput;

		virtual bool Do(const Frame& in,Frame& out)=0;


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
		virtual bool Do(const Segment& in, Segment& out)
		{
			CLAM_ASSERT(mInput==&in, "sms transformation chain needs input segment");
			//TODO find out why this finalization condition (and not just using size)
			while( mCurrentInputFrame<in.mCurrentFrameIndex)
			{
				if(mUseTemporalBPF)
					UpdateControlValueFromBPF(((TData)in.mCurrentFrameIndex)/in.GetnFrames());
				
				AddFramesToOutputIfInputIsLonger(mCurrentInputFrame, in, out);
				
				const Frame & inframe = in.GetFrame(mCurrentInputFrame);
				Frame & outframe = out.GetFrame(mCurrentInputFrame);

				Do( inframe, outframe );
				if(&in!=&out)
					out.mCurrentFrameIndex++;
				
				mCurrentInputFrame++;
			}
			return true;
		}

	protected:
		//TODO remove. but now is used from TimeStreach
		//! formerly corresponded to UnwrappedProcessingData
		const Frame& GetCurrentFrame(const Segment& in)
		{
			return in.GetFrame(mCurrentInputFrame);
		}
		
		//! formerly corresponded to UnwrappedProcessingData
		Frame& GetCurrentFrame( Segment& out)
		{
			if(mCurrentInputFrame==out.GetnFrames() && mInput->GetnFrames()>out.GetnFrames())
				out.AddFrame(out.GetFrame((TIndex)out.GetnFrames()-1));
			return out.GetFrame(mCurrentInputFrame);
		}

		//TODO remove. but now is used by SMSMorph
		int mCurrentInputFrame;

	};
	
};//namespace CLAM

#endif // _SMSTransformation_
