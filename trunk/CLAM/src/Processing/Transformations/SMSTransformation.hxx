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
#include "BPF.hxx"

#include "SpectralPeakArray.hxx"
#include "Frame.hxx"
#include "Segment.hxx"


namespace CLAM {


	class SMSTransformationConfig: public ProcessingConfig
	{
	public:
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SMSTransformationConfig, 4,ProcessingConfig);
		/** Name of the SMSTransformation object*/
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/** Type of transformation, for the time being just a string, should become
		* an enumeration of known transformation types??*/
		DYN_ATTRIBUTE (1, public, std::string, Type);
		/** Single Value Parameter */
		DYN_ATTRIBUTE (2, public, TData, FAmount);
		/** BPF (envelope-like) Parameter */
		DYN_ATTRIBUTE (3, public, BPF, BPFAmount);


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
			SetBPFAmount(tmpBPF);
			SetFAmount(0);
		}
	public:
		~SMSTransformationConfig(){}
	};

	/**
	 *
	 */
	class SMSTransformation: public Processing 
	{
		typedef InControlTmpl<SMSTransformation> SMSTransformationCtrl;
	
	public:
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() {return "SMSTransformation";}

		/** Config change method
		 *  @param The ProcessingConfig object
		 *  @throw bad_cast exception when the argument is not an SMSTransformationConfig object.
		 *  @return True if the cast has been commited correctly		 
		 */
		virtual bool ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
		{
			bool ret=true;
			mConfig=dynamic_cast<const SMSTransformationConfig&>(c);
			if(mConfig.HasFAmount())
				mAmountCtrl.DoControl(mConfig.GetFAmount());
			else if(mConfig.HasBPFAmount())
				mAmountCtrl.DoControl(mConfig.GetBPFAmount().GetValue(0));
			else
				ret=false;
			return ret;
		}

		
		const ProcessingConfig& SMSTransformation::GetConfig() const
		{
			return mConfig;
		}

		/** Base constructor of class. Calls Configure method with a SMSTransformationConfig initialised by default*/
		SMSTransformation():mAmountCtrl("Amount",this)
		{
			Configure(SMSTransformationConfig());
		};

		/** Constructor with an object of SMSTransformationConfig class by parameter
		 *  @param c SMSFreqShiftConfig object created by the user
		*/
		SMSTransformation(const SMSTransformationConfig& c):mAmountCtrl("Amount",this)
		{
			Configure(c);
		}
		
		/** Destructor of the class*/
 		~SMSTransformation(){};

		/** Supervised Do() function, not yet implemented*/
		virtual bool Do(void){return false;};

		/** Unsupervised Do function, receives a Processing data object input output.
		 *  @param input the processing data that is input to the transformation
		 *  @param out the processing data that is output from the transformation.
		 *  @return Boolean value, never gets to return any value as it stops in ASSERT
		 *  It is an abstract method to be implemented in all derived classes
		 */
		virtual bool Do(const ProcessingData& in, ProcessingData& out)
		{
			CLAM_ASSERT(true,"Transformation not Implemented for such datatypes");
			return false;
		}	
		
		virtual bool Do(const Audio& in, Audio& out)
		{
			CLAM_ASSERT(true,"Transformation not Implemented for such datatypes");
			return false;
		};

		virtual bool Do(const Spectrum& in, Spectrum& out)
		{
			CLAM_ASSERT(true,"Transformation not Implemented for such datatypes");
			return false;
		};
		
		virtual bool Do(const SpectralPeakArray& in, SpectralPeakArray& out)
		{
			CLAM_ASSERT(false,"Transformation not Implemented for such datatypes");
			return false;
		};
		virtual bool Do(const Frame& in, Frame& out)
		{
			CLAM_ASSERT(false,"Transformation not Implemented for such datatypes");
			return false;
		};
		
		virtual bool Do(const Segment& in, Segment& out)
		{
			CLAM_ASSERT(false,"Transformation not Implemented for such datatypes");
			return false;
		};
		
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
		SMSTransformationConfig mConfig;
	public:
		SMSTransformationCtrl mAmountCtrl;

 	}; 

};//namespace CLAM

#endif // _SMSTransformation_
