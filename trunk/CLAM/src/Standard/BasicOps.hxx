#ifndef _BasicOps_
#define _BasicOps_

#include <numeric>
#include <functional>
#include "StaticBool.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"

using std::accumulate;
using std::inner_product;
using std::mem_fun;

/** @todo: 
- There a lot of hacks because of Visual C++ compiler, could be optimized and cleaned-up
	making use of partial specialization. 
- many of these operations could be enhanced to make some sort of "stream-like" operations.*/

namespace CLAM{



/** Auxiliary class to define the order of a moment at compile time*/
template<int order> struct O
{
	public: static O<order> instance;
};

template<int order> O<order>  O<order>::instance;

//A nicer way of referring to the first orders

static O<1>* FirstOrder=NULL;
static O<2>* SecondOrder=NULL;
static O<3>* ThirdOrder=NULL;
static O<4>* FourthOrder=NULL;
static O<5>* FifthOrder=NULL;

template <int o> struct TmplPow
{
public:
	template<class T>
	T operator () (T n) {return n*next(n);}
	TmplPow<o-1> next;
};

template<> struct TmplPow<0>
{
public:
	template<class T>
	T operator() (T n) {return 1;}
};


//template <TData n> const TData TmplPow<n,0>::result=1;
	
/**Binary Operator for use with std::accumulate, for computing Sum(x(i)^n)*/
template <int s=1,class T=TData> class PowerTmpl
{
public:
	PowerTmpl(){}
	T operator() (const T& orig,const T& num)
	{
		return orig+mP(num);
	}
protected:
	TmplPow<s> mP;
};

/** Typedef */
typedef PowerTmpl<>  Power;

/**Binary Operator for use with std::accumulate, for computing Sum(x(i)^1)*/
template<class T=TData> class NoPowerTmpl:public PowerTmpl<1,T>{};
/**Binary Operator for use with std::accumulate, for computing Sum(x(i)^2)*/
template<class T=TData> class SquareTmpl:public PowerTmpl<2,T>{};
/**Binary Operator for use with std::accumulate, for computing Sum(x(i)^3)*/
template<class T=TData> class CubeTmpl:public PowerTmpl<3,T>{};

/** Typedefs */
typedef  NoPowerTmpl<> NoPower;
typedef  SquareTmpl<> Square;
typedef  CubeTmpl<> Cube;

/**Binary Operator for use with std::accumulate, for computing Sum(i*x(i)^n)*/
template <int s=1,class T=TData> class WeightedPower
{
public:
	WeightedPower():i(0){}
	T operator() (const T& orig,const T& num)
	{
		return orig+mP(num)*i++;
	}
protected:
	TIndex i;
	TmplPow<s> mP;
};

/**Binary Operator for use with std::inner_product*/
template <int s=1,class T=TData> class PoweredProduct
{
public:
	PoweredProduct(){}
	T operator() (const T& i1,const T& i2)
	{
		return mP(i1)*i2;
	}
protected:
	TmplPow<s> mP;
};


/**Binary Operator for use with std::accumulate, for computing Sum(i*x(i)^1)*/
template<class T=TData> class WeightedNoPower:public WeightedPower<1,T>{};
/**Binary Operator for use with std::accumulate, for computing Sum(i*x(i)^2)*/
template<class T=TData> class WeightedSquare:public WeightedPower<2,T>{};
/**Binary Operator for use with std::accumulate, for computing Sum(i*x(i)^3)*/
template<class T=TData> class WeightedCube:public WeightedPower<3,T>{};

/**Binary Operator for use with std::accumulate, for computing Sum((x(i)-k)^n)*/
template <int s=1,class T=TData,class U=TData> class BiasedPower
{
public:
	BiasedPower(U imean){mean=imean;};
	U operator() (const U& orig,const T& num)
	{
		return orig+mP(num-mean);
	}
public:
	U mean;
	TmplPow<s> mP;
};

/**Binary Operator for use with std::accumulate, for computing Product(x(i))*/
template <class T=TData> class ProductTmpl
{
public:
	T operator()(const T& orig,const T& num)
	{
		return orig*num;
	}
};

typedef ProductTmpl<> Product;

/** Class used as base for all operators with memory, useful to define array of operators.
 *	@see Stats
 *	@todo: check if polymorphism affects efficiency. */
class BaseMemOp
{
public:
	BaseMemOp():alreadyComputed(false){}
	void Reset(){alreadyComputed=false;}
	virtual ~BaseMemOp(){};
protected:
	bool alreadyComputed;
};

/** Class Function that computes Sum(x(i)^n) using std::accumulate and Power<T,s> BinaryOp
 *	It also has associated memory so operation is not performed more than necessary. */
template <int s,class T=TData> class PoweredSum:public BaseMemOp
{
public:
	PoweredSum():memory(0){}
	T operator()(const Array<T>& a,StaticTrue* b=NULL)
	{
		if(!alreadyComputed)
		{
			memory=(*this)(a,(StaticFalse*)(0));
			alreadyComputed=true;
		}
		return memory;
	}
	T operator()(const Array<T>& a,StaticFalse*)
	{
		return accumulate(a.GetPtr(),a.GetPtr()+a.Size(),T(),mP);
	}
private:
	T memory;
	PowerTmpl<s,T> mP;
};



/** Class Function that computes Sum(x(i)) using std::accumulate and NoPower<T> BinaryOp*/
template<class T=TData> class SumTmpl:public PoweredSum<1,T>{};
/** Class Function that computes Sum(x(i)^2) using std::accumulate and Square<T> BinaryOp*/
template<class T=TData> class SquaredSumTmpl:public PoweredSum<2,T>{};
/** Class Function that computes Sum(x(i)^3) using std::accumulate and Cube<T> BinaryOp*/
template<class T=TData> class CubedSumTmpl:public PoweredSum<3,T>{};

typedef SumTmpl<> Sum;
typedef SquaredSumTmpl<> SquaredSum;
typedef CubedSumTmpl<> CubedSum;

/**Binary Operator for use with std::accumulate, for computing Product(x(i))*/
template <class T=TData> class LogPlusTmpl
{
public:
	T operator()(const T& orig,const T& num)
	{
		return orig+log(Abs(num));
	}
};

typedef LogPlusTmpl<> LogSum;

typedef ProductTmpl<> Product;


/** Class Function for computing logarithmic sum of all data in vector using.
 *	It also has associated memory so operation is not performed more than necessary. */
template <class T=TData> class LogSumTmpl:public BaseMemOp
{
public:
	LogSumTmpl():memory(0){}
	T operator()(const Array<T>& a,StaticTrue* b=NULL)
	{
		if(!alreadyComputed)
		{
			memory=(*this)(a,(StaticFalse*)(0));
			alreadyComputed=true;
		}
		return memory;
	}
	T operator()(const Array<T>& a,StaticFalse*)
	{
		return accumulate(a.GetPtr(),a.GetPtr()+a.Size(),T(1),LogPlusTmpl<T>());
		
	}
private:
	T memory;
};


/** Class Function for computing product of all data in vector.
 *	It also has associated memory so operation is not performed more than necessary. */
template <class T=TData> class InnerProductTmpl:public BaseMemOp
{
public:
	InnerProductTmpl():memory(0){}
	T operator()(const Array<T>& a,StaticTrue* b=NULL)
	{
		if(!alreadyComputed)
		{
			memory=(*this)(a,(StaticFalse*)(0));
			alreadyComputed=true;
		}
		return memory;
	}
	T operator()(const Array<T>& a,StaticFalse*)
	{
		return accumulate(a.GetPtr(),a.GetPtr()+a.Size(),T(1),std::multiplies<T>());
		
	}
private:
	T memory;
};

typedef InnerProductTmpl<> InnerProduct;

/** Class Function that computes Sum(i*x(i)^n) using std::accumulate and WeightedPower<T,s> BinaryOp
 *	It also has associated memory so operation is not performed more than necessary. */
template <int s, class T=TData> class WeightedPoweredSum:public BaseMemOp
{
public:
	WeightedPoweredSum():memory(0){}
	T operator()(const Array<T>& a,StaticTrue* b=NULL)
	{
		if(!alreadyComputed)
		{
			memory=(*this)(a,(StaticFalse*)(0));
			alreadyComputed=true;
		}
		return memory;
		
	}
	T operator()(const Array<T>& a,StaticFalse*)
	{
			return accumulate(a.GetPtr(),a.GetPtr()+a.Size(),T(),mWP);
	}
private:
	T memory;
	WeightedPower<s,T> mWP;

};

/** Class Function that computes Sum(i*x(i)^n) using std::accumulate and WeightedPower<T,s> BinaryOp
 *	It also has associated memory so operation is not performed more than necessary. */
template <int s, class T=TData> class CrossWeightedPoweredSum:public BaseMemOp
{
public:
	CrossWeightedPoweredSum():memory(0){}
	T operator()(const Array<T>& a1,const Array<T>& a2,StaticTrue* b=NULL)
	{
		if(!alreadyComputed)
		{
			memory=(*this)(a1,a2,(StaticFalse*)(0));
			alreadyComputed=true;
		}
		return memory;
		
	}
	T operator()(const Array<T>& a1,const Array<T>& a2,StaticFalse*)
	{
		return inner_product(a1.GetPtr(),a1.GetPtr()+a1.Size(),a2.GetPtr(),T(),std::plus<T>(),PoweredProduct<s,T>());	
	}
private:
	T memory;
	WeightedPower<s,T> mWP;

};


/** Class Function that computes Sum(i*x(i)) using std::accumulate and WeightedNoPower<T> BinaryOp*/
template<class T=TData> class WeightedSumTmpl:public WeightedPoweredSum<1,T>{};
/** Class Function that computes Sum(i*x(i)^2)using std::accumulate and WeightedSquare<T> BinaryOp*/
template<class T=TData> class WeightedSquaredSumTmpl:public WeightedPoweredSum<2,T>{};
/** Class Function that computes Sum(i*x(i)^3) using std::accumulate and WeightedCube<T> BinaryOp*/
template<class T=TData> class WeightedCubedSumTmpl:public WeightedPoweredSum<3,T>{};

typedef WeightedSumTmpl<> WeightedSum;
typedef WeightedSquaredSumTmpl<> WeightedSquaredSum;
typedef WeightedCubedSumTmpl<> WeightedCubedSum;

/** Class Function that computes 'oth' order Moment using PoweredSum Class Function.
 *	It also has associated memory so operation is not performed more than necessary. */
template<int o, class T=TData,class U=TData> class Moment:public BaseMemOp
{
public:
	Moment():memory(0){}
	U operator()(const Array<T>& a,PoweredSum<o,T>& powSum,StaticTrue* b=NULL)
	{
		if(!alreadyComputed)
		{
			memory=(*this)(a,powSum,(StaticFalse*)(0));
			alreadyComputed=true;
		}
		return memory;
	}
	U operator()(const Array<T>& a,PoweredSum<o,T>& powSum,StaticFalse*)
	{
		return static_cast<U>(powSum(a))/a.Size();
	}
	/**No powered sum previously computed, use member*/
	U operator()(const Array<T>& a,StaticFalse*)
	{
		return (*this)(a,mPs,(StaticFalse*)(0));
	}
	U operator()(const Array<T>& a,StaticTrue* b=NULL)
	{
		return (*this)(a,mPs,(StaticTrue*)(0));
	}
	
protected:
	U memory;
	PoweredSum<o,T> mPs;
};


/** Class Function that computes 'oth' order Center of Gravity using WeightedPoweredSum Class Function.
 *	It also has associated memory so operation is not performed more than necessary. */
template<int o, class T=TData,class U=TData> class CenterOfGravity:public BaseMemOp
{
public:
	CenterOfGravity():memory(0){}
	U operator()(const Array<T>& a,WeightedPoweredSum<o,T>& wPowSum,PoweredSum<o,T>& PowSum,StaticTrue* b=NULL)
	{
		if(!alreadyComputed)
		{
			memory=(*this)(a,(StaticFalse*)(0));
			alreadyComputed=true;
		}
		return memory;
	}
	U operator()(const Array<T>& a,WeightedPoweredSum<o,T>& wPowSum,PoweredSum<o,T>& PowSum,StaticFalse*)
	{
		return static_cast<U>(wPowSum(a))/PowSum(a);
	}
	/**No weighted powered sum and powered sum previously computed, use member*/
	U operator()(const Array<T>& a,StaticFalse*)
	{
		return (*this)(a,mWPS,mPS,(StaticFalse*)(0));
	}
	U operator()(const Array<T>& a,StaticTrue* b=NULL)
	{
		return (*this)(a,mWPS,mPS,(StaticTrue*)(0));
	}

protected:
	U memory;
	WeightedPoweredSum<o,T> mWPS;
	PoweredSum<o,T> mPS;
};

/** Special CenterOfGravity using 2ond sequence as indexing sequence */
template<int o, class T=TData,class U=TData> class CrossCenterOfGravity:public BaseMemOp
{
public:
	CrossCenterOfGravity():memory(0){}
	U operator()(const Array<T>& a1,const Array<T>& a2,CrossWeightedPoweredSum<o,T>& cwPowSum,PoweredSum<o,T>& powSum,StaticTrue* b=NULL)
	{
		if(!alreadyComputed)
		{
			memory=(*this)(a1,a2,cwPowSum,powSum,(StaticFalse*)(0));
			alreadyComputed=true;
		}
		return memory;
	}
	U operator()(const Array<T>& a1,const Array<T>& a2,CrossWeightedPoweredSum<o,T>& cwPowSum,PoweredSum<o,T>& powSum,StaticFalse*)
	{
		return static_cast<U>(cwPowSum(a1,a2))/powSum(a1);
	}
	/**No weighted powered sum and powered sum previously computed, use member*/
	U operator()(const Array<T>& a1,const Array<T>& a2,StaticFalse*)
	{
		return (*this)(a1,a2,mWPS,mPS,(StaticFalse*)(0));
	}
	U operator()(const Array<T>& a1,const Array<T>& a2,StaticTrue* b=NULL)
	{
		return (*this)(a1,a2,mWPS,mPS,(StaticTrue*)(0));
	}

protected:
	U memory;
	CrossWeightedPoweredSum<o,T> mWPS;
	PoweredSum<o,T> mPS;
};


/** Centroid is 1st order center of gravity
 *	@see: CenterOfGravity */
template<class T=TData,class U=TData> class CentroidTmpl:public CenterOfGravity<1,T,U>{};
/** Mean is 1st order Moment
 *	@see: Moment */
template<class T=TData,class U=TData> class MeanTmpl:public Moment<1,T,U>{};
/** Energy is just the squared sum
 *	@see: SquaredSum */
template<class T=TData> class EnergyTmpl:public SquaredSumTmpl<T>{};

typedef CentroidTmpl<> Centroid;
typedef MeanTmpl<> Mean;
typedef EnergyTmpl<> Energy;


/** RMS is the square root of the Energy
 *	@see: Energy */
template<class T=TData,class U=TData> class RMSTmpl:public BaseMemOp
{
public:
	U operator()(const Array<T>& a,SquaredSumTmpl<T>& sqrSum,StaticTrue* b=NULL)
	{
		if(!alreadyComputed)
		{
			memory=(*this)(a,(StaticFalse*)(0));
			alreadyComputed=true;
		}
		return memory;
	}
	U operator()(const Array<T>& a,SquaredSumTmpl<T>& sqrSum,StaticFalse* b)
	{
		return sqrt(sqrSum(a,(StaticFalse*)(0)));
	}
	/**No squared sum previously computed, use member*/
	U operator()(const Array<T>& a,StaticTrue* b=NULL)
	{
		return (*this)(a,mSS,(StaticTrue*)(0));
	}
	U operator()(const Array<T>& a,StaticFalse* b)
	{
		return (*this)(a,mSS,(StaticFalse*)(0));
	}
private:
	U memory;
	SquaredSumTmpl<T> mSS;

};

typedef RMSTmpl<> RMS;

/** Class Function that computes Geometric Mean using InnerProduct class function.
 *	It also has associated memory so operation is not performed more than necessary. */
template<class T=TData,class U=TData> class GeometricMeanTmpl:public BaseMemOp
{
public:
	U operator()(const Array<T>& a,LogSumTmpl<T>& inProd,StaticTrue* b=NULL)
	{
		if(!alreadyComputed)
		{
			memory=(*this)(a,(StaticFalse*)(0));
			alreadyComputed=true;
		}
		return memory;
	}
	U operator()(const Array<T>& a,LogSumTmpl<T>& inProd,StaticFalse*)
	{
		return exp(inProd(a,(StaticFalse*)(0))*1.0/(double)a.Size());
	}
	/**No inner product previously computed, use temporary*/
	U operator()(const Array<T>& a,StaticTrue* b=NULL)
	{
		return (*this)(a,mIP,(StaticTrue*)(0));
	}
	U operator()(const Array<T>& a,StaticFalse*)
	{
		return (*this)(a,mIP,(StaticFalse*)(0));
	}
	
private:
	U memory;
	LogSumTmpl<T> mIP;

};

typedef GeometricMeanTmpl<> GeometricMean;

/** Class Function that computes Sum((x(i)-mean)^n) using std::accumulate and BiasedPower<T,s> BinaryOp
 *	It also has associated memory so operation is not performed more than necessary. */
template <int s,class T=TData,class U=TData> class BiasedPoweredSum:public BaseMemOp
{
public:
	BiasedPoweredSum():memory(0){}
	U operator()(const Array<T>& a,MeanTmpl<T,U>& imean,StaticTrue* b=NULL)
	{
		if(!alreadyComputed)
		{
			memory=(*this)(a,(StaticFalse*)(0));
			alreadyComputed=true;
		}
		return memory;
	}
	U operator()(const Array<T>& a,MeanTmpl<T,U>& imean,StaticFalse*)
	{
		return accumulate(a.GetPtr(),a.GetPtr()+a.Size(),U(),BiasedPower<s,T,U>(imean(a)));
	}

	/** if no mean is passed, used member functor*/
	U operator()(const Array<T>& a,StaticTrue* b=NULL)
	{
		return (*this)(a,mM,(StaticTrue*)(0));
	}
	U operator()(const Array<T>& a,StaticFalse*)
	{
		return (*this)(a,mM,(StaticFalse*)(0));
	}
private:
	U memory;
	MeanTmpl<T,U> mM;
};



/** Class Function that computes 'oth' order CentralMoment using BiasedPoweredSum Class Function.
 *	It can also compute Central Moments using Previously Computed raw Moments.
 *	It also has associated memory so operation is not performed more than necessary. */
template<int o,class T=TData,class U=TData> class CentralMoment:public BaseMemOp
{
public:
	CentralMoment():memory(){}
	U operator()(const Array<T>& a,BiasedPoweredSum<o,T,U>& bps,StaticTrue* b=NULL)
	{
		if(!alreadyComputed)
		{
			memory=(*this)(a,bps,(StaticFalse*)(0));
			alreadyComputed=true;
		}
		return memory;
	}
	U operator()(const Array<T>& a,BiasedPoweredSum<o,T,U>& bps,StaticFalse*)
	{
		return static_cast<U>(bps(a))/a.Size();
	}
	/** No BiasedPoweredSum passed, use member*/
	U operator()(const Array<T>& a,StaticTrue* b=NULL)
	{
		return (*this)(a,mBPS,(StaticTrue*)(0));
	}
	U operator()(const Array<T>& a,StaticFalse*)
	{
		return (*this)(a,mBPS,(StaticFalse*)(0));
	}
	
	
	/** Compute central moments using raw moments*/
	U operator()(const Array<T>& a,Array<BaseMemOp*>& moments,StaticTrue* b=NULL)
	{
		if(!alreadyComputed)
		{
			memory=(*this)(a,moments,(StaticFalse*)(0));
			alreadyComputed=true;
		}
		return memory;
	}
	U operator()(const Array<T>& a,Array<BaseMemOp*>& moments,StaticFalse*)
	{
		CLAM_DEBUG_ASSERT(moments.Size()>=o,"Central Moment: you need as many raw moments as the order of the central moment you want to compute");
		return (*this)(a,moments,(O<o>*)(0));
	}
	
	U operator()(const Array<T>& a,Array<BaseMemOp*>& moments,O<1>*)
	{
		return 0;
	}

	U operator()(const Array<T>& a,Array<BaseMemOp*>& moments,O<2>*)
	{
		/* -m1� + m2 */ 
		TData m1 = (*(dynamic_cast<Moment<1,T,U>*>(moments[0])))(a);
		TData m2 = (*(dynamic_cast<Moment<2,T,U>*>(moments[1])))(a);
		return (-1)*m1*m1 + m2;
	}

	U operator()(const Array<T>& a,Array<BaseMemOp*>& moments,O<3>*)
	{
	  /* 2*m1� - 3*m1*m2 + m3 =   ... 5 Mult  
	 m1*(2*m1� - 3*m2) + m3   ... 4 Mult */
		TData m1 = (*(dynamic_cast<Moment<1,T,U>*>(moments[0])))(a);
		TData m2 = (*(dynamic_cast<Moment<2,T,U>*>(moments[1])))(a);
		TData m3 = (*(dynamic_cast<Moment<3,T,U>*>(moments[2])))(a);
		return m1*(2*m1*m1 - 3*m2) + m3;
	}

	U operator()(const Array<T>& a,Array<BaseMemOp*>& moments,O<4>*)
	{
		/* -3*m1^4 + 6*m1�*m2 - 4*m1*m3 + m4   = ... 9 Mult 
			 m1*(m1*((-3)*m1� + 6*m2) - 4*m3) + m4 ... 6 Mult */
		TData m1 = (*(dynamic_cast<Moment<1,T,U>*>(moments[0])))(a);
		TData m2 = (*(dynamic_cast<Moment<2,T,U>*>(moments[1])))(a);
		TData m3 = (*(dynamic_cast<Moment<3,T,U>*>(moments[2])))(a);
		TData m4 = (*(dynamic_cast<Moment<4,T,U>*>(moments[3])))(a);
		return m1*(m1*((-3)*m1*m1 + 6*m2) - 4*m3) + m4;
	}

	U operator()(const Array<T>& a,Array<BaseMemOp*>& moments,O<5>*)
	{
	  /* 4*u1^5 - 10*u1�*u2 + 10*u1�*u3 - 5*u1*u4+u5    = .... 14 Mult
		 u1*(u1*(u1*(4*u1� - 10*u2) + 10*u3) - 5*u4) + u5 .... 8 Mult   */
		TData m1 = (*(dynamic_cast<Moment<1,T,U>*>(moments[0])))(a);
		TData m2 = (*(dynamic_cast<Moment<2,T,U>*>(moments[1])))(a);
		TData m3 = (*(dynamic_cast<Moment<3,T,U>*>(moments[2])))(a);
		TData m4 = (*(dynamic_cast<Moment<4,T,U>*>(moments[3])))(a);
		TData m5 = (*(dynamic_cast<Moment<5,T,U>*>(moments[4])))(a);

		return m1*(m1*(m1*(4*m1*m1 - 10*m2) + 10*m3) - 5*m4) + m5;
	}
	
protected:
	BiasedPoweredSum<o,T,U> mBPS;
	U memory;
	
};


/** Class Function to compute StandardDeviation. That is square root of second order
 *	central moment.*/
template <class T=TData, class U=TData> class StandardDeviationTmpl:public BaseMemOp
{
public:
	StandardDeviationTmpl():memory(){}
	U operator()(const Array<T>& a,CentralMoment<2,T,U>& centralMoment2,bool useMemory=false)
	{
		if(!useMemory)
		{
			return sqrt(centralMoment2(a));
		}
		else
		{
			if(!alreadyComputed)
			{
				memory=(*this)(a,false);
				alreadyComputed=true;
			}
			return memory;
		}

	}
	/**2nd order central moment is not available elsewhere, use member*/
	U operator()(const Array<T>& a,bool useMemory=false)
	{
		return (*this)(a,mCM2,useMemory);

	}


protected:
	U memory;
	CentralMoment<2,T,U> mCM2;

};

typedef StandardDeviationTmpl<> StandardDeviation;

/** Class Function that computes Skewness using Standard Deviation and 3rd order central moment.
 *	It also has associated memory so operation is not performed more than necessary. */
template <class T=TData,class U=TData> class SkewTmpl:public BaseMemOp
{
public:
	SkewTmpl():memory(){}
	U operator()(const Array<T>& a,StandardDeviationTmpl<T,U>& std,CentralMoment<3,T,U>& ctrMnt3,bool useMemory=false)
	{
		if(!useMemory)
		{
			U tmpStd=std(a);
			return ctrMnt3(a)/(tmpStd*tmpStd*tmpStd);
		}
		else
		{
			if(!alreadyComputed)
			{
				memory=(*this)(a,false);
				alreadyComputed=true;
			}
			return memory;
		}

	}
	/**Standard deviation and 3rd order central moment are not available, use members*/
	U operator()(const Array<T>& a,bool useMemory=false)
	{
		return (*this)(a,mSD,mCM3,useMemory);
	}

protected:
	U memory; 
	StandardDeviationTmpl<T,U> mSD;
	CentralMoment<3,T,U> mCM3;
};

typedef SkewTmpl<> Skew;

/** Class Function that computes Skewness using Variance and 4th order central moment.
 *	It also has associated memory so operation is not performed more than necessary. */
template <class T=TData,class U=TData> class KurtosisTmpl:public BaseMemOp
{
public:
	KurtosisTmpl():memory(){}
	U operator()(const Array<T>& a,CentralMoment<2,T,U>& var,CentralMoment<4,T,U>& ctrMnt4,bool useMemory=false)
	{
		if(!useMemory)
		{
			U tmpVar=var(a);
			return ctrMnt4(a)/(tmpVar*tmpVar)-3;
		}
		else
		{
			if(!alreadyComputed)
			{
				memory=(*this)(a,false);
				alreadyComputed=true;
			}
			return memory;
		}

	}
	/**Variance and 4th order central moment are not available, use temporary*/
	U operator()(const Array<T>& a,bool useMemory=false)
	{
		return (*this)(a,mCM2,mCM4,useMemory);
	}

protected:
	U memory;
	CentralMoment<2,T,U> mCM2;
	CentralMoment<4,T,U> mCM4;
};

typedef KurtosisTmpl<> Kurtosis;
  
};

#endif// _BasicOps_
