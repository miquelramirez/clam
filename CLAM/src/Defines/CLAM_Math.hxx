#ifndef __CLAM_MATH__
#define __CLAM_MATH__

#include <cmath>
//XA
#ifdef WIN32
#ifdef _DEBUG
#include <float.h>
#endif
#endif
#include "Assert.hxx"


//optimized positive integer chopping routine for Windows, is equivalent to int(a) but much more efficient
inline int Chop(float a) {
#if defined (_MSC_VER)
            int i;
#ifdef _DEBUG
/**IMPORTANT: if in release mode, you are responsible for changing controlfp.
	You must do so outside the loop that actually calls the loop */
			CLAM_ASSERT(a>=0,"Chop function only for positive numbers");
			unsigned int saved;
			saved = _controlfp(_RC_CHOP,_MCW_RC);
#endif //_DEBUG
            __asm {
                        fld   a
						//fadd  half
                        fistp i
            }
#ifdef _DEBUG
			_controlfp(saved, _MCW_RC);
#endif //_DEBUG
            return i;
#else //not WIN32
            return int(a); // just hope it's an intrinsic.
#endif //WIN32
}

/* optimized roundInt implementation valid only for positive integers*/
inline int Round(float a)
{
#if defined (_MSC_VER)
	int i;
	static const float half = 0.5f;
#ifdef _DEBUG
/**IMPORTANT: if in release mode, you are responsible for changing controlfp.
	You must do so outside the loop that actually calls the loop */
	CLAM_ASSERT(a>=0,"Round function only for positive numbers");
	unsigned int saved;
	saved = _controlfp(_RC_CHOP,_MCW_RC);
#endif //_DEBUG
    __asm {
			fld   a
			fadd  half
            fistp i
           }
#ifdef _DEBUG
			_controlfp(saved, _MCW_RC);
#endif //_DEBUG
            return i;
#else
	#ifdef __USE_ISOC99
		return lrint(a);
	#else
		return int(rint(a));
	#endif
#endif
}



#ifndef linux

#ifndef MIN
#define MIN(a,b) ((a<=b)?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) ((a>=b)?(a):(b))
#endif

#endif //linux

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
inline double  round(double _X)
        {return (floor(_X+0.5)); }
inline float  round(float _X)
        {return (floorf(_X+0.5f)); }
#endif


/** Fast "pow" for converting a logarithmic value into linear value ( assumes a log
scale factor of 20 ). Warning, float should be TData but includes should then be changed**/
inline float log2lin( float x )
{

//	static double magic = 1.0 / (20.0 * log10(exp(1.0)))=0.1151292546497;

	return exp( x * 0.1151292546497f );

}

#endif // CLAM_Math.hxx
