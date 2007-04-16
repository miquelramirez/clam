/*
 * Author: fabien gouyon 
 * http://www.iua.upf.es/~fgouyon
 * Description:
 *
 * Syntax: C++
 *
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
#include "RD_MeterEstimator.hxx"
#include "Audio.hxx"
#include "Meter.hxx"
#include <iostream>

namespace CLAM
{

	namespace RhythmDescription
	{

		void MeterEstimatorConfig::DefaultInit()
		{
			AddAll();
			UpdateData();
			//default values
			SetTempoLimInf(50);
			SetTempoLimSup(200);
			SetACFUpperLimit(10);
			SetAutomaticIntegTime(true);
			SetACFIntegrationTime(10);
		}

		std::ofstream MeterEstimator::smLogFile;
		bool          MeterEstimator::smLogInit = false;

		MeterEstimator::MeterEstimator()
		{
			CheckLogInitialization();
			AttachChildren();

		}

		MeterEstimator::~MeterEstimator()
		{
		}
	       
		void MeterEstimator::CheckLogInitialization()
		{
			if ( !smLogInit )
			{
				if ( smLogFile.is_open() )
				{
					smLogFile.close();
				}

				std::string logFilename = std::string(GetClassName()) + std::string("_Log.log");
				smLogFile.open( logFilename.c_str() );
				smLogInit = true;
			}
		}

		std::ostream& MeterEstimator::Log()
		{
			CLAM_ASSERT( smLogFile.is_open(), "MeterEstimator::Log() : the log file is not opened!" );
			return smLogFile;
		}

		const ProcessingConfig& MeterEstimator::GetConfig() const
		{
			return mConfig;
		}

		const char* MeterEstimator::GetClassName() const
		{
			return "MeterEstimator";
		}

		// Configure the Processing Object according to the Config object
		bool MeterEstimator::ConcreteConfigure(const ProcessingConfig& c)
		{
			CopyAsConcreteConfig( mConfig, c );

			ConfigureData();
			ConfigureChildren();

			return true;
		}

		bool MeterEstimator::ConfigureChildren()
		{
			AutoCorrelationTDConfig myACFcfg;
			myACFcfg.AddAll();
			myACFcfg.UpdateData();
			myACFcfg.SetUpperLimit(mConfig.GetACFUpperLimit());
			myACFcfg.SetIntegrationTime(mConfig.GetACFIntegrationTime());
			myACFcfg.SetAutomaticIntegTime(mConfig.GetAutomaticIntegTime());
			mACF.Configure(myACFcfg);

			return true;
		}

		bool MeterEstimator::Do(void)
		{
			CLAM_ASSERT(false, "MeterEstimator::Do(): Supervised mode not implemented");
			return false;
		}


		bool MeterEstimator::Do(Audio& audioIn, const Pulse& beatData, 
					Meter& dataOut)
		{
			const Array<TimeIndex>& beats = beatData.GetIndexes();
			
			// MRJ: If there were no beats detected: i.e. silence or
			// error, we just set the numerator and denominator
			// for the meter to 0/0.
			if ( beats.Size() == 0 )
			{
				dataOut.SetNumerator(0);
				dataOut.SetDenominator(0);
				
				return true;
			}
			
			TData sampleRate = audioIn.GetSampleRate();

			// MRJ: We take as the tempo value the BPM descriptor
			// as computed by the TickSequenceTracker composite
			
			TData globalTempo = (60.0 * sampleRate)/beatData.GetRate();
			TData offset = globalTempo / 2.0;
			

			//-------Remove audio DC component------------------
			//TODO
			// MRJ: some of the segment descriptors might be sensitive
			// to the presence/absence of a DC component

			
			// MRJ: Beat intervals descriptors computation.
			// Here we segment the input signal into a series of
			// fragments, centered on each beat ( minus the first one )
			// with a width of globalTempo.
			Array<TData> segments; 
			
			for (int i=1;i<beats.Size();i++)   //NB: begins at 1
				segments.AddElem(beats[i].GetPosition()*sampleRate
						 - offset);


			mSegment.SetHoldsData(true);

			
			List<Segment> segList;
			List<SegmentDescriptors> segDList;
			
			for(int i=0;i<segments.Size()-1;i++) 
			{
				segList.AddElem(mSegment);
				segDList.AddElem(mSegmentD);
				
				Segment & seg = segList[i];
				SegmentDescriptors & segD = segDList[i];
				
				segD.SetpSegment(&seg);
				
				Audio & audioTmp = seg.GetAudio();
				
				audioIn.GetAudioChunk((int)(segments[i]),
						      (int)(segments[i+1]),audioTmp);
				AudioDescriptors & audioD = segD.GetAudioD();
				mAudioDescGen.Do(audioD);
			}
			
			// MRJ: AutoCorrelation factors computation
			// over the sequence of "beat intervals" descriptors
			
			Array<TData> seq,acf;
			
			for (int i=0;i<segments.Size()-1;i++) 
			{
				//MRJ: Temporal Centroid computation result is normalized
				// i.e. mapping to the [0..1] interval.

				TData centroidTime = segDList[i].GetAudioD().GetTemporalCentroid();
				centroidTime*=sampleRate;

				Log() << "Centroid: " << centroidTime << " Segment("<< i+1 <<"): ";
				Log() << segments[i+1] << " Segment("<<i<<"): " << segments[i];
				Log() << " Width: " << segments[i+1] - segments[i];
				Log() << " Factor: " << centroidTime / ( segments[i+1] - segments[i] ) << std::endl; 

				seq.AddElem( centroidTime / ( segments[i+1] - segments[i] ) );
			}

			mACF.Do(seq,acf);
			
			// MRJ: Decision. This is done by finding on which
			// side of the hyperplane defined by M lies the
			// analyzed beat sequence.
			// Note: Take that this is a quite wild departure
			// from the "canon" established in pherrera and fgouyon
			// article. Basically, I found that:
			// 1) there was a grievous error with the indexes due to
			//    a careless translation from Matlab to C
			// 2) it did not make much sense to just consider 5 low-order
			//    autocorrelation values, whose difference ranged
			//    between [1e-3..1e-5].
			
			TSize acfSize = acf.Size();
			if (acfSize<10) 
			{
				Log()<<"upper limit is too small, unreliable computations..."<< std::endl;
				acf.Resize(10);
				acf.SetSize(10);
				//newly created elements are set to 0 automatically
			}

			Log()<<"ACF"<<std::endl;
			for(int i=0;i<acf.Size();i++) 
				Log() << "acf("<<i<<"):="<<acf[i]<<"\n";

			// MRJ: Almost the one on the paper ( coefficient #9 is used instead of #6 ).
			// Fabien changed this so we don't mix ternary and binary hypotheses: 3 and
			// 9 do not have as factor two, while six does. 

			// MRJ: the M feature as was found on the original "Cuidado" implementation
			//TData M = (acf[2]+acf[4]+acf[8])/3 - (acf[3]+acf[9])/2;

			// MRJ: the M feature translated right from MATLABish
			TData M = (acf[1]+acf[3]+acf[7])/3.0 - (acf[2]+acf[8])/2.0;
			Log()<<"Feature M = "<<M<<std::endl;
			

			dataOut.SetDenominator(4);

			// MRJ: Another departure from the canon. We compute the
			// "likelihoods" for our hypothesis i.e. duple/triple meter.
			// Note that we only consider those orderings that are
			// multiples of 2 or 3 ( 2n beat groups, 3n beat groups ).

			TData dupleLikelihood = 0.0;
			int twoMults = 0, threeMults = 0;
			TData tripleLikelihood = 0.0;

			for ( int i = 1; i < acf.Size(); i++ )
			{
				if ( fabs(acf[i]) > 1e-7 )
				{

					if ( (i+1)%2 == 0)
					{
						dupleLikelihood+=acf[i];
						twoMults++;
					}
					
					if ( (i+1)%3 == 0)
					{
						tripleLikelihood+=acf[i];
						threeMults++;
					}
				}
			}

			tripleLikelihood *= 1.0/TData(threeMults);
			dupleLikelihood *= 1.0/TData(twoMults);
			
			Log() << "twoMults= " << twoMults << std::endl;
			Log() << "threeMults= " << threeMults << std::endl;
			Log() << "dupleLikelihood = " << dupleLikelihood << std::endl;
			Log() << "tripleLikelihood = " << tripleLikelihood << std::endl;
			Log() << "M2 = " << dupleLikelihood - tripleLikelihood << std::endl;

			TData M2 = dupleLikelihood - tripleLikelihood;
			if ( M2 < -0.000665 ) 
			{
				dataOut.SetNumerator(3);
				Log()<<"Triple (3/4) meter"<<std::endl;
			}
			else 
			{
				dataOut.SetNumerator(4);
				Log()<<"Duple (4/4) meter"<<std::endl;
			}
					       

			return true;
		}


		
		
		
		void MeterEstimator::AttachChildren()
		{
			mAudioDescGen.SetParent(this);
			mACF.SetParent(this);
		}
		
		void MeterEstimator::ConfigureData()
		{
			mSegment.AddAudio();
			mSegment.AddBeginTime();
			mSegment.AddEndTime();
			//mSegment.AddChildren();
			mSegment.UpdateData();
			
			mSegmentD.RemoveAll();
			mSegmentD.AddAudioD();
			//mSegmentD.AddChildrenD();
			mSegmentD.UpdateData();
			
			AudioDescriptors & audioD = mSegmentD.GetAudioD();
			audioD.RemoveAll();
			audioD.AddTemporalCentroid();
			audioD.UpdateData();
		}
		
		
		
	} // namespace RhythmDescription
	
} // namespace CLAM

