/* header for implementation of the Numerical Recipes in C 
	FFT-Algorithm
	Only uses Powers of Two Framesizes
	n is length of the array.
	data[] starts at data[1] , last element is data[n] 
	isign =1 forward/ isign =-1 backward transform
	
*/
#include "DataTypes.hxx"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

void realft(CLAM::TData data[], unsigned long n, int isign);

#ifdef __cplusplus
}
#endif
