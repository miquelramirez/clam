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

	class SMSSineFilter: public FrameTransformation
	{
		
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "SMSSineFilter";}

		InPort<SpectralPeakArray> mInPeaks;
		OutPort<SpectralPeakArray> mOutPeaks;

		SMSSineFilterConfig mConfig;

	public:
		/** Base constructor of class. Calls Configure method with a SegmentTransformationConfig initialised by default*/
		SMSSineFilter()
			:
			mInPeaks("In SpectralPeaks", this),
			mOutPeaks("Out SpectralPeaks", this)
		{
			Configure( mConfig );
		}
		/** Constructor with an object of SegmentTransformationConfig class by parameter
		 *  @param c SegmentTransformationConfig object created by the user
		*/
		SMSSineFilter(const SMSSineFilterConfig& cfg )
			:
			mInPeaks("In SpectralPeaks", this),
			mOutPeaks("Out SpectralPeaks", this)
		{
//			Configure( SegmentTransformationConfig() );
			Configure( cfg );
		}

		virtual bool ConcreteConfigure(const ProcessingConfig& cfg) 
		{
			CopyAsConcreteConfig( mConfig, cfg );
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
	
	};		
}	//namespace CLAM

#endif // _SMSSineFilter_

