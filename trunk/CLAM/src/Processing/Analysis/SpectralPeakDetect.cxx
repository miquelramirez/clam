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
		SetName("SpectralPeakDetect");
		
		SetNumBands(513);
		SetMagThreshold(-80);
		SetMaxPeaks(100);
	}


	/* Processing  object Method  implementations */

	SpectralPeakDetect::SpectralPeakDetect()
	{
		Configure(SpectralPeakDetectConfig());
	}

	SpectralPeakDetect::SpectralPeakDetect(const SpectralPeakDetectConfig &c = SpectralPeakDetectConfig())
	{
		Configure(c);
	}

	SpectralPeakDetect::~SpectralPeakDetect()
	{}


	/* Configure the Processing Object according to the Config object */

	bool SpectralPeakDetect::ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
	{

		mConfig = dynamic_cast<const SpectralPeakDetectConfig&>(c);
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

	/* The supervised Do() function */

	bool  SpectralPeakDetect::Do(void) 
	{
		throw(ErrProcessingObj(CLASS"::Do(): Supervised mode not implemented"),this);
		return false;
	}

	/* The  unsupervised Do() function */

	bool  SpectralPeakDetect::Do(Spectrum& input, SpectralPeakArray& out)
	{

		int i;
		TSize nSpectralPeaks = 0;	
		TSize binWidth = 0;	 // BinWidth is in NumBins

		TData  middleMag;
		TData  leftMag;
		TData  rightMag;
		TData  interpolatedBin;
		TData  SpectralPeakFreq;
		TData  SpectralPeakPhase;
		TData  SpectralPeakMag;
		TData  diffFromMax;
		TData  SamplingRate = input.GetSpectralRange() * TData(2.0);
		TSize  MagThreshold = mConfig.GetMagThreshold();
		TSize  NumBands = mConfig.GetNumBands();

		// check for correct framesize
		CLAM_ASSERT(input.GetSize() == NumBands,"SpectralPeakDetect::Do - Number of Mags does not match instantiated Number "); 

		CLAM_ASSERT(CheckOutputType(out),"SpectralPeakDetect::Do - Type of output data doesn't match "); 
		
		DataArray& inMagBuffer=input.GetMagBuffer();
		DataArray& inPhaseBuffer=input.GetPhaseBuffer();

		TSize maxPeaks=mConfig.GetMaxPeaks();
		out.SetnMaxPeaks(maxPeaks);
		out.SetnPeaks(0);
		
		DataArray& outMagBuffer=out.GetMagBuffer();
		DataArray& outFreqBuffer=out.GetFreqBuffer();
		DataArray& outPhaseBuffer=out.GetPhaseBuffer();
		DataArray& outBinPosBuffer=out.GetBinPosBuffer();
		DataArray& outBinWidthBuffer=out.GetBinWidthBuffer();

		

		

		// detection loop 
		for (i=0;i<NumBands-2;i++) {
			
			if (out.GetnPeaks()>maxPeaks) break;

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
			if ((middleMag >= leftMag) && (middleMag >= rightMag) && (middleMag > MagThreshold)) {	

				TSize SpectralPeakPosition = i+1; 	// middleMag has index i+1

				// update last BinWidth 
				if ((nSpectralPeaks > 0) && (outBinWidthBuffer[nSpectralPeaks-1] == 0)) { 			
			
					TSize lastSpectralPeakBin = (TSize) (outFreqBuffer[nSpectralPeaks-1]*2* NumBands/SamplingRate);
					TSize tempVal = binWidth - (TSize)((SpectralPeakPosition-lastSpectralPeakBin)/2.0);
					outBinWidthBuffer[nSpectralPeaks-1]=TData(tempVal);
					binWidth = (TSize) ((SpectralPeakPosition-lastSpectralPeakBin)/2.0);
				}	

				// if we get to the end of a constant area then ... 
				if ((middleMag == leftMag) && (middleMag > rightMag) && (nSpectralPeaks > 0)){
			
					// update last SpectralPeak BinPosition, it will be located in the middle of the constant area 
					interpolatedBin = (double) outBinPosBuffer[nSpectralPeaks-1] + (double) (i+1-outBinPosBuffer[nSpectralPeaks-1])/2; // center BinPos 
					SpectralPeakFreq = interpolatedBin * SamplingRate / 2 / NumBands;
			
					outFreqBuffer[nSpectralPeaks-1]=SpectralPeakFreq;
					outMagBuffer[nSpectralPeaks-1]=input.GetMag(SpectralPeakFreq);
					outPhaseBuffer[nSpectralPeaks-1]=input.GetPhase(SpectralPeakFreq);
					outBinWidthBuffer[nSpectralPeaks-1]=int(SpectralPeakPosition-outBinPosBuffer[nSpectralPeaks-1]);
					outBinPosBuffer[nSpectralPeaks-1]= interpolatedBin; // interpolated BinPos is stored	
				}

				else { //  add SpectralPeak... BinWidth will be updated in the next turn
			
					// quadratic SpectralPeak interpolation ( taken from old SMS) 
					diffFromMax =  TData(0.5) * ((leftMag-rightMag) / (leftMag- 2*middleMag + rightMag));
					interpolatedBin = SpectralPeakPosition+diffFromMax;
			
					// store SpectralPeak data 
					SpectralPeakFreq  = interpolatedBin * SamplingRate / 2 / NumBands;	// interpolate Frequency 
					SpectralPeakMag	  = middleMag-TData(.25)*(leftMag-rightMag)*diffFromMax;

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
						if (rightPhase>0)
							leftPhase+=TData(TWO_PI);
						else
							rightPhase+=TData(TWO_PI);
					if (diffFromMax >= 0)
						SpectralPeakPhase = leftPhase + diffFromMax*(rightPhase-leftPhase);
					else
						SpectralPeakPhase = leftPhase + (1+diffFromMax)*(rightPhase-leftPhase);	
					outFreqBuffer.AddElem(SpectralPeakFreq);
					outMagBuffer.AddElem(SpectralPeakMag); 
					outPhaseBuffer.AddElem(SpectralPeakPhase);
					outBinPosBuffer.AddElem(interpolatedBin);
					outBinWidthBuffer.AddElem(0); // BinWidth will be set later
					
					nSpectralPeaks++;
				
				}
			}
			binWidth++;
		}
		// update the very last binwidth value if it's not set yet 
		if ((nSpectralPeaks > 0) && (outBinWidthBuffer[nSpectralPeaks-1] == 0)){ 			
		
			TSize lastSpectralPeakBin = (TSize) (outFreqBuffer[nSpectralPeaks-1] * 2 * NumBands / SamplingRate);
			TSize tempVal = binWidth - (TSize)((i-lastSpectralPeakBin)/2.0);
			outBinWidthBuffer[nSpectralPeaks-1]=TData(tempVal);
			binWidth = (TSize) ((i-lastSpectralPeakBin)/2.0);
		}
		if(nSpectralPeaks>maxPeaks)
			out.SetnMaxPeaks(nSpectralPeaks);
		out.SetnPeaks(nSpectralPeaks);
		return true;
	}



	bool SpectralPeakDetect::CheckOutputType(SpectralPeakArray& out) 
	{
		out.SetScale(EScale::eLog);
		out.AddBinWidthBuffer();
		out.AddFreqBuffer();
		out.AddBinPosBuffer();
		out.AddMagBuffer();
		out.AddPhaseBuffer();

		out.UpdateData();

		/* Clear the  output Array */

		out.SetnPeaks(0); // set the number of SpectralPeaks to 0

		return true;
	}
};//namespace CLAM
