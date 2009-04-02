
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


#ifndef _ThreeBandFilter_
#define _ThreeBandFilter_

#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "Frame.hxx"
#include "FrameTransformation.hxx"
#include "FrameTransformationConfig.hxx"

namespace CLAM{

/**
	Amplifies the spectrum magnitude to different gain for each of three bands.
	Cutoff frequencies are normalized in the interval 0,1 which maps
	into the full spectral range of the input spectrum.
	It applies one spectral bin transitions to avoid Gibbs effect.
	@param[in] "In Spectrum" [Port] Input spectrum
	@param[out] "Out Spectrum" [Port] Output spectrum
	@param[in] LowCutoff [Control] Normalized frequency that separes the first and second bands.
	@param[in] HighCutoff [Control] Normalized frequency that separes the second and third bands. [0..1]
	@param[in] LowGain [Control] Gain in dB's for the first band.
	@param[in] MidGain [Control] Gain in dB's for the second band.
	@param[in] HighGain [Control] Gain in dB's for the third band.
	@ingroup SpectralTransformations
*/
	class ThreeBandFilter: public FrameTransformationTmpl<Spectrum>
	{
		InPort<Spectrum> mIn;
		OutPort<Spectrum> mOut;

		FloatInControl mLowCutoffFreqCtl;
		FloatInControl mHighCutoffFreqCtl;
		FloatInControl mLowGainCtl;
		FloatInControl mMidGainCtl;
		FloatInControl mHighGainCtl;
	public:
		const char* GetClassName() const
		{
			return "ThreeBandFilter";
		}

		ThreeBandFilter() 
			: mIn("In Spectrum", this)
			, mOut("Out Spectrum", this)
			, mLowCutoffFreqCtl("LowCutoff", this)
			, mHighCutoffFreqCtl("HighCutoff", this)
			, mLowGainCtl("LowGain", this)
			, mMidGainCtl("MidGain", this)
			, mHighGainCtl("HighGain", this)
		{
			Configure( FrameTransformationConfig() );
		}

 		~ThreeBandFilter() {}	
		
		bool ConcreteConfigure( const ProcessingConfig& config )
		{
			mLowCutoffFreqCtl.SetBounds(0.,1000000.);
			mLowCutoffFreqCtl.DoControl(1000.);
			
			mHighCutoffFreqCtl.SetBounds(0.,1000000.);
			mHighCutoffFreqCtl.DoControl(5000.);
			
			mLowGainCtl.SetBounds(0,100);
			mLowGainCtl.DoControl(0);
			
			mMidGainCtl.SetBounds(0,100);
			mMidGainCtl.DoControl(0);

			mHighGainCtl.SetBounds(0,100);
			mHighGainCtl.DoControl(0);
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
	};		
};//namespace CLAM

#endif // _ThreeBandFilter_
