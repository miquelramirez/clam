#include "SpectralPeakArrayAspect.hxx"
#include "SpectralPeakArrayAdapter.hxx"

namespace CLAMVM
{
		SpectralPeakArrayAspect::SpectralPeakArrayAspect( SpectralPeakArrayAdapter& view )
				: mAdapter( view )
		{
		}

		SpectralPeakArrayAspect::~SpectralPeakArrayAspect()
		{
		}

		void SpectralPeakArrayAspect::ForceViewRefresh()
		{
				mAdapter.Publish();
		}
		
}
