
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


#ifndef _Deesser_
#define _Deesser_

#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "Frame.hxx"
#include "FrameTransformation.hxx"
#include "FrameTransformationConfig.hxx"
#include "SpectralNotch.hxx"

namespace CLAM{

	class Deesser: public FrameTransformationTmpl<Spectrum>
	{
		InPort<Spectrum> mIn;
		OutPort<Spectrum> mOut;

		FloatInControl mLowCutoffFreqCtl;
		FloatInControl mHighCutoffFreqCtl;
		FloatInControl mThresholdCtl;
	public:
		const char* GetClassName() const
		{
			return "Deesser";
		}

		Deesser() 
			: 
			mIn("In Spectrum", this), 
			mOut("Out Spectrum", this) ,
			mLowCutoffFreqCtl("LowCutoff", this),
			mHighCutoffFreqCtl("HighCutoff", this),
			mThresholdCtl("Threshold", this)
		{
			Configure( FrameTransformationConfig() );
			mNotch.Configure( FrameTransformationConfig() );
		}

 		~Deesser() {}	
		
		virtual bool InitControls()
		{ 
			mLowCutoffFreqCtl.DoControl(3000);
			mHighCutoffFreqCtl.DoControl(6000);
			mThresholdCtl.DoControl(-30);
			
			return true;
		}
		
		bool Do(const Frame& in, Frame& out)
		{
			return Do(in.GetSpectrum(), 
				  out.GetSpectrum());
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
		SpectralNotch mNotch;
	};		
};//namespace CLAM

#endif // _Deesser_
