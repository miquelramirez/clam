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

//Resampling using iir filter (coef from Matlab)
//Filtered in both ways in order to avoid phase distortion

#include "OD_AudioDecimation.hxx"
#include <deque>

namespace CLAM
{

	namespace OnsetDetection
	{

		AudioDecimator::AudioDecimator()
		{
			Configure(AudioDecimatorConfig());
		}

		AudioDecimator::AudioDecimator(AudioDecimatorConfig& c)
		{
			Configure(c);
		}

		AudioDecimator::~AudioDecimator() {}


		bool AudioDecimator::ConcreteConfigure(const ProcessingConfig& c)
		{
			CopyAsConcreteConfig( mConfig, c );
			
			return true;
		}

		bool AudioDecimator::Do(void) 
		{
			return false;
		}

		void AudioDecimator::DecimateFrom44100To22050( const DataArray& signal,
							       DataArray& decimatedSignal )
		{
			CLAM_ASSERT( decimatedSignal.Size() == signal.Size()/2,
				     "OnsetDetection::AudioDecimator::DecimateFrom44100To22050(): the decimatedSignal array"
				     " is required to have half the size of the input signal" );

			CLAM_ASSERT( signal.Size() > 8,
				     "You cannot filter a 8 samples signal with an 8th order filter!!!!" );

			TSize size = signal.Size();
			TSize decimatedSize = decimatedSignal.Size();

			double a0, a1, a2,a3, a4, a5, a6, a7, a8, b0, b1, b2, b3, b4, b5, b6, b7, b8;
			//coefs from Matlab for cheby1 filter order=8, R=0.5, wn=0.4.
			a0 = 0.00034708996409;
			a1 = 0.00277671971273;
			a2 = 0.00971851899455;
			a3 = 0.01943703798909;
			a4 = 0.02429629748636;
			a5 = 0.01943703798909;
			a6 = 0.00971851899455;
			a7 = 0.00277671971273;
			a8 = 0.00034708996409;
			
			b1 = -3.86557532033009;
			b2 = 8.26246590570273;
			b3 = -11.69390652917087;
			b4 = 11.77561171603404;
			b5 = -8.54420813616590;
			b6 = 4.35590129644984;
			b7 = -1.43430940799601;
			b8 = 0.23814049785960;			
			
			int i;
			int j=0;
			double out = 0.0;

			std::deque<double> inMemory;

			std::deque<double> outMemory;


			// The "difficult" part of the filtering loop has been stripmined

			inMemory.push_front( signal[0] );
			out = a0 * inMemory[0];
			outMemory.push_front( out );
			decimatedSignal[j++]=out;



			inMemory.push_front( signal[1] );
			out = a0*inMemory[0] + a1*inMemory[1] 
				- b1*outMemory[0];
			outMemory.push_front( out );


			inMemory.push_front( signal[2] );
			out = a0*inMemory[0] + a1*inMemory[1] + a2*inMemory[2]
				- b1*outMemory[0] - b2*outMemory[1];
			outMemory.push_front( out );
			decimatedSignal[j++]=out;


			inMemory.push_front( signal[3] );
			out = a0 * inMemory[0] + a1* inMemory[1] + a2*inMemory[2] + a3*inMemory[3]
				- b1*outMemory[0] - b2*outMemory[1] - b3*outMemory[2];
			outMemory.push_front( out );
			

			inMemory.push_front( signal[4] );
			out = a0 * inMemory[0] + a1*inMemory[1] + a2*inMemory[2] + a3*inMemory[3] + a4*inMemory[4]
				- b1*outMemory[0] - b2*outMemory[1] - b3*outMemory[2] - b4*outMemory[3];
			outMemory.push_front( out );
			decimatedSignal[j++] = out;

			inMemory.push_front( signal[5] );
			out = a0 * inMemory[0] + a1*inMemory[1] + a2*inMemory[2] + a3*inMemory[3] + a4*inMemory[4] + a5*inMemory[5]
				- b1*outMemory[0] - b2*outMemory[1] - b3*outMemory[2] - b4*outMemory[3] - b5*outMemory[4];
			outMemory.push_front( out );

			inMemory.push_front( signal[6] );
			out = a0 * inMemory[0] + a1*inMemory[1] + a2*inMemory[2] + a3*inMemory[3] + a4*inMemory[4] + a5*inMemory[5] +a6*inMemory[6]
				- b1*outMemory[0] - b2*outMemory[1] - b3*outMemory[2] - b4*outMemory[3] - b5*outMemory[4] - b6*outMemory[5]; 
			outMemory.push_front( out );
			decimatedSignal[j++] = out;

			inMemory.push_front( signal[7] );
			out = a0 * inMemory[0] + a1*inMemory[1] + a2*inMemory[2] + a3*inMemory[3] + a4*inMemory[4] + a5*inMemory[5] 
				+a6*inMemory[6] + a7*inMemory[7]
				- b1*outMemory[0] - b2*outMemory[1] - b3*outMemory[2] - b4*outMemory[3] - b5*outMemory[4] 
				- b6*outMemory[5] - b7*outMemory[6]; 
			outMemory.push_front( out );

			
			//filtering
			for(i=8; i<size && j < decimatedSize; i++)
			{
				
				inMemory.push_front( signal[i] );
				out = a0 * inMemory[0] + a1*inMemory[1] + a2*inMemory[2] + a3*inMemory[3] + a4*inMemory[4] + a5*inMemory[5] 
					+a6*inMemory[6] + a7*inMemory[7] + a8*inMemory[8]
					- b1*outMemory[0] - b2*outMemory[1] - b3*outMemory[2] - b4*outMemory[3] - b5*outMemory[4] 
					- b6*outMemory[5] - b7*outMemory[6] - b8*outMemory[7]; 
				outMemory.pop_back();
				outMemory.push_front( out );


				if ( i % 2 == 0 ) decimatedSignal[j++] = out;
				inMemory.pop_back();
			}
		}

