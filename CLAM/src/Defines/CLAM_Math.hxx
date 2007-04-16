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

#ifndef __CLAM_MATH__
#define __CLAM_MATH__

#include <cmath>
#include "DataTypes.hxx"
#include "FastRounding.hxx"


//The following constants are defined also in OSDefines but only for windows and using the #define preprocessor
//directive. It is much better to use const float declarations
const float PI_	=	3.1415926535897932384626433832795028841972;		/* pi */
const float ONE_OVER_PI	=	(0.3183098861837906661338147750939f);
const float TWOPI		=	(6.2831853071795864769252867665590057683943f);		/* 2*pi */
const float ONE_OVER_TWOPI =	(0.15915494309189535682609381638f);
const float PI_2	=		(1.5707963267948966192313216916397514420986f);		/* pi/2 */
const float TWO_OVER_PI	=	(0.636619772367581332267629550188f);
const float LN2		=		(0.6931471805599453094172321214581765680755f);		/* ln(2) */
const float ONE_OVER_LN2 =	(1.44269504088896333066907387547f);
const float LN10	=		(2.3025850929940456840179914546843642076011f);		/* ln(10) */
const float ONE_OVER_LN10 =	(0.43429448190325177635683940025f);
const float LN2_OVER_LN10 = LN2*ONE_OVER_LN10;
const float TIMES20LN2_OVER_LN10 = 20*LN2_OVER_LN10;
const long LONG_OFFSET	=	4096L;
const float FLOAT_OFFSET =	4096.0;
const float HUGE_ = 1.0e8;
const float ROOT2	=		(1.4142135623730950488016887242096980785697f);		/* sqrt(2) */

/** Efficient versions of common functions*/
inline float CLAM_sin(register float x)
{
#ifndef CLAM_OPTIMIZE
	return (float) sin((double)x);
#else
	x *= ONE_OVER_PI;
	register float accumulator, xPower, xSquared;
	register long evenIntPart = ((long)(0.5f*x + 1024.5) - 1024)<<1;
	x -= (float)evenIntPart;
	xSquared = x*x;
	accumulator = 3.14159265358979f*x;
	xPower = xSquared*x;
	accumulator += -5.16731953364340f*xPower;
	xPower *= xSquared;
	accumulator += 2.54620566822659f*xPower;
	xPower *= xSquared;
	accumulator += -0.586027023087261f*xPower;
	xPower *= xSquared;
	accumulator += 0.06554823491427f*xPower;
	return accumulator;
#endif
}

inline float CLAM_cos(register float x)
	{
#ifndef CLAM_OPTIMIZE
	return (float) cos((double)x);
#else
	x *= ONE_OVER_PI;
	register float accumulator, xPower, xSquared;
	
	register long evenIntPart = ((long)(0.5f*x + 1024.5f) - 1024)<<1;
	x -= (float)evenIntPart;
	
	xSquared = x*x;
	accumulator = 1.57079632679490f*x;						/* series for sin(PI/2*x) */
	xPower = xSquared*x;
	accumulator += -0.64596406188166f*xPower;
	xPower *= xSquared;
	accumulator += 0.07969158490912f*xPower;
	xPower *= xSquared;
	accumulator += -0.00467687997706f*xPower;
	xPower *= xSquared;
	accumulator += 0.00015303015470f*xPower;
	return 1.0f - 2.0f*accumulator*accumulator;				/* cos(w) = 1 - 2*(sin(w/2))^2 */
#endif
	}

inline float CLAM_atan(register float x)
	{
#ifndef CLAM_OPTIMIZE
	return (float) atan((double)x);
#else
	register float accumulator, xPower, xSquared, offset;
	
	offset = 0.0f;
	
	if (x < -1.0f)
		{
		offset = -PI_2;
		x = -1.0f/x;
		}
	 else if (x > 1.0f)
		{
		offset = PI_2;
		x = -1.0f/x;
		}
	xSquared = x*x;
	accumulator = 1.0f;
	xPower = xSquared;
	accumulator += 0.33288950512027f*xPower;
	xPower *= xSquared;
	accumulator += -0.08467922817644f*xPower;
	xPower *= xSquared;
	accumulator += 0.03252232640125f*xPower;
	xPower *= xSquared;
	accumulator += -0.00749305860992f*xPower;
	
	return offset + x/accumulator;
#endif
}

