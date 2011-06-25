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

#ifndef _Stats_
#define _Stats_


#include "BasicOps.hxx"
#include "Array.hxx"
#include <algorithm>

namespace CLAM{


template <unsigned int x,unsigned int y> class GreaterThan
{
	public: static StaticBool<(x>y)> mIs;
};


template <unsigned int x,unsigned int y> StaticBool<(x>y)>  GreaterThan<x,y>::mIs;


/**
 * An StatMemory may hold a T value and remembers whether
 * it has been set or is not initialized.
 * It has two states, value memorized and no value memorized.
 * By default is not memorized until it is assigned to a value that is copied.
 * Then you can query the value using the call operator.
 * By reseting it you are releasing the memory until a new assignement.
 */
template <typename T>
class StatMemory
{
public:
	StatMemory() : mMemorized(false) {}
	const StatMemory & operator = (const T & value)
	{
		mMemorized=true;
		mMemory=value;

		return *this;
	}
	bool HasValue()
	{
		return mMemorized;
	}
	void Reset()
	{
		mMemorized=false;
	}
	operator T()
	{
		CLAM_ASSERT(mMemorized,"Using a value that has not been memorized");
		return mMemory;
	}
private:
	T mMemory;
	bool mMemorized;
};


/** Class to hold basic statistics related to an array of arbitrary data. Statistics are computed
 *  efficiently and reusing computations whenever possible.
 *	@param abs whether the statistics are performed directly on the values (by default or template
 *	parameter=false) or on the absolute value of the array elements
 *	@param T the array type
 *	@param U the type of the resulting statistics
 *	@pre Most stats are not tolerant to size 0 data sets
 */
template <bool abs=false,class T=TData, class U=TData,int initOrder=5> class StatsTmpl
{

public:

/* @internal Only constructor available. We do not want a default constructor because then we could not be sure
 * that data is consisten and we would have to be constantly be doing checks.*/

	StatsTmpl(const Array<T>* data):mMoments(initOrder,5),mCentralMoments(initOrder,5),mCenterOfGravities(initOrder,5)
	{
		CLAM_ASSERT(data!=NULL,"Stats: A constructed array must be passed");
		mData=data;
		/*we initialize moments up to initOrder-th order, if higher moments are asked for
		arrays are then resized*/
		mMoments.SetSize(initOrder);
		mCentralMoments.SetSize(initOrder);
		mCenterOfGravities.SetSize(initOrder);
		for(unsigned i=0;i<initOrder;i++)
		{
			mMoments[i]=NULL;
			mCentralMoments[i]=NULL;
			mCenterOfGravities[i]= NULL;
		}
		InitMoment((O<initOrder>*)(0));
	}
	~StatsTmpl()
	{
		for (int i=0;i<mMoments.Size();i++)
		{
			if(mMoments[i]) delete mMoments[i];
		}
		for (int i=0;i<mCentralMoments.Size();i++)
		{
			if(mCentralMoments[i]) delete mCentralMoments[i];
		}
		for (int i=0;i<mCenterOfGravities.Size();i++)
		{
			if(mCenterOfGravities[i]) delete mCenterOfGravities[i];
		}
	}

	/** Method to change data array and reset all previous computations*/
	void SetArray(const Array<T>* data)
	{
		Reset();
		mData=data;
		mCentroid.Reset();
	}

	/**
	 *	Get order-th raw moment.
	 *	This method just acts as a selector, if order is greater than init order, we cannot assure
	 *	that the pointer has been initialized and we need extra checks (slow downs).
	 */
	template <int order> U GetMoment(const O<order>*)
	{
		return GetMoment((const O<order>*)(0),GreaterThan<order,initOrder>::mIs);
	}

	/** Get all raw moments up to the order indicated*/
	template<int order> void GetMoments(Array<U>& moments, const O<order>*)
	{
		if(moments.Size()<order)
		{
			moments.Resize(order);
			moments.SetSize(order);
		}
		pTmpArray=&moments;
		GetChainedMoment((const O<order>*)(0));
		pTmpArray=NULL;
	}

	/**
	 *	Get order-th central moment.
	 *	This method just acts as a selector, if order is greater than init order, we cannot assure
	 *	that the pointer has been initialized and we need extra checks (slow downs).
	 */
	template <int order> U GetCentralMoment(const O<order>*)
	{
		return GetCentralMoment((const O<order>*)(0),GreaterThan<order,initOrder>::mIs);
	}

	/** Get all central moments up to the order indicated*/
	template<int order> void GetCentralMoments(Array<U>& centralMoments,
		const O<order>*)
	{
		if(centralMoments.Size()<order)
		{
			centralMoments.Resize(order);
			centralMoments.SetSize(order);
		}
		pTmpArray=&centralMoments;
		GetChainedCentralMoment((const O<order>*)(0));
		pTmpArray=NULL;
	}

	/**
	 *	Get order-th center of gravity.
	 *	This method just acts as a selector, if order is greater than init order, we cannot assure
	 *	that the pointer has been initialized and we need extra checks (slow downs).
	 */
	template <int order> U GetCenterOfGravity(const O<order>*)
	{
		return GetCenterOfGravity((const O<order>*)(0),GreaterThan<order,initOrder>::mIs);
	}

	/** Get all center of gravities up to the order indicated*/
	template<int order> void GetCenterOfGravities(Array<U>& centerOfGravities,
		const O<order>*)
	{
		if(centerOfGravities.Size()<order)
		{
			centerOfGravities.Resize(order);
			centerOfGravities.SetSize(order);
		}
		pTmpArray=&centerOfGravities;
		GetChainedCenterOfGravity((const O<order>*)(0));
		pTmpArray=NULL;
	}

	/**
	 * Get mean, compute it if necessary.
	 *
	 * \f[
	 * 	Mean(X) = \frac {\sum x_i} { Size(X) }
	 * \f]
	 */
	U GetMean()
	{
		if (mData->Size()<=0) return U(.0);
		//FirstOrder* first;
		return GetMoment(FirstOrder);
	}

	/**
	 * Get centroid, compute it if necessary.
	 *
	 * The centroid of a function returns the position \f$i\f$ 
	 * around which most higher values are concentrated.
	 *
	 * \f[
	 * 	Centroid(X) = \frac 
	 * 		{\sum i \cdot x_i }
	 * 		{\sum x_i}
	 * \f]
	 *
	 * whenever the Mean(X) is less than 1e-7, then it will return the mid position
	 * \f[
	 * 	\frac{Size(X)-1}{2}
	 * \f]
	 *
	 */
	U GetCentroid()
	{
//		return GetCenterOfGravity(FirstOrder);
		if (mCentroid.HasValue()) return mCentroid;
		unsigned N = mData->Size();
		U mean = GetMean();
		if (mean < 1e-7 ) 
		{
			mCentroid = U(N-1)/2;
			return mCentroid;
		}
		U centroid=0.0;
		for (unsigned i = 0; i < N; i++)
		{
			centroid += (abs?Abs((*mData)[i]):(*mData)[i]) * (i+1);
		}
		mCentroid=centroid/mean/U(N) - 1;
		return mCentroid;
	}

	/**
	 * Computes and returns the Spread arround the Centroid.
	 * \f[
	 * 	Spread(Y) = \frac
	 * 		{\sum_{i=0}^{N-1}{(Centroid(Y)-x_i)^2 y_i} }
	 * 		{ \sum_{i=0}^{N-1}{y_i} }
	 * \f]
	 * The spread gives an idea on how much the distribution
	 * is NOT concentrated over the distribution centroid.
	 * Taking the array as a distribution and the values being probabilities,
	 * the spread would be the variance of such distribution.
	 * 
	 * Significant values:
	 * - For a full concentration on a single bin: 0.0
	 * - For two balanced diracs on the extreme bins
	 * \f[
	 * 	Spread(BalancedDiracsDistribution) = {N^2 \over 4}
	 * \f]
	 * - For a uniform distribution the spread it's:
	 * \f[
	 * 	Spread(UniformDistribution) = \frac{(N-1)(N+1)}{12}
	 * \f]
	 *
	 * Singularities and solution:
	 * - When \f$\sum{y_i}\f$ is less than 1e-14 it return the uniform distribution
	 *   formula above.
	 * - Centroid NaN silence NaN is solved inside GetCentroid
	 * - When Centroid is less than 0.2, 0.2 is taken as the centroid value.
	 *
	 * Normalization: Multiply the result by the square of the gap between
	 * arrays positions. ex. in an array representing a spectrum multiply by
	 * \f$ BinFreq^2 \f$
	 * 
	 * @todo still not tested as stats but tested its usage for SpectralSpread
	 * @todo should use other stats than centroid to save computations
	 * @see GetCentroid
	 */
	U GetSpread()
	{
		const unsigned N = mData->Size();
		const Array<T> & data = *mData;
		const U centroid = GetCentroid();

		// Compute spectrum variance around centroid frequency
		TData variance = 0;
		TData sumMags  = 0;
		for (unsigned i=0; i<N; i++)
		{
			U centroidDistance = i - centroid;
			centroidDistance *= centroidDistance;
			variance += centroidDistance * data[i];
			sumMags  += data[i];
		}
		// NaN solving: Silence is like a plain distribution
		if (sumMags < 1e-14) return U(N+1) * (N-1) / 12;

		return variance / sumMags;
	}

	/** Get standard deviation, compute it if necessary*/
	U GetStandardDeviation()
	{
		return mStdDev(*mData,GetCentralMomentFunctor<2>(),true);
	}

	/**
	 * Get skewness coefficient, compute it if necessary.
	 *
	 * The Skewness of a distribution gives an idea of 
	 * the assimetry of the variance of the values.
	 * @f[
	 * Skewness(X) = \frac
	 * 	{\sum{\left( (x_i-Mean(X))^3\right)} } 
	 * 	{\left(
	 * 		\sum{\left(
	 * 			x_i-Mean(X)
	 * 		\right)^2}
	 * 	\right) ^\frac{3}{2} }
	 * @f]
	 * Tipical values:
	 * - This function returns greater positive values when
	 *   there are more extreme values above the median than below.
	 * - Returns negative values when 
	 *   there are more extreme values below the median than above.
	 * - Returns zero when the distribution of the \f$x_i\f$ 
	 *   values around the Median is equilibrated.
	 *
	 * Singularities and solutions:
	 * - Constant functions: Currently returns NaN but, in the future,
	 *   it should return 0 because it can be considered an 
	 *   equilibrated function.
	 *
	 * @todo Give an order of magnitude, limits or something
	 */
	U GetSkew()
	{
		return mSkew(*mData,mStdDev,GetCentralMomentFunctor<3>(),true);
	}

	/**
	 * Get kurtosis, compute it if necessary.
	 *
	 * The Kurtosis of a distribution gives an idea 
	 * of the degree of pickness of the distribution.
	 * @f[
	 * Kurtosis(X) = \frac
	 * 	{\sum{\left( (x_i-Mean(X))^4\right)} } 
	 * 	{\left(
	 * 		\sum{\left(
	 * 			x_i-Mean(X)
	 * 		\right)^2}
	 * 	\right) ^2 }
	 * @f]
	 *
	 * Tipical values:
	 * - A normal distribution of \f$x_i\f$ values has a kurtosis near to 3.
	 * - A constant distribution has a kurtosis of \f$\frac{-6(n^2+1)}{5(n^2-1)} + 3 \f$
	 *
	 * Singularities and solutions:
	 * - Constant functions: Currently returns 3 althought it is not clear
	 *   that it should be the right one, and it can vary on future implementations.
	 */
	U GetKurtosis()
	{
		return mKurtosis(*mData,GetCentralMomentFunctor<2>(),GetCentralMomentFunctor<4>(),true);
	}

	/**
	 * Get variance, compute it if necessary.
	 *
	 * The variance is the mean cuadratic distance from the mean.
	 * @f[
	 * Variance(X) = \frac
	 * 	{\sum{\left( (x_i-Mean(X))^2\right)} } 
	 * 	{Size(X)}
	 * @f]
	 */
	U GetVariance()
	{
		return GetCentralMoment(SecondOrder);
	}

	/**
	 * Get energy, compute it if necessary.
	 *
	 * @f[
	 * 	Energy(X) = \sum{{x_i}^2 }
	 * @f]
	 * 
	 */
	T GetEnergy()
	{
		return mEnergy(*mData);
	}

	/**
	 * Get the Geometric mean, and computes it if necessary.
	 *
	 * The Geometric mean gives the mean magnitude order.
	 * It converges with the mean when all the values \f$x_i\f$ are closer.
	 *
	 * @f[
	 * 	GeometricMean(X) = {\left( \prod x_i \right)} ^ \frac{1}{Size(X) }
	 * @f]
	 * In order to make the computation cheap, For easy computation, logarithms are used.
	 * @f[
	 * 	\log (GeometricMean(X)) = \frac 
	 * 		{ \sum \log_e x_i }
	 * 		{ Size(X) }
	 * @f]
	 */

	U GetGeometricMean()
	{
		return mGeometricMean(*mData);
	}

	/**
	 * Get the root means square (RMS), compute it if necessary.
	 * \f[
	 * 	\sqrt { \sum{{x_i}^2 } }
	 * \f]
	 * @todo Is it a mean??
	 * */
	T GetRMS()
	{
		return mRMS(*mData);
	}

	/** Get maximum value */
	T GetMax()
	{
		return mMaxElement(*mData);
	}

	/** Get minimum value */
	T GetMin()
	{
		return mMinElement(*mData);
	}

	/**
	 * Computes and returns the Slope.
	 *
	 * The slope gives an idea of the mean pendent on the array:
	 * - Less than zero means that is decreasing
	 * - More than zero means that is increasing
	 * - Zero means that any tendency is the dominant
	 * 
	 * The Slope is defined as:
	 * \f[
	 * 	{1 \over \sum{x_i}}
	 * 	{ N \sum{i x_i } - \sum{i} \sum{x_i}
	 * 		\over
	 * 	{N \sum{i^2} - (\sum{i})^2 }}
	 * \f]
	 *
	 * We can transform this formula into one depending on the Centroid
	 * which is already calculated in order to obtain other stats:
	 * \f[
	 * 	6 {
	 * 		{ 2 Centroid - N + 1}
	 * 			\over
	 * 		{ N (N-1) (N+1)}
	 * 	}
	 * \f]
	 * 
	 * The slope is relative to the array position index.
	 * If you want to give to the array position a dimentional meaning,
	 * (p.e. frequency or time) then you should divide by the gap between array positions.
	 * for example GetSlope/BinFreq for a FFT or GetSlope*SampleRate for an audio
	 *
	 */

	U GetSlope()
	{
		// TODO: Sums where Y is used can be taken from Mean and Centroid

		const TSize size  = mData->Size();

		// \sum^{i=0}_{N-1}(x_i)
//		const TData sumY = GetMean()*size;
		// \sum^{i=0}_{N-1}(i x_i)
//		const TData sumXY = GetCentroid()*GetMean()*size;
		// \sum^{i=0}_{N-1}(i)
//		const TData sumX = (size-1)*size/2.0;
		// \sum^{i=0}_{N-1}(i^2)
//		const TData sumXX = (size-1)*(size)*(size+size-1)/6.0;

		//TData num = size*sumXY - sumX*sumY; 
		// = size Centroid Mean size - (size-1)(size)(size)Mean/2
		// = size^2 mean (Centroid - (size-1)/2)
		//num = size*size*GetMean()*(GetCentroid()-(size-1)/2.0);

		// size*sumXX - sumX*sumX =
		// = size (size-1) size (size+size-1)/6 - (size-1)^2(size)^2/4
		// = size^2 ( (size-1)(size+size-1)/6 - (size-1)^2/4 )
		// = size^2 (size-1)( (size+size-1)/6 - (size-1)/4 )
		// = size^2 (size-1)( (4*size-2) - (3*size-3) )/12
		// = size^2 (size-1) (size+1)/12

		//TData denum = (size*sumXX - sumX*sumX)*sumY;
		// = size mean size^2 (size-1) (size+1) / 12
		// = size^3 mean (size-1) (size+1) / 12
		//denum = size*size*size * GetMean() * (size-1) * (size+1) /12.0;

		// return num/denum;
		// = size^2 mean (Centroid - (size-1)/2) / (size^3 mean (size-1) (size+1) / 12)
		// = (Centroid - (size-1)/2) / (size (size-1) (size+1) /12)
		// = ( 12*centroid - 6*size + 6 ) / ( size (size-1) (size+1) )
		// = 6 (2*centroid - size + 1)) / ( size (size-1) (size+1) )
		return 6*(2*GetCentroid() - size + 1) / (size * (size-1) * (size+1));

	}
	/**
	 * Get flatness, compute it if necessary.
	 *
	 * The flatness is the relation among the geometric mean and the arithmetic mean.
	 *
	 * \f[
	 * 	Flatness(X) = \frac
	 * 		{GeometricMean(X)}
	 * 		{Mean(X)}
	 * \f]
	 *
	 * Singularities and solution:
	 * - When the mean is lower than 1e-20, it is set at 1e-20
	 * - When the geometric mean is lower than 1e-20, it is set at 1e-20
	 * @todo Explain why this is a mesure of the flatness
	 * @bug Singularity solution don't work for non absolute stats
	 */
	U GetFlatness()
	{
		U mean = GetMean();
		U geometricMean = GetGeometricMean();
		if (mean<1e-20) mean=TData(1e-20);
		if (geometricMean<1e-20 ) geometricMean=TData(1e-20);
		return geometricMean/mean;
	}

	/**
	 * Reset all the cached computations.
	 * This method is called automatically if you change the data pointer 
	 * using the SetData method, but it should be called explicitly whenever
	 * the values on that array changes externally.
	 */
	void Reset()
	{
		//Note: we keep previously allocated data, we just reset computations
		for (int i=0;i<mMoments.Size();i++)
			if(mMoments[i]!=NULL) mMoments[i]->Reset();
		for (int i=0;i<mCentralMoments.Size();i++)
			if(mCentralMoments[i]!=NULL) mCentralMoments[i]->Reset();
		for (int i=0;i<mCenterOfGravities.Size();i++)
			if(mCenterOfGravities[i]!=NULL) mCenterOfGravities[i]->Reset();

		mKurtosis.Reset();
		mStdDev.Reset();
		mSkew.Reset();
		mEnergy.Reset();
		mRMS.Reset();
		mGeometricMean.Reset();
		mMaxElement.Reset();
		mMinElement.Reset();
		mCentroid.Reset();
	}

private:
	/**
	 * @warning The implementation of this statistic is numerically unstable.
	 * Don't use it.
	 */
	U GetTilt()
	{
		const Array<T>& Y = *mData;
		const TSize size  = mData->Size();
		const U m1 = GetMean();

		TData d1=0;
		TData d2=0;
		for (unsigned i=0;i<size;i++)
		{
			d1 += i/Y[i];
			d2 += 1/Y[i];
		}

		// ti = m1/ai *(n - (d1/d2))
		// SpecTilt = m1²/ti² * SUM[1/ai *(i-d1/d2)]

		TData SumTi2 = 0;
		TData Tilt = 0;
		for (unsigned i=0;i<size;i++) 
		{
			Tilt += (1/Y[i] *(i-d1/d2));
			TData ti = m1/Y[i]*(i - (d1/d2));
			SumTi2 += ti*ti;
		}

		Tilt*= (m1*m1/SumTi2);
		return Tilt;
	}

	/** Chained method for initializing moments*/
	template<int order> void InitMoment(const O<order>*)
	{
		if(mMoments[order-1]!=NULL)
			delete mMoments[order-1];
		mMoments[order-1]=new Moment<order,abs,T,U>;
		if(mCentralMoments[order-1]!=NULL)
			delete mCentralMoments[order-1];
		mCentralMoments[order-1]=new CentralMoment<order,abs,T,U>;
		if(mCenterOfGravities[order-1]!=NULL)
			delete mCenterOfGravities[order-1];
		mCenterOfGravities[order-1]= new CenterOfGravity<order,abs,T,U>;
		InitMoment((O<order-1>*)(0));
	}

	/** Chained method terminator */
	void InitMoment(O<1>*)
	{
		mMoments[0]=new Moment<1,abs,T,U>;
		mCentralMoments[0]=new CentralMoment<1,abs,T,U>;
		mCenterOfGravities[0]= new CenterOfGravity<1,abs,T,U>;
	}

	/** Get order-th raw moment, order is smaller than init order*/
	template<int order> U GetMoment(const O<order>*,StaticFalse&)
	{
		return (*(dynamic_cast<Moment<order,abs,T,U>*> (mMoments[order-1])))(*mData);
	}

	/** Get order-th raw moment, order is greater than init order*/
	template<int order> U GetMoment(const O<order>*,StaticTrue&)
	{
		if(order>mMoments.Size())
		{
			int previousSize=mMoments.Size();
			mMoments.Resize(order);
			mMoments.SetSize(order);
			for(int i=previousSize;i<order;i++) mMoments[i]=NULL;
		}

		if(mMoments[order-1]==NULL)
		{
			mMoments[order-1]=new Moment<order,abs,T,U>;
		}
		//return GetMoment((const O<order>*)(0),StaticFalse());
		return (*(dynamic_cast<Moment<order,abs,T,U>*> (mMoments[order-1])))(*mData);
	}

	/** Chained method to return moment indicated by order and previous*/
	template<int order> void GetChainedMoment(const O<order>* )
	{
		(*pTmpArray)[order-1]=GetMoment((const O<order>*)(0));
		GetChainedMoment((O<order-1>*)(0));
	}

	/** Chained method terminator*/
	void GetChainedMoment(O<1>* )
	{
		(*pTmpArray)[0]=GetMoment((O<1>*)(0));
	}

	/** Get order-th central moment, order is smaller than init order*/
	template<int order> U GetCentralMoment(const O<order>*,StaticFalse&)
	{
		CentralMoment<order,abs,T,U> & tmpMoment = GetCentralMomentFunctor<order>();

		//first we see if we already have corresponding Raw Moments up to the order demanded
		for(int i=0;i<order;i++)
		{
			//if we don't, we will have to compute them
			if(mMoments[i]==NULL)
				return tmpMoment(*mData);
		}

		// if we do, we will use formula that relates Central Moments with Raw Moments
		return tmpMoment(*mData,mMoments);
	}

	/** Get order-th central moment, order is greater than init order*/
	template<int order> U GetCentralMoment(const O<order>*,StaticTrue&)
	{
		if(order>mCentralMoments.Size())
		{
			const int previousSize=mCentralMoments.Size();
			mCentralMoments.Resize(order+1);
			mCentralMoments.SetSize(order+1);
			for(int i=previousSize; i<order; i++) mCentralMoments[i]=NULL;
		}
		if(mCentralMoments[order-1]==NULL)
		{
			mCentralMoments[order-1] = new CentralMoment<order,abs,T,U>;
		}

		return GetCentralMoment((const O<order>*)(0),StaticFalse());
	}



	/** Chained method to return central moment indicated by order and previous*/
	template<int order> void GetChainedCentralMoment(const O<order>* )
	{
		(*pTmpArray)[order-1]=GetCentralMoment((const O<order>*)(0));
		GetChainedCentralMoment((O<order-1>*)(0));
	}

	/** Chained method terminator*/
	void GetChainedCentralMoment(O<1>* )
	{
		(*pTmpArray)[0]=GetCentralMoment((O<1>*)(0));
	}

	/** Get order-th center of gravity, order is smaller than init order*/
	template<int order> U GetCenterOfGravity(const O<order>*,StaticFalse& orderIsGreater)
	{
		return (*dynamic_cast<CenterOfGravity<order,abs,T,U>*> (mCenterOfGravities[order-1]))(*mData);
	}

	/** Get order-th center of gravity, order is greater than init order*/
	template<int order> U GetCenterOfGravity(const O<order>*,StaticTrue& orderIsGreater)
	{
		if(order>mCenterOfGravities.Size())
		{
			int previousSize=mCenterOfGravities.Size();
			mCenterOfGravities.Resize(order);
			mCenterOfGravities.SetSize(order);
			for(int i=previousSize;i<order;i++) mCenterOfGravities[i]=NULL;
		}
		if(mCenterOfGravities[order-1]=NULL)
		{
			mCenterOfGravities[order-1]=new CenterOfGravity<order,abs,T,U>;
		}

		return GetCenterOfGravity((const O<order>*)(0),StaticFalse());
	}

	/** Chained method to return center of gravity indicated by order and previous*/
	template<int order> void GetChainedCenterOfGravity(const O<order>* )
	{
		(*pTmpArray)[order-1]=GetCenterOfGravity((const O<order>*)(0));
		GetChainedCenterOfGravity((O<order-1>*)(0));
	}

	/** Chained method terminator*/
	void GetChainedCenterOfGravity(O<1>* )
	{
		(*pTmpArray)[0]=GetCenterOfGravity((O<1>*)(0));
	}

	template <unsigned order>
	CentralMoment<order,abs,T,U> & GetCentralMomentFunctor()
	{
		CLAM_ASSERT( signed(order-1) < mCentralMoments.Size(),
			"Calling for a Central Moment order above the configured one");

		typedef CentralMoment<order,abs,T,U> CentralMomentN;
		const unsigned int position = order-1;
		if (!mCentralMoments[position])
			mCentralMoments[position] = new CentralMomentN;
		return *dynamic_cast<CentralMomentN*>(mCentralMoments[position]);
	}


	Array<BaseMemOp*> mMoments;
	Array<BaseMemOp*> mCentralMoments;
	Array<BaseMemOp*> mCenterOfGravities;
	KurtosisTmpl<abs,T,U> mKurtosis;
	SkewTmpl<abs,T,U> mSkew;
	StandardDeviationTmpl<abs,T,U> mStdDev;
	EnergyTmpl<T> mEnergy;
	RMSTmpl<T> mRMS;
	GeometricMeanTmpl<T,U> mGeometricMean;
	ComplexMaxElement<abs,T> mMaxElement;
	ComplexMinElement<abs,T> mMinElement;
	StatMemory<U> mCentroid;

	const Array<T>* mData;

	/** Dummy pointer used because of some VC6 limitations*/
	Array<T>* pTmpArray;

};





typedef StatsTmpl<> Stats;



};//namespace



#endif

