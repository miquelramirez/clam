#ifndef MYFFT
#define MYFFT
#include <fftw3.h>
#include <math.h>
#include <string.h>

#define Float double
#define TWO_PI (M_PI * 2.0)

/*--------------------------------- Global variables --------------------------------------*/

/*-------------------------------- myfft.c functions  -------------------------------------*/

Float mus_linear_to_db(Float x);
Float mus_radians_to_hz(Float rads);
Float mus_hz_to_radians(Float hz);
Float mus_degrees_to_radians(Float degree);
Float mus_radians_to_degrees(Float rads);
Float mus_db_to_linear(Float x);

void Hanning(Float *win, int n);
void mus_fftw(Float *rl, int n, int dir);
void fourier_spectrum(Float *sf, Float *fft_data, int fft_size, int data_len, Float *window, int win_len);
bool isNaN(double val);

#endif