inline float CLAM_atan2(float Imag, float Real)
	{
#ifndef CLAM_OPTIMIZE
	return (float) atan2((double)Imag, (double)Real);
#else
	if(Real==0 && Imag==0) return 0.f;
	register float accumulator, xPower, xSquared, offset, x;
		
	if (Imag > 0.0f)
		{
		if (Imag <= -Real)
			{
			offset = PI_;
			x = Imag/Real;
			}
		 else if (Imag > Real)
			{
			offset = PI_2;
			x = -Real/Imag;
			}
		 else
			{
			offset = 0.0f;
			x = Imag/Real;
			}
		}
	 else
		{
		if (Imag >= Real)
			{
			offset = -PI_;
			x = Imag/Real;
			}
		 else if (Imag < -Real)
			{
			offset = -PI_2;
			x = -Real/Imag;
			}
		 else
			{
			offset = 0.0f;
			x = Imag/Real;
			}
		}
	
	xSquared = x*x;
	accumulator = 1.0f;
	xPower = xSquared;
	accumulator += 0.33288950512027f*xPower;
	xPower *= xSquared;
	accumulator += -0.08467922817644f*xPower;
	xPower *= xSquared;
	accumulator += 0.03252232640125f*xPower;
	xPower *= xSquared;
	accumulator += -0.00749305860992f*xPower;
	        
	return offset + x/accumulator;
#endif
}

inline float	CLAM_exp2(register float x)
{
#ifndef CLAM_OPTIMIZE
	return (float) exp(LN2*(double)x);
#else
	if (x >= -127.0f)
		{
		register float accumulator, xPower;
		register union {float f; long i;} xBits;
			
		xBits.i = (long)(x + FLOAT_OFFSET) - LONG_OFFSET;		/* integer part */
		x -= (float)(xBits.i);									/* fractional part */
		
		accumulator = 1.0f + 0.69303212081966f*x;
		xPower = x*x;
		accumulator += 0.24137976293709f*xPower;
		xPower *= x;
		accumulator += 0.05203236900844f*xPower;
		xPower *= x;
		accumulator += 0.01355574723481f*xPower;
		
		xBits.i += 127;											/* bias integer part */
		xBits.i <<= 23;											/* move biased int part into exponent bits */
		
		return accumulator * xBits.f;
		}
	 else
		{
		return 0.0f;
		}
#endif
}

inline float	CLAM_log2(register float x)
{
#ifndef CLAM_OPTIMIZE
	return (float) (ONE_OVER_LN2*log((double)x));
#else
	if (x > 5.877471754e-39f)
		{
		register float accumulator, xPower;
		register long intPart;
		
		register union {float f; long i;} xBits;
		
		xBits.f = x;
		
		intPart = ((xBits.i)>>23);					/* get biased exponent */
		intPart -= 127;								/* unbias it */
		
		x = (float)(xBits.i & 0x007FFFFF);			/* mask off exponent leaving 0x800000*(mantissa - 1) */
		x *= 1.192092895507812e-07f;					/* divide by 0x800000 */
		
		accumulator = 1.44254494359510f*x;
		xPower = x*x;
		accumulator += -0.71814525675041f*xPower;
		xPower *= x;
		accumulator += 0.45754919692582f*xPower;
		xPower *= x;
		accumulator += -0.27790534462866f*xPower;
		xPower *= x;
		accumulator += 0.12179791068782f*xPower;
		xPower *= x;
		accumulator += -0.02584144982967f*xPower;
		
		return accumulator + (float)intPart;
		}
	 else
		{
		return -HUGE_;
		}
#endif
}

inline float CLAM_pow(float x, float y)
{
#ifndef CLAM_OPTIMIZE
	return (float) pow((double)x, (double)y);
#else
	return CLAM_exp2(y*CLAM_log2(x));
#endif
}

inline float CLAM_sqrt(register float x)
	{
#ifndef CLAM_OPTIMIZE
	return (float) sqrt((double)x);
#else
	if (x > 5.877471754e-39f)
		{
		register float accumulator, xPower;
		register long intPart;
		register union {float f; long i;} xBits;
		
		xBits.f = x;
		
		intPart = ((xBits.i)>>23);					/* get biased exponent */
		intPart -= 127;								/* unbias it */
		
		x = (float)(xBits.i & 0x007FFFFF);			/* mask off exponent leaving 0x800000*(mantissa - 1) */
		x *= 1.192092895507812e-07f;					/* divide by 0x800000 */
		
		accumulator =  1.0f + 0.49959804148061f*x;
		xPower = x*x;
		accumulator += -0.12047308243453f*xPower;
		xPower *= x;
		accumulator += 0.04585425015501f*xPower;
		xPower *= x;
		accumulator += -0.01076564682800f*xPower;
		
		if (intPart & 0x00000001)
			{
			accumulator *= ROOT2;					/* an odd input exponent means an extra sqrt(2) in the output */
			}
		
		xBits.i = intPart >> 1;						/* divide exponent by 2, lose LSB */
		xBits.i += 127;								/* rebias exponent */
		xBits.i <<= 23;								/* move biased exponent into exponent bits */
		
		return accumulator * xBits.f;
		}
	 else
		{
		return 0.0f;
		}
#endif
	}

