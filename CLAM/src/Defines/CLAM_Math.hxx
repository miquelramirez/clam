#ifndef __CLAM_MATH__
#define __CLAM_MATH__

#include <cmath>


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
		inline int max(int a, int b) { 
			return (a>=b)? a : b;
		}
		inline int min(int a, int b) { 
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


#endif // CLAM_Math.hxx
