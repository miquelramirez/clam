
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


#ifndef _OscillatingSpectralNotch_
#define _OscillatingSpectralNotch_

#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "Frame.hxx"
#include "FrameTransformation.hxx"
#include "FrameTransformationConfig.hxx"
#include "SimpleOscillator.hxx"

namespace CLAM{

	class OscillatingSpectralNotch: public FrameTransformationTmpl<Spectrum>
	{
		InPort<Spectrum> mIn;
		OutPort<Spectrum> mOut;
		
		InControl mFreqCtl;	
		InControl mBandwidthCtl;
		InControl mGainCtl;
		InControl mOscFreqCtl;
		InControl mOscWidthCtl;

	public:
		const char* GetClassName() const
		{
			return "OscillatingSpectralNotch";
		}

		OscillatingSpectralNotch() 
			: 
			mIn("In Spectrum", this), 
			mOut("Out Spectrum", this),
			mFreqCtl("Freq", this),
			mBandwidthCtl("Bandwidth", this),
			mGainCtl("Gain",this),
			mOscFreqCtl("OscFreq", this),
			mOscWidthCtl("OscWidth",this)
		{
			Configure( FrameTransformationConfig() );
		}

 		~OscillatingSpectralNotch() {}	
		bool ConcreteConfigure( const ProcessingConfig& config )
		{
			mFreqCtl.SetBounds(80,15000);
			mFreqCtl.DoControl(1000);
			
			mBandwidthCtl.SetBounds(1,1000);
			mBandwidthCtl.DoControl(100);
			
			mGainCtl.SetBounds(-60,60);
			mGainCtl.DoControl(0);
			
			mOscFreqCtl.SetBounds(0.1,5);
			mOscFreqCtl.DoControl(1);

			mOscWidthCtl.SetBounds(5,200);
			mOscWidthCtl.DoControl(50);
			return true;
		}
		
		virtual bool InitControls()
		{ 
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
		SimpleOscillator mOscillator;
	
	};		
};//namespace CLAM

#endif // _OscillatingSpectralNotch_
