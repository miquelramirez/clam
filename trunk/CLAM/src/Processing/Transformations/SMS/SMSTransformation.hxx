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

#ifndef _SMSTransformation_
#define _SMSTransformation_


#include "Processing.hxx"
#include "ProcessingData.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"

#include "BPF.hxx"

#include "SpectralPeakArray.hxx"
#include "Frame.hxx"
#include "Segment.hxx"

namespace CLAM {


	/** Configuration class for all SMSTransformations. It includes a float-like single value 
	 *	parameter and a BPF envelope-like parameter. Either one of these may be used to initialize 
	 *	and update the value control in an SMSTransformation.
	 */
	class SMSTransformationConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SMSTransformationConfig, 4,ProcessingConfig);
		/** Name of the SMSTransformation object*/
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/** Type of transformation, for the time being just a string, should become
		* an enumeration of known transformation types??*/
		DYN_ATTRIBUTE (1, public, std::string, Type);
		/** Single Value Parameter */
		DYN_ATTRIBUTE (2, public, TData, Amount);
		/** BPF (envelope-like) Parameter */
		DYN_ATTRIBUTE (3, public, BPF, BPFAmount);


	protected:
		/** The single-value Amount and the On configuration are added by default. 
		 *	Should you need the BPF, it must be added explicitly
		 */
		void DefaultInit()
		{
			AddAmount();
			UpdateData();
			DefaultValues();
		}

		/** By default, the amount is set to 0 and the On parameter to true.*/
		void DefaultValues()
		{
			SetAmount(0);
		}
	public:
		virtual ~SMSTransformationConfig(){}
	};

	/** Abstract base class for all SMS Transformations. It implements all basic behaviour for
	 *	SMS Transformations such as Configuration and Control handling but defers the selection
	 *	of a particular Do overload to its template subclass SMSTransformationTmpl. 
	 *	@see SMSTransformationTmpl
	 */
	class SMSTransformation:public Processing
	{
		
		typedef InControlTmpl<SMSTransformation> SMSTransformationCtrl;
	
	public:
		/** Configuration change method. Note that the Amount Control is initialized from the
		 *	the values in the configuration. Appart from that the member boolean variable that
		 *	indicates whether a BPF is used is also initialized and the On/Off control also.
		 *  @param The ProcessingConfig object
		 *  @return True if the cast has been commited correctly		 
		 */
		virtual bool ConcreteConfigure(const ProcessingConfig& c)
		{
			CopyAsConcreteConfig(mConfig, c);
			mUseTemporalBPF=false;
			if(mConfig.HasAmount())
				mAmountCtrl.DoControl(mConfig.GetAmount());
			else if(mConfig.HasBPFAmount()){
				mAmountCtrl.DoControl(mConfig.GetBPFAmount().GetValue(0));
				mUseTemporalBPF=true;}
			else
				mAmountCtrl.DoControl(0);
			return true;
		}
		
		const ProcessingConfig& GetConfig() const
		{
			return mConfig;
		}

		/** Base constructor of class. Calls Configure method with a SMSTransformationConfig initialised by default*/
		SMSTransformation():mAmountCtrl("Amount",this),mOnCtrl("On",this),mInput("Input",this,1),mOutput("Output",this,1)
		{
		}

		/** Constructor with an object of SMSTransformationConfig class by parameter
		 *  @param c SMSTransformationConfig object created by the user
		*/
		SMSTransformation(const SMSTransformationConfig& c):mAmountCtrl("Amount",this),mOnCtrl("On",this),mInput("Input",this,1),mOutput("Output",this,1)
		{
			Configure(c);
		}
		
		/** Destructor of the class*/
 		virtual ~SMSTransformation(){};

		/** Supervised Do() function. It calls the non-supervised Do that receives Segment as
		 *	input and output.
		 */
		bool Do(void)
		{
			return Do(mInput.GetData(),mOutput.GetData());
		}

		/** Pure virtual method that is implemented in the template subclass.
		 *	@see SMSTransformationTmpl::Do(const Segment& in, Segment& out) */
		virtual bool Do(const Segment& in, Segment& out)=0;
		
		/** Method to update the Amount control from an existing BPF configured in the
		 *	configuration phase.
		 */
		bool UpdateControlValueFromBPF(TData pos)
		{
			if(mConfig.HasBPFAmount())
			{
				mAmountCtrl.DoControl(mConfig.GetBPFAmount().GetValue(pos));
				return true;
			}
			else return false;
		}
	
	protected:

/**@TODO: The UnwrapProcessingData methods could possibly be moved to a more
 *	generic place, like the Segment class (becoming a friend operation?). */
		
		/** Particular method for unwrapping a Frame from a given Segment
		 *	@return: current Frame in the Segment returned as a constant reference.
		 *	@param in: input Segment (constant reference)
		 *	@param Frame*: just used as the selector that indicates that this
		 *	overload is to be used
		 */
		const Frame& UnwrapProcessingData(const Segment& in,Frame*)
		{
			return in.GetFrame(in.mCurrentFrameIndex);
		}
		/** Particular method for unwrapping a Frame from a given Segment
		 *	@return: current Frame in the Segment returned as a non-constant reference.
		 *	@param in: input Segment (non- constant reference)
		 *	@param Frame*: just used as the selector that indicates that this
		 *	overload is to be used
		 */
		Frame& UnwrapProcessingData(Segment& out,Frame*)
		{
			return out.GetFrame(out.mCurrentFrameIndex);
		}

		/** Particular method for unwrapping an Audio from a given Segment
		 *	@return: AudioFrame in current Frame in the Segment returned as a constant reference.
		 *	@param in: input Segment (constant reference)
		 *	@param Audio*: just used as the selector that indicates that this
		 *	overload is to be used
		 */
		const Audio& UnwrapProcessingData(const Segment& in,Audio*)
		{
			return in.GetFrame(in.mCurrentFrameIndex).GetAudioFrame();
		}
		/** Particular method for unwrapping an Audio from a given Segment
		 *	@return: AudioFrame in current Frame in the Segment returned as a non-constant reference.
		 *	@param in: input Segment (non-constant reference)
		 *	@param Audio*: just used as the selector that indicates that this
		 *	overload is to be used
		 */
		Audio& UnwrapProcessingData(Segment& out,Audio*)
		{
			return out.GetFrame(out.mCurrentFrameIndex).GetAudioFrame();
		}
		/** Particular method for unwrapping a Spectrum from a given Segment
		 *	@return: Residual Spectrum in current Frame in the Segment returned as a constant reference.
		 *	@param in: input Segment (constant reference)
		 *	@param Spectrum*: just used as the selector that indicates that this
		 *	overload is to be used
		 */
		const Spectrum& UnwrapProcessingData(const Segment& in,Spectrum*)
		{
			return in.GetFrame(in.mCurrentFrameIndex).GetResidualSpec();
		}
		/** Particular method for unwrapping a Spectrum from a given Segment
		 *	@return: Residual Spectrum in current Frame in the Segment returned as a non-constant reference.
		 *	@param in: input Segment (non-constant reference)
		 *	@param Spectrum*: just used as the selector that indicates that this
		 *	overload is to be used
		 */
		Spectrum& UnwrapProcessingData(Segment& out,Spectrum*)
		{
			return out.GetFrame(out.mCurrentFrameIndex).GetResidualSpec();
		}
		/** Particular method for unwrapping a SpectralPeakArray from a given Segment
		 *	@return: SpectralPeakArray in current Frame in the Segment returned as a constant reference.
		 *	@param in: input Segment (constant reference)
		 *	@param SpectralPeakArray*: just used as the selector that indicates that this
		 *	overload is to be used
		 */
		const SpectralPeakArray& UnwrapProcessingData(const Segment& in,SpectralPeakArray*)
		{
			return in.GetFrame(in.mCurrentFrameIndex).GetSpectralPeakArray();
		}
		/** Particular method for unwrapping a SpectralPeakArray from a given Segment
		 *	@return: SpectralPeakArray in current Frame in the Segment returned as a non-constant reference.
		 *	@param in: input Segment (non-constant reference)
		 *	@param SpectralPeakArray*: just used as the selector that indicates that this
		 *	overload is to be used
		 */
		SpectralPeakArray& UnwrapProcessingData(Segment& out,SpectralPeakArray*)
		{
			return out.GetFrame(out.mCurrentFrameIndex).GetSpectralPeakArray();
		}

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
		InPortTmpl<Segment> mInput;
		/** Output Port. Note that all SMSTransformations will have segment as input and output, 
		 *	regartheless on what particular "unwrapped" Processing Data they implement the 
		 *	transformation*/
		OutPortTmpl<Segment> mOutput;
	};
	
	/** Template SMS Transformation abstract class. It derives from also abstract SMSTransformation
	 *	class and adds behaviour for a particular Processing Data (Frame, Audio, Spectrum or 
	 *	SpectralPeakArray).
	 */
	template <class UnwrappedProcessingData>
	class SMSTransformationTmpl: public SMSTransformation
	{
	public:
		/** Default constructor */
		SMSTransformationTmpl() {}
		/** Constructor taking a configuration as input argument.
		 *	@see SMSTransformation::SMSTransformation(const SMSTransformationConfig& c) */
		SMSTransformationTmpl(const SMSTransformationConfig& c):SMSTransformation(c){}
		/** Virtual destructor.*/
		virtual ~SMSTransformationTmpl(){}
		/** Pure virtual method that must be implemented in any concrete transformation class.*/
		virtual bool Do(const UnwrappedProcessingData& in,UnwrappedProcessingData& out)=0;

		/** Unsupervised Do function, receives a Segment as input and output. This overload is
		 *	the one called from the supervised Do(). Note that if BPF parameter is used, the
		 *	Amount control is also updated.
		 *  @param input the Segment that is input to the transformation
		 *  @param out the Segment that is output from the transformation.
		 *  @return Boolean value, whether the process has finished successfully or not.
		 */
		bool Do(const Segment& in, Segment& out)
		{
			
			if(mUseTemporalBPF)
				UpdateControlValueFromBPF(((TData)in.mCurrentFrameIndex)/in.GetnFrames());
			return Do(UnwrapSegment(in),UnwrapSegment(out));
			
		}

	
	protected:

		/** Selector method that unwraps a particular ProcessingData (Frame, Audio, Spectrum or
		 *	SpectralPeakArray) from a given Segment. For doing so it uses the concrete
		 *	implementations  of the UnwrapProcessingData implemented in the base class.
		 *	@return a const reference to the desired ProcessingData
		 *	@param a const reference to a Segment.
		 */
		const UnwrappedProcessingData& UnwrapSegment(const Segment& in)
		{
			return UnwrapProcessingData(in,(UnwrappedProcessingData*)(0));
		}
		/** Selector method that unwraps a particular ProcessingData (Frame, Audio, Spectrum or
		 *	SpectralPeakArray) from a given Segment. For doing so it uses the concrete
		 *	implementations  of the UnwrapProcessingData implemented in the base class.
		 *	@return a non-const reference to the desired ProcessingData
		 *	@param a non-const reference to a Segment.
		 */
		UnwrappedProcessingData& UnwrapSegment( Segment& in)
		{
			return UnwrapProcessingData(in,(UnwrappedProcessingData*)(0));
		}

		
	};


};//namespace CLAM

#endif // _SMSTransformation_
