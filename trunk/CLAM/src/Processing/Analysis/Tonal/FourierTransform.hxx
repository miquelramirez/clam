/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef FourierTransform_hxx
#define FourierTransform_hxx
#include <vector>
#include <fftw3.h>

class FourierTransform {
	std::vector<double> datah; //(=insegment)
	unsigned long mFrameSize;
	bool mIsComplex;
	double mDataNorm;
	double * _realInput;
	fftw_complex * _complexOutput;
	fftw_plan _plan;
public:
	FourierTransform(unsigned long int frameSize, double normalizationFactor, bool isComplex);
	~FourierTransform();

	void doIt(const float * input);
	void doIt(const double * input);
	const std::vector<double> & spectrum() const {return datah;};
};

#endif//FourierTransform_hxx

