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

#endif // CLAM_Math.hxx
