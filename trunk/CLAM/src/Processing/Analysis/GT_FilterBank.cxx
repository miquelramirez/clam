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

//Implemented from Slaney's Auditory Toolbox (http://rvl4.ecn.purdue.edu/~malcolm/interval/1998-010/)

#include "Complex.hxx"
#include "ERB_Space_Gen.hxx"
#include "GT_FilterBank.hxx"
#include "Audio.hxx"
#include "OSDefines.hxx"
#include <libresample.h>

namespace CLAM
{

	GT_FilterBank::GT_FilterBank()
	{
		Configure(GT_FilterBankConfig());
	}

	GT_FilterBank::GT_FilterBank(GT_FilterBankConfig& c)
	{
		Configure(c);
	}

	GT_FilterBank::~GT_FilterBank() 
	{
	}


	bool GT_FilterBank::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig( mConfig, c );

		mnChannels = mConfig.GetnChannels();
		mLowFreq = mConfig.GetLowFreq();
		mHighFreq = mConfig.GetHighFreq();
		mSampleRate = mConfig.GetSampleRate();

		MakeERBFilters();
		
		return true;
	}

	bool GT_FilterBank::Do(void) 
	{
		return false;
	}
	
	bool GT_FilterBank::Do(Audio &in, Array< Array<float> >& filterBankOutputs )
	{
		CLAM_ASSERT( filterBankOutputs.Size() == mnChannels,
			     "GT_FilterBank::Do() : filterBankOutputs array size is smaller than the number of bands setup on the configuration" );


		const TSize audiosize = in.GetSize();
	
		Array<float> audioArray;

		audioArray.Resize( audiosize );
		audioArray.SetSize( audiosize );


		const DataArray & inputBuffer = in.GetBuffer();

		double factor = 245.0 / in.GetSampleRate();
		TSize  downAudioSize = audioArray.Size()/90;

		void* resamp_handle = resample_open( 1, factor, factor );

		double tempBuffer0[4];
		double tempBuffer1[4];
		double tempBuffer2[4];
		double tempBuffer3[4];
		double tempBuffer4[4];

		//filtering

		//Slaney's comment:
		//Note this implementation fixes a problem in the original code by
		//computing four separate second order filters.  This avoids a big
		//problem with round off errors in cases of very small mCentreFreqs (100Hz) and
		//large sample rates (44kHz).  The problem is caused by roundoff error
		//when a number of poles are combined, all very close to the unit
		//circle.  Small errors in the eigth order coefficient, are multiplied
		//when the eigth root is taken to give the pole location.  These small
		//errors lead to poles outside the unit circle and instability.

		//Is that also true here?
	
		Array<float>& outputBuffer = audioArray;

		for ( int i = 0; i < mnChannels; i++ )
		{
		
			tempBuffer0[0] = inputBuffer[0];
			tempBuffer0[1] = inputBuffer[1];
			tempBuffer1[0] = 0;
			tempBuffer1[1] = 0;
			tempBuffer2[0] = 0;
			tempBuffer2[1] = 0;
			tempBuffer3[0] = 0;
			tempBuffer3[1] = 0;
			tempBuffer4[0] = 0;
			tempBuffer4[1] = 0;

			outputBuffer[0] = 0;
			outputBuffer[1] = 0;

			const double A0byGain=A0/gain[i];
			const double A11byGain=A11[i]/gain[i];
			const double A2byGain=A2/gain[i];
			const double B1i=B1[i];
			const double B2i=B2[i];
			const double A12i=A12[i];
			const double A13i=A13[i];
			const double A14i=A14[i];

			unsigned j0=1,j1=0,j2;

			for(int j=2; j<audiosize; j++)
			{
				// j0, j1 and j2 implement a circular buffer of 4 positions
				// Althougt we need only 3, 4 is cheaper on calculations.
				j2 = j1;  // j-2
				j1 = j0;  // j-1
				j0 = j&3; // j

				tempBuffer0[j0] = inputBuffer[j];
				tempBuffer1[j0] =
					+ A0byGain *tempBuffer0[j0]
					+ A11byGain*tempBuffer0[j1] 
					+ A2byGain *tempBuffer0[j2]
					- B1i *tempBuffer1[j1] 
					- B2i *tempBuffer1[j2];
				tempBuffer2[j0] =
					+ A0  *tempBuffer1[j0] 
					+ A12i*tempBuffer1[j1] 
					+ A2  *tempBuffer1[j2]
					- B1i *tempBuffer2[j1] 
					- B2i *tempBuffer2[j2];
				tempBuffer3[j0] =
					+ A0  *tempBuffer2[j0]
					+ A13i*tempBuffer2[j1]
					+ A2  *tempBuffer2[j2]
					- B1i *tempBuffer3[j1]
					- B2i *tempBuffer3[j2];
				tempBuffer4[j0] =
					+ A0  *tempBuffer3[j0]
					+ A14i*tempBuffer3[j1]
					+ A2  *tempBuffer3[j2]
					- B1i *tempBuffer4[j1] 
					- B2i *tempBuffer4[j2];
				outputBuffer[j] = fabs(tempBuffer4[j0]);
			}

				
			//Decimation to 245 Hz
			int srcused;

			resample_process( resamp_handle, factor,
					  audioArray.GetPtr(), audioArray.Size(),
					  1, &srcused,
					  filterBankOutputs[i].GetPtr(), downAudioSize + 1000 );

			filterBankOutputs[i].SetSize( downAudioSize );

			for(int k=0 ; k< filterBankOutputs[i].Size() ; k++)
				filterBankOutputs[i][k] = filterBankOutputs[i][k]*mCentreFreq[i];
			

		}

		resample_close( resamp_handle );

		return true;
	}



	void GT_FilterBank::MakeERBFilters()
	{
	
		mCentreFreq.Resize(mnChannels);
		mCentreFreq.SetSize(mnChannels);
	
		ERB_SpaceGenConfig ERBCfg;
		ERBCfg.SetNumFilter(mnChannels);
		ERBCfg.SetLowFreq(mLowFreq);
		ERBCfg.SetHighFreq(mHighFreq);

		ERB_SpaceGen ERBS(ERBCfg);

		ERBS.Do(mCentreFreq);

	
		//Glasberg and Moore Parameters
		double EarQ = 9.26449;
		double minBW = 24.7;
		int order = 1;	
	
		double T=1.0/(double)mSampleRate;


		A0 = T;
		A2 = 0;
		B0 = 1;	

		Array<double> ERB, B;


		ERB.Resize(mCentreFreq.Size());
		ERB.SetSize(mCentreFreq.Size());
		B.Resize(mCentreFreq.Size());
		B.SetSize(mCentreFreq.Size());	
		B1.Resize(mCentreFreq.Size());
		B1.SetSize(mCentreFreq.Size());
		B2.Resize(mCentreFreq.Size());
		B2.SetSize(mCentreFreq.Size());
		A11.Resize(mCentreFreq.Size());
		A11.SetSize(mCentreFreq.Size());
		A12.Resize(mCentreFreq.Size());
		A12.SetSize(mCentreFreq.Size());
		A13.Resize(mCentreFreq.Size());
		A13.SetSize(mCentreFreq.Size());
		A14.Resize(mCentreFreq.Size());
		A14.SetSize(mCentreFreq.Size());
		gain.Resize(mCentreFreq.Size());
		gain.SetSize(mCentreFreq.Size());
	

		ComplexTmpl<double> a, b, tempGain;

		double invEarQ = 1.0 / EarQ;
		double invOrder = 1.0 / (double)order;

		for(int i=0; i<mnChannels; i++)
		{

			ERB[i] = pow( ( pow(mCentreFreq[i]*invEarQ,order) + pow(minBW,order) ) , invOrder);

			B[i] = ERB[i]*2.0*1.019*M_PI;

			const double invExpBiT = 1.0 / exp(B[i]*T);
			const double twocfpiT = 2.0 * mCentreFreq[i] * M_PI * T;
			const double cos_2cfpiT = std::cos( twocfpiT );
			const double sin_2cfpiT = std::sin( twocfpiT );
			const double two_1_5 = pow( 2.0, 1.5 );

			B1[i] = -2.0*cos_2cfpiT*invExpBiT;
			B2[i] = exp(-2.0*B[i]*T);

			A11[i] = -(2*T*cos_2cfpiT*invExpBiT 
				   + 2.0*sqrt(3+two_1_5)*T*sin_2cfpiT*invExpBiT)/2.0;

			A12[i] = -(2.0*T*cos_2cfpiT*invExpBiT 
				   - 2.0*sqrt(3+two_1_5)*T*sin_2cfpiT*invExpBiT)/2.0;

			A13[i] = -(2.0*T*cos_2cfpiT*invExpBiT 
				   + 2.0*sqrt(3.0-two_1_5)*T*sin_2cfpiT*invExpBiT)/2.0;

			A14[i] = -(2*T*cos_2cfpiT*invExpBiT 
				   - 2.0*sqrt(3.0-two_1_5)*T*sin_2cfpiT*invExpBiT)/2.0;

			a.SetReal(-2.0*std::cos(2.0* twocfpiT));
			a.SetImag(-2.0*std::sin(2.0* twocfpiT));

			b.SetReal(2.0*exp(-B[i]*T)*cos_2cfpiT);
			b.SetImag(2.0*exp(-B[i]*T)*sin_2cfpiT);

			tempGain =
				( a*T + b*T*(cos_2cfpiT - sqrt(3.0-two_1_5)*sin_2cfpiT))*
				( a*T + b*T*(cos_2cfpiT + sqrt(3.0-two_1_5)*sin_2cfpiT))*
				( a*T + b*T*(cos_2cfpiT - sqrt(3.0+two_1_5)*sin_2cfpiT))*
				( a*T + b*T*(cos_2cfpiT + sqrt(3.0+two_1_5)*sin_2cfpiT))/
				(( a-2.0/exp(2.0*B[i]*T)  - (a-2.0)*invExpBiT)*( a-2.0/exp(2.0*B[i]*T)  - (a-2.0)*invExpBiT)*
				 ( a-2.0/exp(2.0*B[i]*T)  - (a-2.0)*invExpBiT)*( a-2.0/exp(2.0*B[i]*T)  - (a-2.0)*invExpBiT));

			gain[i] = tempGain.Mag();

		}


	}
	
} // namespace CLAM

