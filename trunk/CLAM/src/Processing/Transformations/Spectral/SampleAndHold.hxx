
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


#ifndef _SampleAndHold_
#define _SampleAndHold_

#include "InPort.hxx"
#include "OutPort.hxx"
#include "Frame.hxx"
#include "FrameTransformation.hxx"
#include "FrameTransformationConfig.hxx"
#include "TokenDelay.hxx"

namespace CLAM{

	class SampleAndHold: public FrameTransformationTmpl<Spectrum>
	{
		InPort<Spectrum> mIn;
		OutPort<Spectrum> mOut;

	public:
		const char* GetClassName() const
		{
			return "SampleAndHold";
		}

		SampleAndHold() 
			: 
			mIn("In Spectrum", this), 
			mOut("Out Spectrum", this)
		{
			Configure( FrameTransformationConfig() );
		}

 		~SampleAndHold() {}	
		
		bool Do(const Frame& in, Frame& out)
		{
			return Do(in.GetSpectrum(), 
				  out.GetSpectrum());
		}
		
		virtual bool InitControls()
		{ 
			mAmount.DoControl(500);
			
			mTimeElapsed = 0;
			return true;
		}
		
		bool Do(const Spectrum& in, Spectrum& out);

		bool Do()
		{
			bool result = Do(mIn.GetData(), mOut.GetData()); 
			mIn.Consume();
			mOut.Produce();
			return result;
		}
	private:
		Spectrum mSpectrum;
		
		TSize mFrameSize;
		TData mTimeElapsed;
	};		
};//namespace CLAM

#endif // _SampleAndHold_
