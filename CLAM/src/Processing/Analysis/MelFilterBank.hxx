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

#ifndef __MELFILTERBANK__
#define __MELFILTERBANK__

#include "Processing.hxx"
#include "Spectrum.hxx"
#include "MelSpectrum.hxx"
#include "Array.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"

namespace CLAM
{
	
	class MelFilterBankConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE( MelFilterBankConfig, 6, ProcessingConfig );
		/** Spectrum size **/
		DYN_ATTRIBUTE( 0, public, TSize,       SpectrumSize );
		/** Spectral Range **/
		DYN_ATTRIBUTE( 1, public, TData,       SpectralRange );
		/** Low cutoff frequency **/
		DYN_ATTRIBUTE( 2, public, TData,       LowCutoff );
		/** High cutoff frequency **/
		DYN_ATTRIBUTE( 3, public, TData,       HighCutoff );
		/** Number of Bands **/
		DYN_ATTRIBUTE( 4, public, TSize,       NumBands );
		/** Use square of the spectrum for computation (false by default) **/
		DYN_ATTRIBUTE( 5, public, bool,        UsePower );

	protected:
		void DefaultInit();
	};

	/**
	 * This class performs filterbank analysis for Mel Frequency
	 * Cepstrum Coefficients (MFCC) computation. It is designed to be
	 * compliant with similar functionality found in the speech
	 * processing software HTK v3.2.1.
	 *
	 * No normalization or windowing of the spectrum data is performed
	 * prior to analysis.
	 *
	 * @see CLAM::CepstralTransform
	 */
	class MelFilterBank : public Processing
	{
	public:
		MelFilterBank();

		MelFilterBank( const MelFilterBankConfig& );

		~MelFilterBank();

		bool Do();

		/**
		 * Perform the filterbank analysis using the specified number of
		 * bands.
		 *
		 * @param s Input spectrum from FFT transform
		 * @param ms Output filterbank coefficients
		 *
		 * @return True is computation was successfull, false otherwise
		 */
		bool Do(const Spectrum& s, MelSpectrum& ms );

		const ProcessingConfig& GetConfig() const
		{
			return mConfig;
		}

		virtual const char* GetClassName() const
		{
			return "MelFilterBank";
		}

	protected:

		virtual bool ConcreteConfigure( const ProcessingConfig& cfg );

	protected:

		/**
		 * Linear to Mel scale transformation.
		 *
		 * @param linFreq Frequency in Hz.
		 * @return Frequency in Mel.
		 */
		TData Mel( TData linFreq );

		void InitializeTables();

	private:

		MelFilterBankConfig mConfig;
		Array<TIndex>       mMelBand;
		DataArray           mFilterWeights;
		TIndex              mLowIdx;
		TIndex              mHighIdx;
		InPort<Spectrum> mIn;
		OutPort<MelSpectrum> mOut;
	};
}


#endif // MelFilterBank.hxx

