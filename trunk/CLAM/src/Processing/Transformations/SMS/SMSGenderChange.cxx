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

#include "SMSGenderChange.hxx"
#include "Factory.hxx"

using namespace CLAM;


bool SMSGenderChange::Do(const Frame& in, Frame& out)
{
	if(in.GetFundamental().GetFreq(0))//we only transform voiced frames
	{
		Frame tmpFrame;
		tmpFrame=in;

		TData minPitch=100;
		TData maxPitch=500;

		//Maximum spectral shift
		TData maxSss=200;

		//amount for spectral shape shift
		TData sss;

		TData pitch=in.GetFundamentalFreq();

		if(pitch<minPitch) sss=0;
		else if (pitch>maxPitch) sss=maxSss;
		else sss=(pitch-minPitch)/((maxPitch-minPitch)/maxSss);

		
		//if amount is zero it means from male to female, else from female to male
		TData amount=mAmountCtrl.GetLastValue();

		TData pitchTransposition;

		if(amount) //female to male
		{
			sss=-sss;
			pitchTransposition=0.5;
		}
		else //male to female
		{
			pitchTransposition=2;
		}

		
		
		mPO_SpectralShapeShift.mAmountCtrl.DoControl(sss);
		mPO_PitchShift.mAmountCtrl.DoControl(pitchTransposition);

		mPO_SpectralShapeShift.Do(in.GetSpectralPeakArray(),tmpFrame.GetSpectralPeakArray());
		mPO_PitchShift.Do(tmpFrame,out);
	}
	return true;
}

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
static ProcessingFactory::Registrator<CLAM::SMSGenderChange> regtSMSGenderChange( "SMSGenderChange" );
