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


/** Class to hold basic statistics related to an array of arbitrary data. Statistics are computed
 *	efficiently and reusing computations whenever possible.
 *	@param abs whether the statistics are performed directly on the values (by default or template
 *	parameter=false) or on the absolute value of the array elements
 *	@param T the array type
 *	@param U the type of the resulting statistics
 */
template <bool abs=false,class T=TData, class U=TData,int initOrder=5> class StatsTmpl
{

public:

/** Only constructor available. We do not want a default constructor because then we could not be sure
 *	that data is consisten and we would have to be constantly be doing checks.*/

	StatsTmpl(const Array<T>* data):mMoments(initOrder,5),mCentralMoments(initOrder,5),mCenterOfGravities(initOrder,5)
	{
		CLAM_ASSERT(data!=NULL,"Stats: A constructed array must be passed");
		mData=data;
		/*we initialize moments up to initOrder-th order, if higher moments are asked for
		arrays are then resized*/
		mMoments.SetSize(initOrder);
		mCentralMoments.SetSize(initOrder);
		mCenterOfGravities.SetSize(initOrder);
		int i;
		for(i=0;i<initOrder;i++)
		{
			mMoments[i]=NULL;
			mCentralMoments[i]=NULL;
			mCenterOfGravities[i]= NULL;
		}
		InitMoment((O<initOrder>*)(0));
	}
	~StatsTmpl()
	{
		for (unsigned i=0;i<mMoments.Size();i++)
		{
			if(mMoments[i]) delete mMoments[i];
		}
		for (unsigned i=0;i<mCentralMoments.Size();i++)
		{
			if(mCentralMoments[i]) delete mCentralMoments[i];
		}
		for (unsigned i=0;i<mCenterOfGravities.Size();i++)
		{
			if(mCenterOfGravities[i]) delete mCenterOfGravities[i];
		}
	}

	/** Method to change data array and reset all previous computations*/
	void SetArray(const Array<T>* data)
	{
		Reset();
		mData=data;
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

	/** Get mean, compute it if necessary*/
	U GetMean()
	{
		//FirstOrder* first;
		return GetMoment(FirstOrder);
	}

	/** Get centroid, compute it if necessary*/
	U GetCentroid()
	{
		return GetCenterOfGravity(FirstOrder);
	}

	/** Get standard deviation, compute it if necessary*/
	U GetStandardDeviation()
	{
		if(!mCentralMoments[2])//instantiate second central moment if not present: we will like to reuse its value
			mCentralMoments[2]=new CentralMoment<2,abs,T,U>();
		return mStdDev(*mData,*dynamic_cast<CentralMoment<2,abs,T,U>*> (mCentralMoments[2]),true);
	}

	/** Get skewness coefficient, compute it if necessary*/
	U GetSkew()
	{
		if(!mCentralMoments[3])//instantiate second central moment if not present: we will like to reuse its value
			mCentralMoments[3]=new CentralMoment<3,abs,T,U>();
		return mSkew(*mData,mStdDev,*dynamic_cast<CentralMoment<3,abs,T,U>*>(mCentralMoments[3]),true);
	}

	/** Get kurtosis, compute it if necessary*/
	U GetKurtosis()
	{
		if(!mCentralMoments[4])//instantiate second central moment if not present: we will like to reuse its value
			mCentralMoments[4]=new CentralMoment<4,abs,T,U>();
		return mKurtosis(*mData,*dynamic_cast<CentralMoment<2,abs,T,U>*>(mCentralMoments[2]),*dynamic_cast<CentralMoment<4,abs,T,U>*>(mCentralMoments[4]),true);
	}

	/** Get variance, compute it if necessary*/
	U GetVariance()
	{
		return GetCentralMoment(SecondOrder);
	}

	/** Get energy, compute it if necessary*/
	T GetEnergy()
	{
		return mEnergy(*mData);
	}

	U GetGeometricMean()
	{
		return mGeometricMean(*mData);
	}

	/** Get rms, compute it if necessary*/
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

		const Array<T>& Y = *mData;
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
	U GetTilt()
	{
		const Array<T>& Y = *mData;
		const TSize size  = mData->Size();
		const U m1 = GetMoment(FirstOrder);

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

	U GetFlatness()
	{
		return 10*log10(GetGeometricMean()/GetMean());
	}

	/** Reset all previously computed values */
	void Reset()
	{
		//Note: we keep previously allocated data, we just reset computations
		for (unsigned i=0;i<mMoments.Size();i++)
			if(mMoments[i]!=NULL) mMoments[i]->Reset();
		for (unsigned i=0;i<mCentralMoments.Size();i++)
			if(mCentralMoments[i]!=NULL) mCentralMoments[i]->Reset();
		for (unsigned i=0;i<mCenterOfGravities.Size();i++)
			if(mCenterOfGravities[i]!=NULL) mCenterOfGravities[i]->Reset();

		mKurtosis.Reset();
		mStdDev.Reset();
		mSkew.Reset();
		mEnergy.Reset();
		mRMS.Reset();
		mGeometricMean.Reset();
		mMaxElement.Reset();
		mMinElement.Reset();
	}

private:



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
			int i;
			for(i=previousSize;i<order;i++) mMoments[i]=NULL;
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
		CentralMoment<order,abs,T,U>* tmpMoment= dynamic_cast<CentralMoment<order,abs,T,U>*> (mCentralMoments[order-1]);

		//first we see if we already have corresponding Raw Moments up to the order demanded
		int i;
		bool existRawMoments=true;
		for(i=0;i<order;i++)
		{
			if(mMoments[i]==NULL)
			{
				existRawMoments=false;
				break;
			}
		}

		if(existRawMoments) //if we do, we will use formula that relates Central Moments with Raw Moments
			return (*tmpMoment)(*mData,mMoments);
		else //if we don't, we will have to compute them
			return (*tmpMoment)(*mData);
	}

	/** Get order-th central moment, order is greater than init order*/
	template<int order> U GetCentralMoment(const O<order>*,StaticTrue&)
	{
		if(order>mCentralMoments.Size())
		{
			int previousSize=mCentralMoments.Size();
			mCentralMoments.Resize(order+1);
			mCentralMoments.SetSize(order+1);
			int i;
			for(i=previousSize;i<order;i++) mCentralMoments[i]=NULL;
		}
		if(mCentralMoments[order-1]==NULL)
		{
			mCentralMoments[order-1]=new CentralMoment<order,abs,T,U>;
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
			int i;
			for(i=previousSize;i<order;i++) mCenterOfGravities[i]=NULL;
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

	const Array<T>* mData;

	/** Dummy pointer used because of some VC6 limitations*/
	Array<T>* pTmpArray;

};





typedef StatsTmpl<> Stats;



};//namespace



#endif

