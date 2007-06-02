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

#ifndef __SPECTRUMADAPTER__
#define __SPECTRUMADAPTER__

#include "ProcessingDataAdapter.hxx"
#include "SpectrumModel.hxx"

namespace CLAM
{
		class Spectrum;
		class ProcessingData;
}

namespace CLAMVM
{

		using CLAM::Spectrum;
		using CLAM::ProcessingData;

		class SpectrumAdapter 
				: public ProcessingDataAdapter, public SpectrumModel
		{
				// attributes
		protected:
				const Spectrum*     mObserved;

				// Implementation Details
		protected:

				/**
				 *  This pure virtual method allows to define
				 *  which is the scale used for acquiring the spectrum
				 *  supporting the possibility to make the necessary conversions
				 *  (i.e. the observed Spectrum comes as a Complex Array for expressing
				 *  both magnitude and phase, but you are providing Linear Magnitud and Phase
				 *  - Polar complex numbers in two buffers ). This should try to do the least
				 *  work possible, taking the easiest approach to achieve this effect.
				 *  Look at @see Spectrum class to get an idea of the reasons behind this.
				 */

				virtual void TranslateSpectralData() = 0;

				// Public class interfacs
		public:

				SpectrumAdapter();
				
				virtual ~SpectrumAdapter();
				
				virtual const char* GetClassName() const
				{
						return "SpectrumAdapter";
				}


				virtual bool Publish() = 0;

				virtual bool BindTo( const ProcessingData& procDataObj );
		};

}

#endif // SpectrumAdapter.hxx

