#ifndef __CLAM_MATH__
#define __CLAM_MATH__

#include <cmath>

#if  !defined(CLAM_DOUBLE) && !defined(__GNUC__) && (_MSC_VER<1310)

inline long  abs(long _X)
        {return (labs(_X)); }
inline double abs(double _X)
        {return (fabs(_X)); }
inline float  abs(float _X)
        {return (fabsf(_X)); }
inline float  acos(float _X)
        {return (acosf(_X)); }
inline float  asin(float _X)
        {return (asinf(_X)); }
inline float  atan(float _X)
        {return (atanf(_X)); }
inline float  atan2(float _Y, float _X)
        {return (atan2f(_Y, _X)); }
inline float  ceil(float _X)
        {return (ceilf(_X)); }
inline float  cos(float _X)
        {return (cosf(_X)); }
inline float  cosh(float _X)
        {return (coshf(_X)); }
inline float  exp(float _X)
        {return (expf(_X)); }
inline float  fabs(float _X)
        {return (fabsf(_X)); }
inline float  floor(float _X)
        {return (floorf(_X)); }
inline float  fmod(float _X, float _Y)
        {return (fmodf(_X, _Y)); }
inline float  frexp(float _X, int * _Y)
        {return (frexpf(_X, _Y)); }
inline float  ldexp(float _X, int _Y)
        {return (ldexpf(_X, _Y)); }
inline float  log(float _X)
        {return (logf(_X)); }
inline float  log10(float _X)
        {return (log10f(_X)); }
inline float  modf(float _X, float * _Y)
        {return (modff(_X, _Y)); }
inline float  pow(float _X, float _Y)
        {return (powf(_X, _Y)); }
inline float  sin(float _X)
        {return (sinf(_X)); }
inline float  sinh(float _X)
        {return (sinhf(_X)); }
inline float  sqrt(float _X)
        {return (sqrtf(_X)); }
inline float  tan(float _X)
        {return (tanf(_X)); }
inline float  tanh(float _X)
        {return (tanhf(_X)); }

#endif

#ifndef MIN
#define MIN(a,b) ((a<=b)?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) ((a>=b)?(a):(b))
#endif

#endif // CLAM_Math.hxx
