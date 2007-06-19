/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "MelFilterBank.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{
	static FactoryRegistrator<ProcessingFactory, MelFilterBank> regMelFilterBank("MelFilterBank");
}

	void MelFilterBankConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetUsePower(false);
		SetSpectrumSize(513);
		SetNumBands(20);
		SetSpectralRange(22050);
		SetLowCutoff(0);
		SetHighCutoff(11025);
	}
	
	MelFilterBank::MelFilterBank()
		: mIn("Spectrum", this)
		, mOut("Mel Spectrum", this)
	{
		Configure(MelFilterBankConfig()); 
	}

	MelFilterBank::MelFilterBank( const MelFilterBankConfig& cfg )
		: mIn("Spectrum", this)
		, mOut("Mel Spectrum", this)
	{
		Configure( cfg );
	}

	MelFilterBank::~MelFilterBank()
	{
	}

	bool MelFilterBank::Do()
	{
		const Spectrum & spectrum = mIn.GetData();
		MelSpectrum & melSpectrum = mOut.GetData();
		bool ok = Do(spectrum,melSpectrum);
		mIn.Consume();
		mOut.Produce();
		return ok;
	}

	bool MelFilterBank::Do( const Spectrum& spec, MelSpectrum& melSpec )
	{
		if ( !AbleToExecute() ) return false; // Object is disabled
		
		const DataArray& specMag = spec.GetMagBuffer();

		CLAM_ASSERT( specMag.Size() == mConfig.GetSpectrumSize(),
		 "Spectrum doesn't have the expected size!" );
		CLAM_ASSERT( spec.GetSpectralRange() == mConfig.GetSpectralRange(),
		 "Spectrum doesn't have the expected frequency range!" );
		CLAM_ASSERT( spec.GetScale() == EScale::eLinear,
		 "Spectrum is not in linear scale!" );

		const TSize numBands = mConfig.GetNumBands();

		melSpec.SetNumBands(numBands);
		melSpec.SetLowCutoff(mConfig.GetLowCutoff());
		melSpec.SetHighCutoff(mConfig.GetHighCutoff());
		melSpec.SetSpectralRange(spec.GetSpectralRange());

		DataArray& melCoeffs = melSpec.GetCoefficients();
		
		if ( melCoeffs.Size() != numBands ) {
			melCoeffs.Resize( numBands );
			melCoeffs.SetSize( numBands );
		}
		for( int i = 0; i < numBands; i++)
			melCoeffs[i] = 0;
		
		const bool usePower = mConfig.GetUsePower();

		for (TIndex i=mLowIdx; i<=mHighIdx; i++) {

			// NOTE: What is the significance of using square?
			TData mag = specMag[i];
			if (usePower) mag *=mag;

			// Get the Mel band number.
			TIndex bandIdx = mMelBand[i];

			// Weight spectrum sample with triangular window.
			TData weightedMag = mFilterWeights[i] * mag;

			// Add spectrum sample contribution to apropriate bands.
			if (bandIdx >= 0) {
				melCoeffs[bandIdx] += weightedMag;
			}

			if ((bandIdx+1) < numBands) {
				melCoeffs[bandIdx+1] += mag - weightedMag;
			}
		}
		return true;
	}
	
	bool MelFilterBank::ConcreteConfigure( const ProcessingConfig& cfg )
	{
		CopyAsConcreteConfig( mConfig, cfg );
		InitializeTables();
		return true;
	}
	
	TData MelFilterBank::Mel( TData linFreq )
	{
		/* When using log instead of log10, the scaling factor is 1127
		   instead of 2595. */

		/* NOTE: The discrepancy between HTK and CLAM originates here, or
		   more specificaly from multiplying the argument passed to this
		   method with deltaFreq. (Ref. fb.fres in HTK source code.) */
		
		return 1127.0*log(1.0 + linFreq/700.0);
	}

	void MelFilterBank::InitializeTables()
	{

		/* Filterbank cutoff frequencies in Hz. */
		const TData lowCutoff  = mConfig.GetLowCutoff();
		const TData highCutoff = mConfig.GetHighCutoff();

		/* Filterbank cutoff frequencies in Mel scale. */
		const TData melLowCutoff  = Mel(lowCutoff);
		const TData melHighCutoff = Mel(highCutoff);
		const TData melFreqRange  = melHighCutoff - melLowCutoff;

		/* Index frequency resolution. */
		const TSize specSize  = mConfig.GetSpectrumSize();
		const TData specRange = mConfig.GetSpectralRange();

		const TData deltaFreq = specRange/specSize;


		/* Spectrum index of lowest filterbank frequency (must be 1 or
		   more). */
		mLowIdx = (TIndex)(lowCutoff/deltaFreq + 1.5);


		if (mLowIdx < 1) mLowIdx = 1;

		/* Spectrum index of highest filterbank frequency (must not exceed
		   spectrum size). */
		mHighIdx = (TIndex)(highCutoff/deltaFreq - 0.5);

		if (mHighIdx >= specSize) mHighIdx = specSize-1;


		/* Table of filterbank centre frequencies. */
		const TSize maxBands = mConfig.GetNumBands() + 1;

		TData* centreFreq = new TData[maxBands];

		for (TIndex i=0; i<maxBands; i++) {
			centreFreq[i] = ((i+1)/(TData)maxBands)*melFreqRange + melLowCutoff;
		}


		/* Table for converting linear indexes to Mel band numbers. */
		mMelBand.Resize( specSize );
		mMelBand.SetSize( specSize );

		TData  melFreq = 0;
		TIndex bandIdx = 0;
		for (TIndex i=0; i<specSize; i++) {

			if (i<mLowIdx || i>mHighIdx) {

				// Index is outside the desired range.
				mMelBand[i] = -1;
			} else {

				melFreq = Mel((TData)i*deltaFreq);

				// Select the band of the closest centre frequency beneath.

				// NOTE: The condition bandIdx<maxBands may cause an index out
				// of range error in Do(...)!!!

				while (centreFreq[bandIdx]<melFreq && bandIdx<maxBands) bandIdx++;
				mMelBand[i] = bandIdx-1;
			}

		}


		/* Table of triangular filterbank window weights. */
		mFilterWeights.Resize( specSize );
		mFilterWeights.SetSize( specSize );
		
		for (TIndex i=0; i<specSize; i++) {
			bandIdx = mMelBand[i];

			if (i<mLowIdx || i>mHighIdx) {
				mFilterWeights[i] = 0.0;
			} else {

				if (bandIdx >= 0) {
					mFilterWeights[i] = (centreFreq[bandIdx+1] - Mel((TData)i*deltaFreq))
						/ (centreFreq[bandIdx+1] - centreFreq[bandIdx]);

				} else {
					mFilterWeights[i] = (centreFreq[0] - Mel((TData)i*deltaFreq))
						/ (centreFreq[0] - melLowCutoff);

				}
			}

		} // End for

		delete [] centreFreq;
	}

}

