#include "SpectrumAdapter.hxx"
#include "Spectrum.hxx"

namespace CLAMVM
{
		using CLAM::TSize;

		SpectrumAdapter::SpectrumAdapter()
				: mObserved( NULL )
		{
		}

		SpectrumAdapter::~SpectrumAdapter()
		{
		}

		bool SpectrumAdapter::BindTo( const ProcessingData& specObj )
		{
				mObserved = dynamic_cast< const Spectrum* >( &specObj );

				if ( !mObserved ) return false;

				return true;
		}

}
