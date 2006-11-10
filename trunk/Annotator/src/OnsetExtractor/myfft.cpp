/**************************************************************

 myfft provides functions for handling the FFT. Main function is: 

 void fourier_spectrum(Float *sf, Float *fft_data, int fft_size, 
		     int data_len, Float *window, int win_len)

 abs(FFT(sf)) is computed with this function. Requires that FFTW 
 is installed.

***************************************************************/

#include "myfft.h"
#include "params.h"
#include <iostream>

using namespace std;

static double *rdata = NULL, *idata = NULL;
static fftw_plan rplan, iplan;
static int last_fft_size = 0;
static Float w_rate = (TWO_PI / 22050.0);


/*--------------------------------------------

 Conversion functions

---------------------------------------------*/

Float mus_linear_to_db(Float x) {
	if (x > 0.0) return(20.0 * log10(x)); 
	return(-200.0);
}

Float mus_radians_to_hz(Float rads) {return(rads / w_rate);}
Float mus_hz_to_radians(Float hz) {return(hz * w_rate);}
Float mus_degrees_to_radians(Float degree) {return(degree * TWO_PI / 360.0);}
Float mus_radians_to_degrees(Float rads) {return(rads * 360.0 / TWO_PI);}
Float mus_db_to_linear(Float x) {return(pow(10.0, x / 20.0));}


/*--------------------------------------------

 IsNaN returns 1 if the input number is a NaN.

-------------------------------------------*/

bool isNaN(double val) {
	return (isnan(val) || isinf(val));
}

/*---------------------------------------------

 Create Hanning window

----------------------------------------------*/

void Hanning(Float *window, int size) {
	for (int i=1; i<=size; i++)
		window[i-1]=0.5*(1-cos(2*M_PI*((double)i/(double)(size+1))));
}

/*--------------------------------------------

 Apply FFTW

---------------------------------------------*/

void mus_fftw(Float *rl, int n, int dir)
{
  int i;
  if (n != last_fft_size)
    {
      if (rdata) {fftw_free(rdata); fftw_free(idata); fftw_destroy_plan(rplan); fftw_destroy_plan(iplan);}
      rdata = (double *)fftw_malloc(n * sizeof(double));
      idata = (double *)fftw_malloc(n * sizeof(double));
      rplan = fftw_plan_r2r_1d(n, rdata, idata, FFTW_R2HC, FFTW_ESTIMATE); 
      iplan = fftw_plan_r2r_1d(n, rdata, idata, FFTW_HC2R, FFTW_ESTIMATE);
      last_fft_size = n;
    }
  memset((void *)idata, 0, n * sizeof(double));
  for (i = 0; i < n; i++) {rdata[i] = rl[i];}
  if (dir != -1)
    fftw_execute(rplan);
  else fftw_execute(iplan);
  for (i = 0; i < n; i++) rl[i] = idata[i];
}

/*---------------------------------------------
 
 Spectrum calculation, using mus_fftw

---------------------------------------------*/

void fourier_spectrum(Float *sf, Float *fft_data, int fft_size, int data_len, Float *window, int win_len)
{
  int i;

  if (window)
  {
      for (i = 0; i < win_len; i++)
	fft_data[i] = window[i] * sf[i]; 

      for (i=win_len; i<data_len; i++)
	fft_data[i] = sf[i];
  }
  else
  {
      for (i = 0; i < data_len; i++)
	fft_data[i] = sf[i];
  }

  if (data_len < fft_size) 
      memset((void *)(fft_data + data_len), 0, (fft_size - data_len) * sizeof(Float));

  int j;
  mus_fftw(fft_data, fft_size, 1);

  fft_data[0] = fabs(fft_data[0]);
  fft_data[fft_size / 2] = fabs(fft_data[fft_size / 2]);

  for (i = 1, j = fft_size - 1; i < fft_size / 2; i++, j--) {
     fft_data[i] = hypot(fft_data[i], fft_data[j]);
  }

  // Correct NaN values

  for (int i=0; i<fft_size/2; i++)
	if (isNaN(fft_data[i]))
		fft_data[i]=0.0;
}