		void AudioDecimator::DecimateFrom22050To245( const Array<double>& signal,
							     Array<double>& decimatedSignal )
		{
			CLAM_ASSERT( decimatedSignal.Size() == signal.Size() / 90,
				     "AudioDecimator::DecimateFrom22050To245(): decimated signal size is not 90 times smaller than signal" );
			TSize size = signal.Size();
			int i;

			double a0, a1, a2,a3, a4, b1, b2, b3, b4;

			if ( mOutput.AllocatedSize() < signal.Size() )
			{
				mOutput.Resize( signal.Size() );
			}
			mOutput.SetSize( signal.Size() );

			if ( mOutputRev.AllocatedSize() < signal.Size() )
			{
				mOutputRev.Resize( signal.Size() );
			}
			mOutputRev.SetSize( signal.Size() );						
						
			a0 = 0.13376242754340e-007;
			a1 = 0.53504971017360e-007;
			a2 = 0.80257456526040e-007;
			a3 = 0.53504971017360e-007;
			a4 = 0.13376242754340e-007;			
			
			b1 = -3.96578818443280;
			b2 = 5.89871439914956;
			b3 = -3.90004183080475;
			b4 = 0.96711584278936;
				
			// The "difficult" part of the filtering loop has been stripmined
			mOutput[0] = a0*signal[0];

			mOutput[1] = a0*signal[1]+ a1*signal[0]
				- b1*mOutput[0];

			mOutput[2] = a0*signal[2]+ a1*signal[1] + a2*signal[0] 
				- b1*mOutput[1] - b2*mOutput[0];

			mOutput[3] = a0*signal[3]+ a1*signal[2] + a2*signal[1] + a3*signal[0] 
				- b1*mOutput[2] - b2*mOutput[1] - b3*mOutput[0];

			
			//filtering
			for(i=4; i<size; i++)
			{
				mOutput[i] = a0*signal[i] + a1*signal[i-1] + a2*signal[i-2] + a3*signal[i-3] + a4*signal[i-4] 
					     - b1*mOutput[i-1] - b2*mOutput[i-2] - b3*mOutput[i-3] - b4*mOutput[i-4];				
			}

			for(i=0;i<size;i++)
				mOutputRev[size-1-i]=mOutput[i];

			mOutput[0] = a0*mOutputRev[0];
			mOutput[1] = a0*mOutputRev[1]+ a1*mOutputRev[0]- b1*mOutput[0];
			mOutput[2] = a0*mOutputRev[2]+ a1*mOutputRev[1] + a2*mOutputRev[0] - b1*mOutput[1] - b2*mOutput[0];
			mOutput[3] = a0*mOutputRev[3]+ a1*mOutputRev[2] + a2*mOutputRev[1] + a3*mOutputRev[0] 
				- b1*mOutput[2] - b2*mOutput[1] - b3*mOutput[0];

			//filtering the reverse signal for 0 phase distortion
			for(i=4; i<size; i++)
			{

				mOutput[i] = a0*mOutputRev[i] + a1*mOutputRev[i-1] + a2*mOutputRev[i-2] + a3*mOutputRev[i-3] + a4*mOutputRev[i-4] 
					- b1*mOutput[i-1] - b2*mOutput[i-2] - b3*mOutput[i-3] - b4*mOutput[i-4];				
			
			}

			for(i=0;i<size;i++)
				mOutputRev[i]=mOutput[size-1-i];
			
			TSize decimatedSize = size/90;
			
			i=0;
			int j=0;
			//decimation
			while(i < decimatedSize && j < size )
			{
				decimatedSignal[i] = mOutputRev[j];
				i++;
				j+=90;
			}
			

		}

