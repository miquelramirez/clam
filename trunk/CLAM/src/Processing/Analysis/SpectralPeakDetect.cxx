/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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
#include "ErrProcessingObj.hxx"

#define CLASS "SpectralPeakDetect"

namespace CLAM {

	/* The  Configuration object has at least to have a name */

	void SpectralPeakDetectConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		DefaultValues();
	}

	
	void SpectralPeakDetectConfig::DefaultValues()
	{
		SetMagThreshold(-80.f);
		SetMaxPeaks(1000);
		SetMaxFreq(10000.f);
	}


	/* Processing  object Method  implementations */

	SpectralPeakDetect::SpectralPeakDetect()
		: mInput( "Input spectrum", this, 1 ),
		  mOutput( "Output spectral peak array", this, 1 )
	{
		Configure(SpectralPeakDetectConfig());
	}

	SpectralPeakDetect::SpectralPeakDetect(const SpectralPeakDetectConfig &c = SpectralPeakDetectConfig())
		: mInput( "Input spectrum", this, 1 ),
		  mOutput( "Output spectral peak array", this, 1 )
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

	void SpectralPeakDetect::Attach( Spectrum& in, SpectralPeakArray& out )
	{
		mInput.Attach( in );
		mOutput.Attach( out );
	}

	/* The supervised Do() function */

	bool  SpectralPeakDetect::Do(void)
	{

		return Do( mInput.GetData(), mOutput.GetData() );
	}

	/* The  unsupervised Do() function */

	bool  SpectralPeakDetect::Do(const Spectrum& input, SpectralPeakArray& out)
	{

		int i;
		TSize nSpectralPeaks = 0;
		TSize binWidth = 0;	 // BinWidth is in NumBins

		TData middleMag;
		TData leftMag;
		TData rightMag;
		TData interpolatedBin;
		TData spectralPeakFreq=0;
		TData spectralPeakPhase;
		TData spectralPeakMag;
		TData diffFromMax;
		const TData samplingRate = input.GetSpectralRange() * TData(2.0);
		const TSize magThreshold = mConfig.GetMagThreshold();
		const TSize nBins = input.GetSize();
		const TData maxFreq= mConfig.GetMaxFreq();

		CLAM_ASSERT(CheckOutputType(out),"SpectralPeakDetect::Do - Type of output data doesn't match ");
		
		DataArray& inMagBuffer=input.GetMagBuffer();
		DataArray& inPhaseBuffer=input.GetPhaseBuffer();

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
			
			// local maximum Detected ! 
			if ((middleMag >= leftMag) && (middleMag >= rightMag) && (middleMag > magThreshold)) {	

				TSize SpectralPeakPosition = i+1; 	// middleMag has index i+1

				// update last BinWidth 
				if ((nSpectralPeaks > 0) && (outBinWidthBuffer[nSpectralPeaks-1] == 0)) { 			
			
					TSize lastSpectralPeakBin = (TSize) (outFreqBuffer[nSpectralPeaks-1]*2* nBins/samplingRate);
					TSize tempVal = binWidth - (TSize)((SpectralPeakPosition-lastSpectralPeakBin)/2.0);
					outBinWidthBuffer[nSpectralPeaks-1]=TData(tempVal);
					binWidth = (TSize) ((SpectralPeakPosition-lastSpectralPeakBin)/2.0);
				}	

				// if we get to the end of a constant area then ...
				if ((middleMag == leftMag) && (middleMag > rightMag) && (nSpectralPeaks > 0)){
			
					// update last SpectralPeak BinPosition, it will be located in the middle of the constant area 
					interpolatedBin = (double) outBinPosBuffer[nSpectralPeaks-1] + (double) (i+1-outBinPosBuffer[nSpectralPeaks-1])/2; // center BinPos 
					spectralPeakFreq = interpolatedBin * samplingRate / 2 / nBins;
			
					outFreqBuffer[nSpectralPeaks-1]=spectralPeakFreq;
					outMagBuffer[nSpectralPeaks-1]=input.GetMag(spectralPeakFreq);
					outPhaseBuffer[nSpectralPeaks-1]=input.GetPhase(spectralPeakFreq);
					outBinWidthBuffer[nSpectralPeaks-1]=int(SpectralPeakPosition-outBinPosBuffer[nSpectralPeaks-1]);
					outBinPosBuffer[nSpectralPeaks-1]= interpolatedBin; // interpolated BinPos is stored	
				}

				else { //  add SpectralPeak... BinWidth will be updated in the next turn
			
					// quadratic SpectralPeak interpolation ( taken from old SMS) 
					diffFromMax =  TData(0.5) * ((leftMag-rightMag) / (leftMag- 2*middleMag + rightMag));
					interpolatedBin = SpectralPeakPosition+diffFromMax;
			
					// store SpectralPeak data 
					spectralPeakFreq  = interpolatedBin * samplingRate / 2 / nBins;	// interpolate Frequency 
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



	bool SpectralPeakDetect::CheckOutputType(SpectralPeakArray& out) 
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

/*		out.SetScale(EScale::eLog);
		out.AddBinWidthBuffer();
		out.AddFreqBuffer();
		out.AddBinPosBuffer();
		out.AddMagBuffer();
		out.AddPhaseBuffer();
		out.UpdateData();*/

		return true;
	}
};//namespace CLAM
