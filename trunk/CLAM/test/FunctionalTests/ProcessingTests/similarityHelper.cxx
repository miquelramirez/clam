#include "similarityHelper.hxx"
#include "Assert.hxx"
#include "CLAM_Math.hxx"
#include "SpectralPeakArray.hxx"

namespace CLAMTest
{
	static double evaluateAverage( CLAM::DataArray& left )
	{
		double avg = 0.0;

		for ( int i = 0; i < left.Size(); i++ )
		{
			avg += left[i];
		}

		avg /= double( left.Size() );
		
		return avg;
	}

	static double evaluateCovariance( CLAM::DataArray& left,
					  double leftAverage,
					  CLAM::DataArray& right,
					  double rightAverage )
	{
		CLAM_ASSERT( left.Size() == right.Size(),
			     "Arrays cannot be compared: sizes differ!");

		double cov = 0.0;

		for ( int i = 0; i < left.Size(); i++ )
		{
			cov+= ( left[i]-leftAverage ) * ( right[i] - rightAverage );
		}

		cov /= double( left.Size() );

		return cov;
	}

	static double evaluateVariance( CLAM::DataArray& vector,
					double average )
	{
		CLAM_ASSERT( vector.Size() > 0, "Void vector: variance cannot be evaluated" );
		double var = 0.0;

		for ( int i = 0; i < vector.Size(); i++ )
		{
			double tmp = vector[i]-average;
			tmp *= tmp;
			var += tmp;
		}

		var /= double( vector.Size() );

		return var;
	}

	double evaluateSimilarity( CLAM::DataArray& left,
				   CLAM::DataArray& right )
	{
	
		double leftAverage = evaluateAverage( left );
		double rightAverage = evaluateAverage( right );

		double covLeftRight = evaluateCovariance( left, leftAverage,
							  right, rightAverage );

		double leftVariance = evaluateVariance( left, leftAverage );
		double rightVariance = evaluateVariance( right, rightAverage );

		return covLeftRight/( sqrt(leftVariance)*sqrt(rightVariance) );
	
	}
	
	void flattenSpectralPeakArray( const CLAM::SpectralPeakArray& in,
				       CLAM::DataArray& out )
	{
		CLAM::TSize flattenedLen = in.GetMagBuffer().Size() + in.GetFreqBuffer().Size()
			+ in.GetPhaseBuffer().Size() + in.GetBinPosBuffer().Size() + in.GetBinWidthBuffer().Size();
		
		out.Resize( flattenedLen );
		out.SetSize( flattenedLen );
		
		int i = 0, j = 0;
		
		for ( i = 0; i < in.GetMagBuffer().Size(); i++, j++ )
		{
			out[j] = in.GetMagBuffer()[i];
		}
		
		for ( i = 0; i < in.GetFreqBuffer().Size(); i++, j++ )
		{
			out[j] = in.GetFreqBuffer()[i];
		}

		for ( i = 0; i < in.GetPhaseBuffer().Size(); i++, j++ )
		{
			out[j] = in.GetPhaseBuffer()[i];
		}

		for ( i = 0; i < in.GetBinPosBuffer().Size(); i++, j++ )
		{
			out[j] = in.GetBinPosBuffer()[i];
		}

		for ( i = 0; i < in.GetBinWidthBuffer().Size(); i++, j++ )
		{
			out[j] = in.GetBinWidthBuffer()[i];
		}
				
	}

}
