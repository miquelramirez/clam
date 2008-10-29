
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


#ifndef _SpectralDelay_
#define _SpectralDelay_

#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "Frame.hxx"
#include "FrameTransformation.hxx"
#include "FrameTransformationConfig.hxx"
#include "TokenDelay.hxx"

namespace CLAM{

	class SpectralDelay: public FrameTransformationTmpl<Spectrum>
	{
		InPort<Spectrum> mIn;
		OutPort<Spectrum> mOut;

		FloatInControl mLowCutoffFreqCtl;
		FloatInControl mHighCutoffFreqCtl;
		FloatInControl mLowDelayCtl;
		FloatInControl mMidDelayCtl;
		FloatInControl mHighDelayCtl;
	public:
		const char* GetClassName() const
		{
			return "SpectralDelay";
		}

		SpectralDelay() 
			: 
			mIn("In Spectrum", this), 
			mOut("Out Spectrum", this) ,
			mLowCutoffFreqCtl("LowCutoff", this),
			mHighCutoffFreqCtl("HighCutoff", this),
			mLowDelayCtl("LowDelay", this),
			mMidDelayCtl("MidDelay", this),
			mHighDelayCtl("HighDelay", this)
		{
			Configure( FrameTransformationConfig() );
			TokenDelayConfig cfg;
			cfg.SetDelay(0);
			cfg.SetMaxDelay(100);
			mLFDelay.Configure(cfg);
			mMFDelay.Configure(cfg);
			mHFDelay.Configure(cfg);
		}

 		~SpectralDelay() {}
	
		bool ConcreteConfigure( const ProcessingConfig& config )
		{
			mLowCutoffFreqCtl.SetBounds(0,1000000);
			mLowCutoffFreqCtl.DoControl(1000);
			
			mHighCutoffFreqCtl.SetBounds(0,1000000);
			mHighCutoffFreqCtl.DoControl(5000);
			
			mLowDelayCtl.SetBounds(0,100);
			mLowDelayCtl.DoControl(0);
			
			mMidDelayCtl.SetBounds(0,100);
			mMidDelayCtl.DoControl(0);

			mHighDelayCtl.SetBounds(0,100);
			mHighDelayCtl.DoControl(0);
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
		TokenDelay<Spectrum> mLFDelay;
		TokenDelay<Spectrum> mMFDelay;
		TokenDelay<Spectrum> mHFDelay;
		
		Spectrum mLFSpectrum;
		Spectrum mMFSpectrum;
		Spectrum mHFSpectrum;
		
		TSize mFrameSize;
	};		
};//namespace CLAM

#endif // _SpectralDelay_
