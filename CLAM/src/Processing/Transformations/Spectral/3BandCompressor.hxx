
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


#ifndef _ThreeBandCompressor_
#define _ThreeBandCompressor_

#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "Frame.hxx"
#include "FrameTransformation.hxx"
#include "FrameTransformationConfig.hxx"

namespace CLAM{
/**
	Limits the spectrum magnitude to different threshold for each of three bands.
	Notice that despite the name, this is a Limiter not a Compressor.
	Cutoff frequencies are normalized in the interval 0,1 which maps
	into the full spectral range of the input spectrum.
	@param[in] "In Spectrum" [Port] Input spectrum
	@param[out] "Out Spectrum" [Port] Output spectrum
	@param[in] LowCutoff [Control] Normalized frequency that separes the first and second bands. [0..1]
	@param[in] HighCutoff [Control] Normalized frequency that separes the second and third bands. [0..1]
	@param[in] LowThreshold [Control] Threshold in dB's for the first band.
	@param[in] MidThreshold [Control] Threshold in dB's for the second band.
	@param[in] HighThreshold [Control] Threshold in dB's for the third band.
	@ingroup SpectralTransformations
*/
	class ThreeBandCompressor: public FrameTransformationTmpl<Spectrum>
	{
		InPort<Spectrum> mIn;
		OutPort<Spectrum> mOut;

		FloatInControl mLowCutoffFreqCtl;
		FloatInControl mHighCutoffFreqCtl;
		FloatInControl mLowThresholdCtl;
		FloatInControl mMidThresholdCtl;
		FloatInControl mHighThresholdCtl;
	public:
		const char* GetClassName() const
		{
			return "ThreeBandCompressor";
		}

		ThreeBandCompressor() 
			: mIn("In Spectrum", this)
			, mOut("Out Spectrum", this)
			, mLowCutoffFreqCtl("LowCutoff", this)
			, mHighCutoffFreqCtl("HighCutoff", this)
			, mLowThresholdCtl("LowThreshold", this)
			, mMidThresholdCtl("MidThreshold", this)
			, mHighThresholdCtl("HighThreshold", this)
		{
			Configure( FrameTransformationConfig() );
			mLowCutoffFreqCtl.SetBounds(0.,1.);
			mHighCutoffFreqCtl.SetBounds(0.,1.);
		}

 		~ThreeBandCompressor() {}	
		
		virtual bool InitControls()
		{ 
			mLowCutoffFreqCtl.DoControl(1000);
			mHighCutoffFreqCtl.DoControl(5000);
			
			mLowThresholdCtl.DoControl(0);
			mMidThresholdCtl.DoControl(0);
			mHighThresholdCtl.DoControl(0);
			
			return true;
		}

		bool Do(const Frame& in, Frame& out)
		{
			return Do(in.GetSpectrum(), out.GetSpectrum());
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
}//namespace CLAM

#endif // _ThreeBandCompressor_
