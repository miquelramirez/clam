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


#ifndef _SMSOddEvenHarmonicRatio_
#define _SMSOddEvenHarmonicRatio_

#include "FrameTransformation.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "SpectralPeakArray.hxx"
#include "Frame.hxx"
#include "SMSTransformationConfig.hxx"


namespace CLAM{


	class SMSOddEvenHarmonicRatio: public FrameTransformation
	{
		
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "SMSOddEvenHarmonicRatio";}

		InPort<SpectralPeakArray> mIn;
		OutPort<SpectralPeakArray> mOut;

		InControl mOddFactor;

	public:
		/** Base constructor of class. Calls Configure method with a SegmentTransformationConfig initialised by default*/
		SMSOddEvenHarmonicRatio() 
			: 
			mIn("In SpectralPeaks", this), 
			mOut("Out SpectralPeaks", this),
			mOddFactor("Odd Harmonics Factor", this)
		{
			Configure( SegmentTransformationConfig() );

		}

		/** Destructor of the class*/
 		~SMSOddEvenHarmonicRatio()
		{}
	

		const ProcessingConfig& GetConfig() const { throw 0; }

		bool ConcreteConfigure(const ProcessingConfig& c) { return true; }

		bool Do(const Frame& in, Frame& out)
		{
			return Do(in.GetSpectralPeakArray(), out.GetSpectralPeakArray());
		}

		bool Do(const SpectralPeakArray& in, SpectralPeakArray& out);

		bool Do()
		{
			bool result = Do(mIn.GetData(), mOut.GetData());
			mIn.Consume();
			mOut.Produce();
			return result;
		}

	};		
};//namespace CLAM

#endif // _SMSOddEvenHarmonicRatio_

