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
