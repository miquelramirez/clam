#ifndef __BASIC_STATISTICS__H__
#define __BASIC_STATISTICS__H__

#include "DataTypes.hxx"

#ifndef NULL
#define NULL ((void*)0)
#endif

/**
 *  This is a C-like interface  to statistical computations on Audio Data.
 *  The interface tries to be very basic, offering a single function to
 *  almost every value computed. 
 *  Generally when doing statistical computations several proporties are
 *  used more than once in different descriptors. Although the interface 
 *  tries to hide this fact, it is actually a interface with two layers.
 *  This means, that some of the functions can be used together with 
 *  already computed values (The default is to compute them again !!).
 *  These precomputable values are in all cases but one the Moments Vector, 
 *  computable with the Moment function.
 *
 *  @author  Guenter Geiger 
 *  @version 0.2
 *
 */




namespace CLAM {

/**
 * A type to hold the Moments of a random process.
 *
 */

typedef TData* Moments;


/**
 * Computes the moments array.
 * @param data     The sample data stored in an C-array
 * @param size     The size of the data array
 * @param moments  Return parameter, the computed moments
 * @param order    Order of moments to be computed, defaults to 4
 * 
 * Multiplying the  second order Moment with the size, gives the
 * Energy measure.
 * Taking the  square root of that yields the RMS.
 * The first order moment is the Magnitude Mean
 */


TData Moment(TData* data,TSize size,Moments moments,TSize order = 4,bool abs = 0 );
TData Energy(Moments moments,TSize size);
TData RMS(Moments moments,TSize size);
TData Mean(Moments moments);

/**
 * Central Moments of a distribution.
 * @param order    Order of central moment
 * @param moments  Array of precomputed raw moments (@see Moment)
 * First order central moment is always 0
 */

TData CentralMoment(int order,Moments moments);


/**
 * Computes the Standard deviation of a distribution.
 * @param moments  Array of precomputed raw moments (@see Moment)
 * The Standard deviation is equal to the square root of the second order 
 * central moment.
 */

TData StandardDeviation(Moments moments);

/**
 * Computes the Variance of a distribution.
 * @param moments  Array of precomputed raw moments (@see Moment)
 * The Variance is equal to the second order central moment.
 */

TData Variance(Moments moments);

/**
 * Computes the Skew of a distribution.
 * @param moments  Array of precomputed raw moments (@see Moment)
 *                 the raw moments have to be at least of order 3.
 */

TData Skew(Moments moments);

/**
 * Computes the Kurtosis of a distribution.
 * @param moments  Array of precomputed raw moments (@see Moment)
 *                 the raw moments have to be at least of order 4.
 */

TData Kurtosis(Moments moments);

/*
 * Compute the Centroid of Random spectral data.
 * @param mag        Array of FFT magnitudes
 * @param size       Size of the  array
 * @param scale      Scale factor (for spectrum this is binwidth, for audio
 *                   data this is the length of the sampling period
 * 
 * @return The Centroid 
 */

TData Centroid(TData* mag, 
	       TSize size,
	       TData scale = 1.0);

/*
 * CentroidMoment computes 1st and higher order centroid moments. 
 * The centroid is the first order spectral moment. There are actually two
 * functions for computing the Centroid Moments. 
 *
 * @param mag        Array of FFT magnitudes
 * @param size       Size of the  array
 * @param order      Order of SpectralMoment.
 * @param scale      Scale factor (for spectrum this is binwidth, for audio
 *                   data this is the length of the sampling period (optional)
 *
 * @param pos        alternatively to the scale value, it is possible to 
 *                   supply an array of positions. (e.g. if the data is not
 *                   evenly distributed over the x axis, 
 *                   Peak data etc. )(optional)
 * @param moments    Array of precomputed raw moments (@see Moment). These have
 *                   to be at least of the same order as the CentroidMoment 
 *                   that is requested.(optional)
 * @param cmoments   Array of returned Centroids (optional).
 * @return           Centroid moment of requested order.
 *                   The cmoments parameter is a array of computed
 *                   centroid moments, up to the  requested order.
 *
 */

TData CentroidMoment(TData*   mag, 
		     TSize    size,
		     TSize    order,
		     TData    scale = 1.0, /* TData* pos */
		     Moments  moments = (TData*) NULL,
		     Moments  cmoments = (TData*) NULL); 

TData CentroidMoment(TData*   mag, 
		     TSize    size,
		     TSize    order,
		     TData*   pos,
		     Moments  moments = (TData*) NULL,
		     Moments  cmoments = (TData*) NULL,
		     bool arr = 1); 


/** Computes the Spectral Tilt
 * @param mag        Array of FFT magnitudes
 * @param size       Size of the array
 * @param scale      Scale value (binwidth in frequency, sample interval)
 * @param pos        Array of positional values (e.g. frequencies of bins)
 * The Spectral Tilt is computed in bin units, which means, to get the
 * Spectral tilt in frequency, one has to DIVIDE it by the bin bandwidth
 * or pass a scale factor.
 */

TData SpectralTilt(TData* mag,
		   TSize size, 
		   TData scale = 1.0,
		   Moments moments = (TData*) NULL);
		   
TData SpectralTilt(TData* mag,
		   TSize size,
		   TData* pos,
		   Moments moments = (TData*) NULL);

/** Computes the Spectral Flatness Measure (SFM)
 * @param mag        Array of FFT magnitudes
 * @param size       Size of the  array
 * @param moments    An array of moments up to order 1 (the mean)
 * This function uses magnitudes instead of Power Spectrum Coeffizients to
 * compute the  SFM
 */

TData SpectralFlatnessMeasure(TData* mag,
			      TSize size,Moments moments = (TData*) NULL);

/** Computes the Tonality Coefficient using the SpectralFlatnessMeasure
 *  @param  sfm    Precomputed SFM measure
 *  @param  sfmmax 
 */

TData TonalityCoefficient(TData sfm, TData sfmmax = -60);

/** Computes the High Frequency Coefficent
 */

TData HighFrequencyCoefficient(TData* mag, TSize size);
  
TSize CepstrumCoefficients(TData* data, TSize size,TData coeff,TSize num);


  /* This is experimental 
   * Usage:
     define a summation kernel that you want to execute 
     with : SUM_KERNEL(mean,out += X[i])

     where mean is the name of the summation function (used as a template
     parameter when calling the summation.

     out += X[i] is the summation kernel.

     Rules for the summation kernel:
        the counting variable is always i
        arrays that can be used are X,Y and Z
        output is the variable out

     this has to be done outside of any function body.

     To use the generated template function:
     Sum<mean>(1024,mag)

  */

  template <class F> TData Sum(int size,TData* X = (TData*)NULL, TData* Y = (TData*)NULL, TData* Z = (TData*)NULL)
  {
    int i;
    TData out = 0;
    for (i=0;i<size;i++) F(out,i,size,X,Y,Z); 
    return out;
  }
    
  #define SUM_KERNEL(name,func) \
  class name  {\
  public: inline name(TData& out,int& i,int size,TData* X1,\
                      TData* X2, TData* X3)\
  {\
     func;\
  }};




} /* namespace */

#endif /* __BASIC_STATISTICS__H__ */
