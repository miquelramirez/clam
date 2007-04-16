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


#ifndef _FFT_
#define _FFT_

#if defined USE_FFTW3
#include "FFT_fftw3.hxx"
#define CLAM_FFT_IMPLEMENTATION FFT_fftw3
#elif defined USE_FFTW
#include "FFT_rfftw.hxx"
#define CLAM_FFT_IMPLEMENTATION FFT_rfftw
#else
#include "FFT_ooura.hxx"
#define CLAM_FFT_IMPLEMENTATION FFT_ooura
#endif
namespace CLAM
{
class FFT : public  CLAM_FFT_IMPLEMENTATION
{
public:
	FFT() : CLAM_FFT_IMPLEMENTATION() {}
	FFT(const FFTConfig & config) : CLAM_FFT_IMPLEMENTATION(config) {}
	const char *GetClassName() const {return "FFT";}
};
}
#undef CLAM_FFT_IMPLEMENTATION

#endif // _FFT_

