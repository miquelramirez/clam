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

#include "SMSPitchDiscretization.hxx"
#include "Factory.hxx"

using namespace CLAM;


bool SMSPitchDiscretization::Do(const Frame& in, Frame& out)
{
	TData pitch=in.GetFundamentalFreq();
	if (pitch>0)
	{
		TData log2=0.69314718f;
		TData test=12*log(pitch/55)/log2;
		TData test1=pitch/55;
		TData test2=log(test1);
		TData test3=test2*12;
		TData test4=test3/log2;


		int nst=Round(12*log(pitch/55)/log2);
		TData pow2_1_12=1.0594630f;
		TData discPitch=55*(pow(pow2_1_12,nst));
		
		TData amount=discPitch/pitch;

		mPO_PitchShift.mAmountCtrl.DoControl(amount);
		mPO_PitchShift.Do(in,out);
		
		Fundamental tmpFund;
		tmpFund.AddElem(discPitch);
		out.SetFundamental(tmpFund);
	}
	return true;
}

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
static ProcessingFactory::Registrator<CLAM::SMSPitchDiscretization> regtSMSPitchDiscretization( "SMSPitchDiscretization" );
