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

#ifndef _SMSSineFilter_
#define _SMSSineFilter_

#include "BPF.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "Frame.hxx"
#include "SpectralPeakArray.hxx"
#include "FrameTransformation.hxx"
#include "FrameTransformationConfig.hxx"
#include "Processing.hxx"
#include "SegmentTransformationConfig.hxx"


namespace CLAM
{
#if 0
	class SMSSineFilterConfig : public ProcessingConfig
	{

	   public:
			DYNAMIC_TYPE_USING_INTERFACE (SMSSineFilterConfig, 1,ProcessingConfig);
			/** Single Value Parameter */
//			DYN_ATTRIBUTE (0, public, TData, Amount);
			/** BPF (envelope-like) Parameter */
			DYN_ATTRIBUTE (0, public, BPF, BPF);


		private:


			void DefaultInit()
			{
				AddAll();
				UpdateData();
				DefaultValues();
			}

			void DefaultValues()
			{
				GetBPF().Insert( 0, 0 );
				GetBPF().Insert( 22050, 0 );
			}

	};
#endif
	class SMSSineFilter: public FrameTransformation
	{
		
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "SMSSineFilter";}

		InPort<SpectralPeakArray> mInPeaks;
		OutPort<SpectralPeakArray> mOutPeaks;

		FloatInControl mIndexCtl;//says what the amount sent as control is modifying
		FloatInControl mUpdateBPFCtl;//"boolean" control used to say that we want to update BPF
		FloatInControl mGainCtl;
		
		void UpdateBPF(TControlData value)
		{
			CLAM::BPF& bpf= mConfig.GetBPF();
			//this should never happen, it should be initialized at configuration time
			if(bpf.Size()==0)
			{
				InitBPF();
			}
			
			bpf.SetValue((int)mIndexCtl.GetLastValue(), mGainCtl.GetLastValue());
		}
		
	public:
		/** Base constructor of class. Calls Configure method with a SegmentTransformationConfig initialised by default*/
		SMSSineFilter()
			: mInPeaks("In SpectralPeaks", this)
			, mOutPeaks("Out SpectralPeaks", this)
			, mIndexCtl("Index", this)
			, mUpdateBPFCtl("UpdateBPF", this, &SMSSineFilter::UpdateBPF)
			, mGainCtl("Gain",this)

		{
			
			//setting default configuration
			mConfig.AddBPF();
			mConfig.UpdateData();
			Configure( mConfig );
		}
		/** Constructor with an object of SegmentTransformationConfig class by parameter
		 *  @param c SegmentTransformationConfig object created by the user
		*/
		SMSSineFilter(const FrameTransformationConfig& cfg )
			: mInPeaks("In SpectralPeaks", this)
			, mOutPeaks("Out SpectralPeaks", this)
			, mIndexCtl("Index", this)
			, mUpdateBPFCtl("UpdateBPF", this, &SMSSineFilter::UpdateBPF)
			, mGainCtl("Gain",this)
		{
			Configure( cfg );
		}

		virtual bool ConcreteConfigure(const ProcessingConfig& cfg) 
		{
			CopyAsConcreteConfig( mConfig, cfg );
			InitBPF();
			return true; 
		}

		const ProcessingConfig& GetConfig() const { return mConfig; }

		/** Destructor of the class*/
 		~SMSSineFilter()
		{}

		bool Do(const Frame& in, Frame& out)
		{
			return Do( in.GetSpectralPeakArray(), out.GetSpectralPeakArray() );
		}

		bool Do(const SpectralPeakArray& in, SpectralPeakArray& out);

				// Note that overriding this method breaks the processing chain functionality. 
		bool Do()
		{
			bool result = Do( mInPeaks.GetData(), mOutPeaks.GetData() );
			mInPeaks.Consume();
			mOutPeaks.Produce();
			return result;
		}
		
		void InitBPF()
		{
			if (!mConfig.HasBPF())
			{
				mConfig.AddBPF();
				mConfig.UpdateData();
			}
			if(mConfig.GetBPF().Size()==0)//else we asume that the user has initialized it before
			{
				BPF& bpf=mConfig.GetBPF();
				bpf.Resize(500);
				bpf.SetSize(500);
				int i;
				for (i=0; i< 500; i++)
				{
					bpf.SetValue(i,0);
				}
			}
		}
	
	};		
}	//namespace CLAM

#endif // _SMSSineFilter_

