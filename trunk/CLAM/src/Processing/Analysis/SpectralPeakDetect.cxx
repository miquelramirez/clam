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

#include "Complex.hxx"
#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"
#include "SpectralPeakDetect.hxx"



namespace CLAM {

	/* Processing  object Method  implementations */

	SpectralPeakDetect::SpectralPeakDetect()
		: mInput( "Input spectrum", this ),
		  mOutput( "Output spectral peak array", this )
	{
		Configure(SpectralPeakDetectConfig());
	}

	SpectralPeakDetect::SpectralPeakDetect(const SpectralPeakDetectConfig &c = SpectralPeakDetectConfig())
		: mInput( "Input spectrum", this ),
		  mOutput( "Output spectral peak array", this )
	{
		Configure(c);
	}

	SpectralPeakDetect::~SpectralPeakDetect()
	{}


	/* Configure the Processing Object according to the Config object */

	bool SpectralPeakDetect::ConcreteConfigure(const ProcessingConfig& c)
	{

		CopyAsConcreteConfig(mConfig, c);
		return true;
	}

	/* Setting Prototypes for faster processing */

	bool SpectralPeakDetect::SetPrototypes(Spectrum& inputs,const SpectralPeakArray& out)
	{
		return true;
	}

	bool SpectralPeakDetect::SetPrototypes()
	{
		return true;
	}
	
	bool SpectralPeakDetect::UnsetPrototypes()
	{
		return true;
	}

	bool  SpectralPeakDetect::Do(void)
	{
		bool result = false;

		mOutput.GetData().SetScale( EScale::eLog );

		if (mInput.GetData().GetScale() != EScale::eLog)
		{
			mTmpLinearInSpectrum = mInput.GetData();
			mTmpLinearInSpectrum.ToDB();
			result = Do( mTmpLinearInSpectrum, mOutput.GetData() );
		}
		else
		{
			result = Do( mInput.GetData(), mOutput.GetData() );
		}

		mInput.Consume();
		mOutput.Produce();
		return result;
	}

	/* The  unsupervised Do() function */

