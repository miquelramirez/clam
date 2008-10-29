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


#ifndef _SpectralShapeShift_
#define _SpectralShapeShift_

#include "SpectralEnvelopeExtract.hxx"
#include "SpectralEnvelopeApply.hxx"
#include "Frame.hxx"
#include "FDCombFilter.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "FrameTransformation.hxx"
#include "FrameTransformationConfig.hxx"
#include "SegmentTransformationConfig.hxx"
#include "SpectralPeakDetect.hxx"

namespace CLAM{


	/** 
	 *	Pitch shift with timbre preservation using the SMS model. In order to preserve timbre, 
	 *	the original spectral shape is extracted and then applied back.
	 */
	class SpectralShapeShift: public FrameTransformationTmpl<Spectrum>
	{
		
		const char *GetClassName() const {return "SpectralShapeShift";}

		InPort<Spectrum> mIn;
		OutPort<Spectrum> mOut;

		FloatInControl mSteps;
	public:

		SpectralShapeShift() 
			: 
			mIn( "InSpectrum", this),
			mOut( "OutSpectrum", this),
			mSteps("Shift Steps", this)
		{
			Configure( SegmentTransformationConfig() );
			mSpectralRange=22050;	//default
			mSpectralPeaks.AddAll();
			mSpectralPeaks.UpdateData();
			mSpectralPeaks.SetScale(EScale::eLog);
			mSpectralEnvelope.AddAll();
			mSpectralEnvelope.UpdateData();
			
			SpectralPeakDetectConfig cfg;
			cfg.SetMagThreshold(-80.f);
			cfg.SetMaxPeaks(200);	
			cfg.SetMaxFreq(15000.f);
			mSpectralPeakDetect.Configure(cfg);
			
			SpectralEnvelopeExtractConfig envCfg;
			envCfg.SetMaxPeaks(1000);
			mSpectralEnvelopeExtract.Configure(envCfg);
		}

 		~SpectralShapeShift() {}

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
		SpectralEnvelopeExtract mSpectralEnvelopeExtract;
		SpectralEnvelopeApply mSpectralEnvelopeApply;
		SpectralPeakDetect mSpectralPeakDetect;
		Spectrum mSpectralEnvelope;
		Spectrum mLogSpectrum;
		SpectralPeakArray mSpectralPeaks;
		TSize mSpectralRange;
	};		
}	//namespace CLAM

#endif // _SpectralShapeShift_