		void AudioDecimator::DecimateFrom22050To245( const Array<float>& signal,
							     Array<double>& decimatedSignal )
		{
			CLAM_ASSERT( decimatedSignal.Size() == signal.Size() / 90,
				     "AudioDecimator::DecimateFrom22050To245(): decimated signal size is not 90 times smaller than signal" );
			TSize size = signal.Size();
			int i;

			double a0, a1, a2,a3, a4, b1, b2, b3, b4;

			if ( mOutput.AllocatedSize() < signal.Size() )
			{
				mOutput.Resize( signal.Size() );
			}
			mOutput.SetSize( signal.Size() );

			if ( mOutputRev.AllocatedSize() < signal.Size() )
			{
				mOutputRev.Resize( signal.Size() );
			}
			mOutputRev.SetSize( signal.Size() );						
						
			a0 = 0.13376242754340e-007;
			a1 = 0.53504971017360e-007;
			a2 = 0.80257456526040e-007;
			a3 = 0.53504971017360e-007;
			a4 = 0.13376242754340e-007;			
			
			b1 = -3.96578818443280;
			b2 = 5.89871439914956;
			b3 = -3.90004183080475;
			b4 = 0.96711584278936;
				
			// The "difficult" part of the filtering loop has been stripmined
			mOutput[0] = a0*signal[0];

			mOutput[1] = a0*signal[1]+ a1*signal[0]
				- b1*mOutput[0];

			mOutput[2] = a0*signal[2]+ a1*signal[1] + a2*signal[0] 
				- b1*mOutput[1] - b2*mOutput[0];

			mOutput[3] = a0*signal[3]+ a1*signal[2] + a2*signal[1] + a3*signal[0] 
				- b1*mOutput[2] - b2*mOutput[1] - b3*mOutput[0];

			
			//filtering
			for(i=4; i<size; i++)
			{
				mOutput[i] = a0*signal[i] + a1*signal[i-1] + a2*signal[i-2] + a3*signal[i-3] + a4*signal[i-4] 
					     - b1*mOutput[i-1] - b2*mOutput[i-2] - b3*mOutput[i-3] - b4*mOutput[i-4];				
			}

			for(i=0;i<size;i++)
				mOutputRev[size-1-i]=mOutput[i];

			mOutput[0] = a0*mOutputRev[0];
			mOutput[1] = a0*mOutputRev[1]+ a1*mOutputRev[0]- b1*mOutput[0];
			mOutput[2] = a0*mOutputRev[2]+ a1*mOutputRev[1] + a2*mOutputRev[0] - b1*mOutput[1] - b2*mOutput[0];
			mOutput[3] = a0*mOutputRev[3]+ a1*mOutputRev[2] + a2*mOutputRev[1] + a3*mOutputRev[0] 
				- b1*mOutput[2] - b2*mOutput[1] - b3*mOutput[0];

			//filtering the reverse signal for 0 phase distortion
			for(i=4; i<size; i++)
			{

				mOutput[i] = a0*mOutputRev[i] + a1*mOutputRev[i-1] + a2*mOutputRev[i-2] + a3*mOutputRev[i-3] + a4*mOutputRev[i-4] 
					- b1*mOutput[i-1] - b2*mOutput[i-2] - b3*mOutput[i-3] - b4*mOutput[i-4];				
			
			}

			for(i=0;i<size;i++)
				mOutputRev[i]=mOutput[size-1-i];
			
			TSize decimatedSize = size/90;
			
			i=0;
			int j=0;
			//decimation
			while(i < decimatedSize && j < size )
			{
				decimatedSignal[i] = mOutputRev[j];
				i++;
				j+=90;
			}
			

		}

		bool AudioDecimator::Do(DataArray &input, int sampleRate, int type)
		{
			return true;

		}

	}

} // namespace CLAM