inline float CLAM_log(register float x)
{
#ifndef CLAM_OPTIMIZE
	return (float) log((double)x);
#else
	return LN2*CLAM_log2(x);
#endif
}

inline float CLAM_log10(register float x)
{
#ifndef CLAM_OPTIMIZE
	return (float) log10((double)x);
#else
	return LN2_OVER_LN10*CLAM_log2(x);
#endif
}

inline float CLAM_20log10(register float x)
{
#ifndef CLAM_OPTIMIZE
	return (float) 20*log10((double)x);
#else
	return TIMES20LN2_OVER_LN10*CLAM_log2(x);
#endif
}

inline float CLAM_exp(register float x)
{
#ifndef CLAM_OPTIMIZE
	return (float) exp((double)x);
#else
	return CLAM_exp2(ONE_OVER_LN2*x);
#endif
}

#if defined _MSC_VER && _MSC_VER < 1310 // MSVC++ 6
#undef min
#undef max
	namespace std
	{	
		template < typename T >
		const T& max( const T& a, const T& b) { 
			return (a>=b)? a : b;
		}
		template < typename T >
		const T& min(const T& a, const T& b) { 
			return (a<=b)? a : b;
		}
	} // namespace std
#endif // MSVC++ 6

#if defined _MSC_VER // MSVC++7
	namespace std
	{
		template <typename T>
		bool isnan(T data)
		{
			return _isnan(data) == 1;
		}
		template <typename T>
		bool isinf(T data)
		{
			return _isnan(data) == 1;
		}
	}
#endif // MSVC++ 7

#ifndef __USE_ISOC99
#ifndef __APPLE__
inline double  round(double _X)
        {return (floor(_X+0.5)); }
inline float  round(float _X)
        {return (floorf(_X+0.5f)); }
#endif // __APPLE__
#endif // __USE_ISOC99


/** Fast "pow" for converting a logarithmic value into linear value ( assumes a log
scale factor of 20 ). Warning, float should be TData but includes should then be changed**/
inline float log2lin( float x )
{

//	static double magic = 1.0 / (20.0 * log10(exp(1.0)))=0.1151292546497;

	return CLAM_exp( x * 0.1151292546497f );

}

/**
 * Returns true if the given (unsigned) integer n is
 * a power-of-two.
 * Will return true for n = 0 and n = 1.
 **/
inline bool isPowerOfTwo( CLAM::TUInt32 n)
{
	return (((n - 1) & n) == 0);
}

/**
 * Returns the closest power-of-two number greater or equal
 * to n for the given (unsigned) integer n.
 * Will return 0 when n = 0 and 1 when n = 1.
 **/
inline CLAM::TUInt32 nextPowerOfTwo( CLAM::TUInt32 n)
{
	--n;

	n |= n >> 16;
	n |= n >> 8;
	n |= n >> 4;
	n |= n >> 2;
	n |= n >> 1;
	++n;

	return n;
}

namespace CLAM
{

/*Non member function, returns absolute value of class T*/
template <class T> inline T Abs(T value)
{
	return ( value < 0 ) ? -value : value;
}

/* DB */

// Default scaling
#define CLAM_DB_SCALING  20  

inline double DB(double linData, int scaling=20) 
{ 
	return (scaling*CLAM_log10(linData)); 
}

inline double Lin(double logData, int scaling=20 ) 
{ 
	return (CLAM_pow(double(10),(logData/scaling)) ); 
}

/** Definition of CLAM_min and CLAM_max. Note1: we are not returning a const reference 
 *	because in some specializations this is not possible. Note2: we are not using std::max and 
 *	std::min by default because in Windows these functions are implemented with different names
 */
template<class T> inline
T CLAM_max(const T& x, const T& y)
	{return (x < y ? y : x); }

template<class T> inline
T CLAM_min(const T& x, const T& y)
	{return (x > y ? y : x); }
}

#endif // CLAM_Math.hxx