	bool  SpectralPeakDetect::Do(const Spectrum& input, SpectralPeakArray& out)
	{
		CLAM_ASSERT(CheckInputType(input), "SpectralPeakDetect::Do() - Type of input data doesn't match expected type.");
		CLAM_ASSERT(CheckOutputType(out), "SpectralPeakDetect::Do() - Type of output data doesn't match expected type.");

		TData middleMag;
		TData leftMag;
		TData rightMag;
		const TData samplingRate = input.GetSpectralRange() * TData(2.0);
		const TData magThreshold = mConfig.GetMagThreshold();
		const TSize nBins = input.GetSize();
		const TData maxFreq= mConfig.GetMaxFreq();

		const DataArray& inMagBuffer=input.GetMagBuffer();
		const DataArray& inPhaseBuffer=input.GetPhaseBuffer();

		TSize maxPeaks=mConfig.GetMaxPeaks();
		if (out.GetnMaxPeaks() != maxPeaks)
		{
			out.SetnMaxPeaks(maxPeaks);
		}

		out.SetnPeaks(0);
		
		DataArray& outMagBuffer=out.GetMagBuffer();
		DataArray& outFreqBuffer=out.GetFreqBuffer();
		DataArray& outPhaseBuffer=out.GetPhaseBuffer();
		DataArray& outBinPosBuffer=out.GetBinPosBuffer();
		DataArray& outBinWidthBuffer=out.GetBinWidthBuffer();
		
		// detection loop 
		TSize nSpectralPeaks = 0;
		TSize binWidth = 0;	 // BinWidth is in NumBins

		int i;
		for (i = 0; (i < nBins-2) && (nSpectralPeaks < maxPeaks); ++i)
		{
			leftMag 	= inMagBuffer[i];
			middleMag	= inMagBuffer[i+1];
			rightMag 	= inMagBuffer[i+2];

			// local constant detected 
			if (middleMag == leftMag && leftMag == rightMag) {

				if ((nSpectralPeaks > 0) && (outBinWidthBuffer[nSpectralPeaks-1] == 0)) {
			
					outBinWidthBuffer[nSpectralPeaks-1]=TData(binWidth); // store last SpectralPeakBinWidth
				}
				binWidth = 1; // Reset Binwidth
				continue;
			}
		
			// local Minimum detected 
			if ((middleMag <= leftMag) && (middleMag<= rightMag)) {
				if ((nSpectralPeaks > 0) && (outBinWidthBuffer[nSpectralPeaks-1] == 0)) {
					outBinWidthBuffer[nSpectralPeaks-1]=TData(binWidth); // store last SpectralPeakBinWidth
				}
				binWidth = 0; // Reset Binwidth
			}
			
			TData interpolatedBin;
			TData spectralPeakFreq=0;
			TData spectralPeakPhase;
			TData spectralPeakMag;
			// local maximum Detected ! 
			if ((middleMag >= leftMag) && (middleMag >= rightMag) && (middleMag > magThreshold)) {	

				TSize SpectralPeakPosition = i+1; 	// middleMag has index i+1

				// update last BinWidth 
				if ((nSpectralPeaks > 0) && (outBinWidthBuffer[nSpectralPeaks-1] == 0)) { 			
			
					TSize lastSpectralPeakBin = (TSize) (outFreqBuffer[nSpectralPeaks-1]*2* (nBins-1)/samplingRate);
					TSize tempVal = binWidth - (TSize)((SpectralPeakPosition-lastSpectralPeakBin)/2.0);
					outBinWidthBuffer[nSpectralPeaks-1]=TData(tempVal);
					binWidth = (TSize) ((SpectralPeakPosition-lastSpectralPeakBin)/2.0);
				}	

				// if we get to the end of a constant area then ...
				if ((middleMag == leftMag) && (middleMag > rightMag) && (nSpectralPeaks > 0)){
			
					// update last SpectralPeak BinPosition, it will be located in the middle of the constant area 
					interpolatedBin = (double) outBinPosBuffer[nSpectralPeaks-1] + (double) (i+1-outBinPosBuffer[nSpectralPeaks-1])/2; // center BinPos 
					spectralPeakFreq = interpolatedBin * samplingRate / 2 / (nBins-1);
			
					outFreqBuffer[nSpectralPeaks-1]=spectralPeakFreq;
					outMagBuffer[nSpectralPeaks-1]=input.GetMag(spectralPeakFreq);
					outPhaseBuffer[nSpectralPeaks-1]=input.GetPhase(spectralPeakFreq);
					outBinWidthBuffer[nSpectralPeaks-1]=int(SpectralPeakPosition-outBinPosBuffer[nSpectralPeaks-1]);
					outBinPosBuffer[nSpectralPeaks-1]= interpolatedBin; // interpolated BinPos is stored	
				}

				else { //  add SpectralPeak... BinWidth will be updated in the next turn
			
					// Estimating the ``true'' maximum peak (frequency and magnitude) of the detected local maximum 
					// using a parabolic cure-fitting. The idea is that the main-lobe of spectrum of most analysis 
					// windows on a dB scale looks like a parabola and therefore the maximum of a parabola fitted 
					// through a local maxima bin and it's two neighboring bins will give a good approximation 
					// of the actual frequency and magnitude of a sinusoid in the input signal.
					//
					// The parabola f(x) = a(x-n)^2 + b(x-n) + c can be completely described using 3 points; 
					// f(n-1) = A1, f(n) = A2 and f(n+1) = A3, where 
					// A1 = 20log10(|X(n-1)|), A2 = 20log10(|X(n)|), A3 = 20log10(|X(n+1)|).
					//
					// Solving these equation yields: a = 1/2*A1 - A2 + 1/2*A3, b = 1/2*A3 - 1/2*A1 and 
					// c = A2.
					//
					// As the 3 bins are known to be a maxima, solving d/dx f(x) = 0, yields (fractional) bin 
					// position x of the estimated peak. Substituting delta_x for (x-n) in this equation yields 
					// the fractional offset in bins from n where the peak's maximum is.
					//
					// Solving this equation yields: delta_x = 1/2 * (A1 - A3)/(A1 - 2*A2 + A3).
					// 
					// Computing f(n+delta_x) will estimate the peak's magnitude (in dB's):
					// f(n+delta_x) = A2 - 1/4*(A1-A3)*delta_x.

					const TData diffFromMax = TData(0.5) * ((leftMag-rightMag) / (leftMag- 2*middleMag + rightMag));
					interpolatedBin = SpectralPeakPosition+diffFromMax;
			
					// store SpectralPeak data 
					spectralPeakFreq  = interpolatedBin * samplingRate / 2 / (nBins-1); // interpolate Frequency 
					spectralPeakMag	  = middleMag-TData(.25)*(leftMag-rightMag)*diffFromMax;

					TData leftPhase,rightPhase;
					if (diffFromMax>=0)
					{
						leftPhase = inPhaseBuffer[i+1];
						rightPhase = inPhaseBuffer[i+2];
					}
					else
					{
						leftPhase = inPhaseBuffer[i];
						rightPhase = inPhaseBuffer[i+1];
					}

					if (fabs(rightPhase-leftPhase)>PI)
					{
						if (rightPhase>0)
							leftPhase+=TData(TWO_PI);
						else
							rightPhase+=TData(TWO_PI);
					}

					if (diffFromMax >= 0)
						spectralPeakPhase = leftPhase + diffFromMax*(rightPhase-leftPhase);
					else
						spectralPeakPhase = leftPhase + (1+diffFromMax)*(rightPhase-leftPhase);

					if (spectralPeakFreq>maxFreq)
						break;

					outFreqBuffer.AddElem(spectralPeakFreq);
					outMagBuffer.AddElem(spectralPeakMag);
					outPhaseBuffer.AddElem(spectralPeakPhase);
					outBinPosBuffer.AddElem(interpolatedBin);
					outBinWidthBuffer.AddElem(0); // BinWidth will be set later

					nSpectralPeaks++;
				
				}
			}
			binWidth++;
		}
		
		// update the very last binwidth value if it's not set yet 
		if ((nSpectralPeaks > 0) && (outBinWidthBuffer[nSpectralPeaks-1] == 0)){ 			
		
			TSize lastSpectralPeakBin = (TSize) (outFreqBuffer[nSpectralPeaks-1] * 2 * nBins / samplingRate);
			TSize tempVal = binWidth - (TSize)((i-lastSpectralPeakBin)/2.0);
			outBinWidthBuffer[nSpectralPeaks-1]=TData(tempVal);
			binWidth = (TSize) ((i-lastSpectralPeakBin)/2.0);
		}


		//TODO: I don't know if we could also change nMaxPeaks if we have found less
		//but then we would be resizing array in every call to the Do and that is not
		//very nice either.
		
		//All this is not necessary, it is not doing anything
/*
		if(nSpectralPeaks>maxPeaks)
			out.SetnMaxPeaks(nSpectralPeaks);
		out.SetnPeaks(nSpectralPeaks);
*/
		return true;
	}


	bool SpectralPeakDetect::CheckInputType(const Spectrum &in)
	{
		if (!in.HasScale())
			return false;

		if (in.GetScale() != EScale::eLog)
			return false;

		if (!in.HasSpectralRange())
			return false;

		if (!in.HasMagBuffer())
			return false;

		if (!in.HasPhaseBuffer())
			return false;

		return true;
	}

	bool SpectralPeakDetect::CheckOutputType(const SpectralPeakArray &out) 
	{
		if (!out.HasScale())
			return false;

		if (out.GetScale() != EScale::eLog)
			return false;

		if (!out.HasBinWidthBuffer())
			return false;

		if (!out.HasFreqBuffer())
			return false;

		if (!out.HasBinPosBuffer())
			return false;

		if (!out.HasMagBuffer())
			return false;

		if (!out.HasPhaseBuffer())
			return false;

		return true;
	}

}

