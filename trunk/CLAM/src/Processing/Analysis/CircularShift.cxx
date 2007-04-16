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

#include "Audio.hxx"
#include "Spectrum.hxx"
#include "Complex.hxx"
#include "CircularShift.hxx"

namespace CLAM {



	/* Processing  object Method  implementations */

	CircularShift::CircularShift()
		: mInput( "Input samples", this ),
		  mOutput( "Shifted samples", this ),
		  mAmount("Amount",this)
		
	{
		Configure(CircularShiftConfig());
	}

	CircularShift::CircularShift(const CircularShiftConfig &c)
		: mInput( "Input samples", this ),
		  mOutput( "Shifted samples", this ),
		  mAmount("Amount",this)
	{
		Configure(c);
	}

	CircularShift::~CircularShift()
	{}


	/* Configure the Processing Object according to the Config object */

	bool CircularShift::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		mAmount.DoControl(TData(mConfig.GetAmount()));

		return true;
	}

	/* Setting Prototypes for faster processing */

	bool CircularShift::SetPrototypes(Spectrum& inputs,const Spectrum& out)
	{
		return false;
	}

	bool CircularShift::SetPrototypes()
	{
		return false;
	}

	bool CircularShift::UnsetPrototypes()
	{
		return false;
	}

	/* The supervised Do() function */

	bool CircularShift::Do(void)
	{
		return Do( mInput.GetAudio(), mOutput.GetAudio() );
		mInput.Consume();
		mOutput.Produce();

	}

	/* The  unsupervised Do() function */

	bool CircularShift::Do( const DataArray& in, DataArray& out)
	{

		int i;
		TData amount = mAmount.GetLastValue();
		int size = in.Size();
		const TData* inp = in.GetPtr();
		TData* outp = out.GetPtr();
		TData* tmp;

		CLAM_ASSERT(size == out.Size(),
			"CircularShift::Do(): input and output vectors do not  match");

		if (amount > 0) {
			int ia = (int)amount;
			tmp = new TData[ia];
			for (i=0;i<ia;i++)
				tmp[i] = inp[size - ia + i];
			for (i=size-ia-1;i>= 0;i--)
				outp[i + ia] = inp[i];
			for (i=0;i< ia;i++)
				outp[i] = tmp[i];
		}
		else {
			int ia = (int)-amount;
			tmp = new TData[ia];
			for (i=0;i<ia;i++)
				tmp[i] = inp[i];
			for (i=0;i< (size - ia);i++)
				outp[i] = inp[i+ia];
			for (i=0;i< ia;i++)
				outp[i+size-ia] = tmp[i];
		}
		delete[] tmp;
		return true;
	}

	bool CircularShift::Do(Spectrum& in, Spectrum& out)
	{
		CLAM_ASSERT(!in.HasMagBuffer(),
			"CircularShift::Do(): only implemented for Spectrums with MagBuffer");

		return Do(in.GetMagBuffer(),out.GetMagBuffer());
	}

	bool CircularShift::Do( const Audio& in, Audio& out)
	{
		Do(in.GetBuffer(),out.GetBuffer());
		return true;
	}


}

