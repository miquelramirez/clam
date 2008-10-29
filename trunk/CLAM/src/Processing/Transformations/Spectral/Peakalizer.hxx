
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


#ifndef _Peakalizer_
#define _Peakalizer_

#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "Frame.hxx"
#include "FrameTransformation.hxx"
#include "FrameTransformationConfig.hxx"

namespace CLAM{

	class Peakalizer: public FrameTransformationTmpl<Spectrum>
	{
		InPort<Spectrum> mIn;
		OutPort<Spectrum> mOut;
		
		FloatInControl mThresholdCtl;
		FloatInControl mBandWidthCtl;
	public:
		const char* GetClassName() const
		{
			return "Peakalizer";
		}

		Peakalizer()
			: 
			mIn("In Spectrum", this), 
			mOut("Out Spectrum", this),
			mThresholdCtl("Threshold", this),
			mBandWidthCtl("Bandwidth", this)
		{
			Configure( FrameTransformationConfig() );
		}

 		~Peakalizer() {}	
		
		virtual bool InitControls()
		{ 
			mThresholdCtl.DoControl(-60);
			mBandWidthCtl.DoControl(500);
			
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
		DataArray mMag;
	};		
};//namespace CLAM

#endif // _Peakalizer_
