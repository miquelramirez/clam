#ifndef __SPECTRUMMODEL__
#define __SPECTRUMMODEL__

#include "DataTypes.hxx"
#include "Array.hxx"
#include "Signalv1.hxx"
#include "Signalv2.hxx"

namespace CLAMVM
{
		using SigSlot::Signalv1;
		using SigSlot::Signalv2;
		using CLAM::TData;
		using CLAM::DataArray;

		class SpectrumModel
		{
		public:

				SpectrumModel( );
				
				virtual ~SpectrumModel();

				// signals
				Signalv1< const DataArray& >        MagnitudePublished;
				Signalv1< const DataArray& >        PhasePublished;
				Signalv1< TData >                   SpectralRangePublished;
				Signalv2< const DataArray&, TData > ObjectPublished;

		};

}

#endif // SpectrumModel.hxx
