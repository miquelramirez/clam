#ifndef __CLAM_MATH__
#define __CLAM_MATH__

#include <cmath>


//optimized integer rounding routine for Windows
inline int Round(float a) {
#if defined (_MSC_VER)
	int i;
	__asm {
		fld   a
		fistp i
	}
	return i;
#else
	#ifdef __USE_ISOC99
		return lrint(a);
	#else
		return int(rint(a));
	#endif
#endif
}

#ifndef __USE_ISOC99
inline double  round(double _X)
        {return (floor(_X+0.5)); }
inline float  round(float _X)
        {return (floorf(_X+0.5f)); }
#endif



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


#endif // CLAM_Math.hxx
