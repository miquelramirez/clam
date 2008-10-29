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


#ifndef _SpectralSpread_
#define _SpectralSpread_

#include "SpectralEnvelopeExtract.hxx"
#include "SpectralEnvelopeApply.hxx"
#include "Frame.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "FrameTransformation.hxx"
#include "FrameTransformationConfig.hxx"
#include "SegmentTransformationConfig.hxx"
#include "Spectrum.hxx"
#include "SpecTypeFlags.hxx"


namespace CLAM{

	/** 
	 *	Pitch shift with timbre preservation using the SMS model. In order to preserve timbre, 
	 *	the original spectral shape is extracted and then applied back.
	 */
	class SpectralSpread: public FrameTransformationTmpl<Spectrum>
	{
		
		const char *GetClassName() const {return "SpectralSpread";}

		InPort<Spectrum> mIn;
		OutPort<Spectrum> mOut;
		
		FloatInControl mCenterFreqCtl;
	public:

		SpectralSpread() 
			: 
			mIn( "InSpectrum", this), 
			mOut( "OutSpectrum", this),
			mCenterFreqCtl("CenterFreq", this)
		{
			Configure( SegmentTransformationConfig() );
			SpecTypeFlags flg;
			flg.bMagPhase = 1;
			flg.bMagPhaseBPF = 1;
			mBPFSpectrum.SetType(flg);
			mFlag.bMagPhase = 0;
			mFlag.bMagPhaseBPF = true;
		}

 		~SpectralSpread() {}

		virtual bool InitControls()
		{ 
			mCenterFreqCtl.DoControl(1000);
			mAmount.DoControl(100);
			
			return true;
		}
		
		bool Do(const Frame& in, Frame& out)
		{
			return Do(in.GetSpectrum(), 
				out.GetSpectrum());
		}
	
		bool Do(const Spectrum& inpeaks,Spectrum& out);

		bool Do()
		{
			bool result = Do(mIn.GetData(), mOut.GetData());
			mIn.Consume();
			mOut.Produce();
			return result;	
		}

	private:
		Spectrum mBPFSpectrum;
		SpecTypeFlags mFlag;
	};		
}	//namespace CLAM

#endif // _SpectralSpread_

