/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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


#include "FFT.hxx"
#include <string>
#include "ErrProcessingObj.hxx"

namespace CLAM {

	const std::string UnnamedFFT("UnnamedFFT");

	void FFTConfig::DefaultInit(void)
	{
		AddName();
		AddAudioSize();
		UpdateData();
		SetAudioSize(0);
	}


	FFT_base::FFT_base() :
		mSize(0),
		mInput("Input",this,1),
		mOutput("Output",this,1)
	{
	};

	FFT_base::~FFT_base()
	{
	}

	void FFT_base::ChangeSize(int n)
	{
		CLAM_ASSERT(n>=0,"Wrong (negative) Size in control input.");

		throw(ErrProcessingObj("Controls not yet implemented.",this));

	}


};//namespace CLAM
