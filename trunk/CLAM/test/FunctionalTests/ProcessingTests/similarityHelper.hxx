#ifndef __SIMILARITY_HELPER__
#define __SIMILARITY_HELPER__

#include "Array.hxx"

namespace CLAM
{
	class SpectralPeakArray;
}

namespace CLAMTest
{
	/**
	 *  Evaluates the Pearson R correlation value
	 *  between input arrays. The formula for computing it is as follows:
	 *
	 *  r(x,y) = Cov( x,y ) / stdDeviation(x)*stdDeviation(y);
	 */
	double evaluateSimilarity( CLAM::DataArray& left,
				   CLAM::DataArray& right );

	/**
	 *  Flattens the input spectral peak array into a floating-point values
	 *  array, so its representation is more amenable to be compared by
	 *  the similarity criterion
	 */
	void flattenSpectralPeakArray( const CLAM::SpectralPeakArray& in,
				       CLAM::DataArray& out );
}

#endif // similarityHelper.hxx
