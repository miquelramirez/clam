/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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


#ifndef _IFFT_
#define _IFFT_

#if defined USE_FFTW3
#include "IFFT_fftw3.hxx"
#define CLAM_FFT_IMPLEMENTATION IFFT_fftw3
#elif defined USE_FFTW
#include "IFFT_rfftw.hxx"
#define CLAM_FFT_IMPLEMENTATION IFFT_rfftw
#else
#include "IFFT_ooura.hxx"
#define CLAM_FFT_IMPLEMENTATION IFFT_ooura
#endif
namespace CLAM
{
class IFFT : public  CLAM_FFT_IMPLEMENTATION
{
public:
	IFFT() : CLAM_FFT_IMPLEMENTATION() {}
	IFFT(const IFFTConfig & config) : CLAM_FFT_IMPLEMENTATION(config) {}
	const char *GetClassName() const {return "IFFT";}
};
}
#undef CLAM_FFT_IMPLEMENTATION

#endif // _IFFT_

