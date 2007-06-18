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

#ifndef _TDFilterGen_
#define _TDFilterGen_

#include "Processing.hxx"
#include "TDFilterKernel.hxx"
#include "OSDefines.hxx"
#include "PolynomialRootsFinder.hxx"

namespace CLAM
{
	class TDFilterGenConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (TDFilterGenConfig, 6, ProcessingConfig);
		/** The processing name **/
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/** The filter type **/
		DYN_ATTRIBUTE (1, public, ETDFilterType, Type);
		/** The Spectral range of the audio **/
		DYN_ATTRIBUTE (2, public, TData, SpectralRange);
		/** The autoregressive filter coefficients */
		DYN_ATTRIBUTE (3, public, DataArray, ACoefficients );
		/** The non-autoregressive filter coefficients */
		DYN_ATTRIBUTE (4, public, DataArray, BCoefficients);
		/** The Sampling Rate **/
		DYN_ATTRIBUTE (5, public, TData, SamplingRate);

	protected:
		void DefaultInit(void);

	};

	class TDFilterGen: public Processing
	{
	private:
		TData				mSpectralRange;
		TData				mSamplingRate;
		ETDFilterType		mType;
		TDFilterGenConfig	mConfig;

	public:

		TDFilterGen();
		
		TDFilterGen( TDFilterGenConfig &cfg );

		~TDFilterGen() {}

		const char* GetClassName(void) const { return "TDFilterGen"; }

		bool Do(void) { return true; }

		bool Do( TDFilterKernel& out );

		const ProcessingConfig &GetConfig(void) const { return mConfig; }

	protected:

		bool ConcreteConfigure( const ProcessingConfig& );

		bool ConcreteStart();

		bool ConcreteStop();

		void CalculateRoots( const DataArray &coeff, Array<Complex> &roots );

		void CalculateFreqs( const Array< Complex > &p, DataArray &freqs, Array< Complex > &pp,
							 bool &hasFreqs, unsigned &nfreqs);

		void CalculateMagnitudeFR( const Array< Complex > &r, DataArray &mag );
	};

}	//namespace

#endif	// TDFilterGen.hxx
