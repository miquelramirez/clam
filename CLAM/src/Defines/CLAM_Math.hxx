#ifndef __CLAM_MATH__
#define __CLAM_MATH__

#include <cmath>
#include "DataTypes.hxx"
#include "FastRounding.hxx"

/*
#ifndef linux

#ifndef MIN
#define MIN(a,b) ((a<=b)?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) ((a>=b)?(a):(b))
#endif

#endif //linux
*/

#if defined _MSC_VER && _MSC_VER < 1310 // MSVC++ 6
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

#ifndef __USE_ISOC99
#ifndef __APPLE__
inline double  round(double _X)
        {return (floor(_X+0.5)); }
inline float  round(float _X)
        {return (floorf(_X+0.5f)); }
#endif
#endif


/** Fast "pow" for converting a logarithmic value into linear value ( assumes a log
scale factor of 20 ). Warning, float should be TData but includes should then be changed**/
inline float log2lin( float x )
{

//	static double magic = 1.0 / (20.0 * log10(exp(1.0)))=0.1151292546497;

	return exp( x * 0.1151292546497f );

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
	return (scaling*log10(linData)); 
}

inline double Lin(double logData, int scaling=20 ) 
{ 
	return (pow(double(10),(logData/scaling)) ); 
}

}

#endif // CLAM_Math.hxx
