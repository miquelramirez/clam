#ifndef _Functors_hxx_
#define _Functors_hxx_

#include <cmath>
//#include "StaticBool.hxx"

//#include "DataTypes.hxx"


namespace CLAM
{
	typedef double TData;
/*
	template <typename baseType>
	struct vectorialExpressionTrait
	{
		typedef StaticTrue isVector;
		typedef baseType expressionType;
	};
	template <typename baseType>
	struct scalarExpressionTrait
	{
		typedef StaticFalse isVector;
		typedef baseType expressionType;
	};
*/
	template <typename ConcreteType> struct unary;
	template <typename ConcreteType> struct binary;

	template <typename fT, typename gT>
	struct composition : public unary< composition<fT,gT> >
	{
		typedef fT fType;
		typedef gT gType;
		typedef typename gT::paramType paramType;
		typedef typename fT::resultType resultType;

		composition()
			: _f(fType()), _g(gType()) {}
		composition(const composition<fType,gType> & other)
			: _f(other._f), _g(other._g) {}
		composition(const fType & f, const gType & g)
			: _f(f), _g(g) {}

		resultType apply(const paramType & x)
		{
			return _f.apply(_g.apply(x));
		}
	private:
		fType _f;
		gType _g;
	};

	template <typename fT, typename g1T, typename g2T>
	struct composition2 : public binary< composition2<fT,g1T,g2T> >
	{
		typedef fT fType;
		typedef g1T g1Type;
		typedef g2T g2Type;
		typedef typename g1T::paramType param1Type;
		typedef typename g2T::paramType param2Type;
		typedef typename fT::resultType resultType;

		composition2()
			: _f(fType()), _g1(g1Type()), _g2(g2Type()) {}
		composition2(const composition2<fType,g1Type,g2Type> & other)
			: _f(other._f), _g1(other._g1), _g2(other._g2) {}
		composition2(const fType & f, const g1Type & g1, const g2Type & g2)
			: _f(f), _g1(g1), _g2(g2) {}

		resultType apply(const param1Type & x1, const param2Type & x2 )
		{
			return _f.apply(_g1.apply(x1),_g2.apply(x2));
		}
	private:
		fType _f;
		g1Type _g1;
		g2Type _g2;
	};

	template <typename fType, typename gType>
	composition<fType,gType> compose(const fType & f, const gType & g)
	{
		return composition<fType,gType>(f,g);
	}

	template <typename fType, typename g1Type, typename g2Type>
	composition2<fType,g1Type,g2Type> compose2(const fType & f, const g1Type & g1, const g2Type & g2)
	{
		return composition2<fType,g1Type,g2Type>(f,g1,g2);
	}

	template <typename ConcreteType>
	struct scalar
	{
	};

	template <typename ConcreteType>
	struct unary : public scalar<ConcreteType>
	{
		typedef TData paramType;
		typedef TData resultType;

		template <typename gType>
		CLAM::composition<ConcreteType,gType> operator()(const gType & g)
		{
			return compose((ConcreteType&)(*this),g);
		}
		resultType operator()(const paramType & x)
		{
			return ((ConcreteType*)this)->ConcreteType::apply(x);
		}
	};


	template <typename ConcreteType>
	struct binary : public scalar<ConcreteType>
	{
		typedef TData param1Type;
		typedef TData param2Type;
		typedef TData resultType;

		template <typename g1Type, typename g2Type>
		CLAM::composition2<ConcreteType,g1Type,g2Type> operator()(const g1Type & g1, const g2Type & g2)
		{
			return compose2((ConcreteType&)(*this),g1,g2);
		}

		resultType operator()(const param1Type & x1, const param2Type & x2)
		{
			return ((ConcreteType*)this)->ConcreteType::apply(x1,x2);
		}
		resultType operator()(const param1Type & x1)
		{
			return ((ConcreteType*)this)->ConcreteType::apply(x1,x1);
		}
	};

	struct sum : public binary<sum>
	{
		resultType apply(const param1Type & x1, const param2Type & x2)
		{
			return x1+x2;
		}
	};

	struct prod : public binary<prod>
	{
		resultType apply(const param1Type & x1, const param2Type & x2)
		{
			return x1*x2;
		}
	};

	struct abs : public unary<abs>
	{
		resultType apply(const paramType & x)
		{
			return std::abs(x);
		}
	};


	struct neg : public unary<neg>
	{
		resultType apply(const paramType & x)
		{
			return -(x);
		}
	};

	template <unsigned n>
	struct pow : public unary<pow<n> >
	{
		typedef TData resultType;
		typedef TData paramType;
		resultType apply(const paramType & x)
		{
			return power(x,(isOdd*)0);
		}
		static resultType squared(const paramType & x)
		{
			return x*x;
		}

		template <unsigned isOdd> class isOddSelector {};
		typedef isOddSelector<(n&1)> isOdd;

		typedef pow<(n>>1)> nextPower;
		typedef typename nextPower::isOdd nextIsOdd;

		static resultType power(const paramType & x, isOddSelector<1> * foo)
		{
			return squared(nextPower::power(x,(nextIsOdd*)0)) * x;
		}
		static resultType power(const paramType & x, isOddSelector<0> * foo)
		{
			return squared(nextPower::power(x,(nextIsOdd*)0));
		}
	};

	template<>
	pow<1>::resultType pow<1>::power(const paramType & x, pow<1>::isOddSelector<1> * foo)
	{
		return x;
	}

	template<>
	pow<0>::resultType pow<0>::power(const paramType & x, pow<0>::isOddSelector<0> * foo)
	{
		return 1;
	}

	typedef pow<1> identity;
	typedef pow<2> squared;
	typedef pow<3> cubed;

	typedef composition2<sum,identity,neg> subst;


}


#endif// _Functors_hxx_

