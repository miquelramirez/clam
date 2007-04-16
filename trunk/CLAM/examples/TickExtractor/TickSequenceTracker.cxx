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

#include "TickSequenceTracker.hxx"
#include "RD_GlobalPulse.hxx"
#include "RD_GridGen.hxx"
#include "Audio.hxx"
#include "CLAM_Math.hxx"
#include <list>
#include <algorithm>

namespace CLAM
{

	namespace RhythmDescription
	{

		TickSequenceTracker::TickSequenceTracker()
		{

			mPeakDetector.SetParent( this );
			mTemporalDiff.SetParent( this );
			mTimeSeriesFinder.SetParent( this );
			mTickSwingAdjuster.SetParent( this );
			mTickOnsetsAdjuster.SetParent( this );			
			mBeatTickAdjuster.SetParent( this );
			mBeatOnsetsAdjuster.SetParent( this );
			mTempoEstimator.SetParent( this );
			mGlobalPREstimator.SetParent( this );
		}
		
		TickSequenceTracker::~TickSequenceTracker()
		{
		}

		// Configure the Processing Object according to the Config object
		bool TickSequenceTracker::ConcreteConfigure(const ProcessingConfig& c)
		{
			CopyAsConcreteConfig( mConfig, c );

			//For Hist peaks:
			IOIHistPeakDetectorConfig apdconf;
			apdconf.SetThreshold( mConfig.GetThreshold_IOIHistPeaks() );
			apdconf.SetSampleRate( mConfig.GetSampleRate() );

			mPeakDetector.Configure( apdconf );

			TimeDifferenceConfig tconf;
			tconf.SetGaussianSize((TSize)(mConfig.GetSampleRate()*mConfig.GetGaussianWindowSize()));

			mTemporalDiff.Configure( tconf );
		
			TimeSeriesFinderConfig tsfConfig;

			//Here the offset is set to 0 as the computation is done
			// over the histogram peaks
			//Thus, OffsetStep=tickLimInf ==> no offset seeking
										
			tsfConfig.SetOffsetMin( 0 );
			tsfConfig.SetOffsetStep(  unsigned(mConfig.GetTickLimInf()*mConfig.GetSampleRate()) );
			tsfConfig.SetIntervalMin( unsigned(mConfig.GetTickLimSup()*mConfig.GetSampleRate()) );
			tsfConfig.SetIntervalMax( unsigned(mConfig.GetTickLimInf()*mConfig.GetSampleRate()) );
			tsfConfig.SetIntervalStep( 10 );
			tsfConfig.SetDeviationPenalty(mConfig.GetDeviationPenalty());
			tsfConfig.SetOverSubdivisionPenalty(mConfig.GetOverSubdivisionPenalty());
				
			mTimeSeriesFinder.Configure( tsfConfig );


			AdjustTickWRTSwingConfig swingAdjusterCfg;

			swingAdjusterCfg.SetSampleRate( mConfig.GetSampleRate() );
			swingAdjusterCfg.SetTempoLimSup( mConfig.GetTempoLimSup() );
			swingAdjusterCfg.SetTempoLimInf( mConfig.GetTempoLimInf() );

			mTickSwingAdjuster.Configure( swingAdjusterCfg );


			AdjustTickWRTOnsetsConfig onsetsAdjusterCfg;

			onsetsAdjusterCfg.SetSampleRate( mConfig.GetSampleRate() );
			onsetsAdjusterCfg.SetDeviationPenalty( mConfig.GetDeviationPenalty() );
			onsetsAdjusterCfg.SetOverSubdivisionPenalty( mConfig.GetOverSubdivisionPenalty() );
			onsetsAdjusterCfg.SetScope( mConfig.GetScope() );
			onsetsAdjusterCfg.SetTickLimSup( mConfig.GetTickLimSup()*mConfig.GetSampleRate() );

			mTickOnsetsAdjuster.Configure( onsetsAdjusterCfg );

			AdjustBeatWRTTickConfig beatTickAdjusterCfg;
			beatTickAdjusterCfg.SetSampleRate( mConfig.GetSampleRate() );
			beatTickAdjusterCfg.SetTempoLimSup( mConfig.GetTempoLimSup() );
			beatTickAdjusterCfg.SetTempoLimInf( mConfig.GetTempoLimInf() );

			mBeatTickAdjuster.Configure( beatTickAdjusterCfg );


			AdjustBeatWRTOnsetsConfig beatOnsetsAdjusterCfg;

			beatOnsetsAdjusterCfg.SetSampleRate( mConfig.GetSampleRate() );
			beatOnsetsAdjusterCfg.SetDeviationPenalty( mConfig.GetDeviationPenalty() );
			beatOnsetsAdjusterCfg.SetOverSubdivisionPenalty( mConfig.GetOverSubdivisionPenalty() );

			mBeatOnsetsAdjuster.Configure( beatOnsetsAdjusterCfg );

			BeatIntervalEstimatorConfig tempoEstCfg;

			tempoEstCfg.SetSampleRate( mConfig.GetSampleRate() );
			tempoEstCfg.SetTempoLimSup( mConfig.GetTempoLimSup() );
			tempoEstCfg.SetTempoLimInf( mConfig.GetTempoLimInf() );

			mTempoEstimator.Configure( tempoEstCfg );

			GlobalPulseRateEstimatorConfig gpreCfg;

			gpreCfg.SetSampleRate( mConfig.GetSampleRate() );
			gpreCfg.SetRateLowerBound( mConfig.GetTickLimInf() );
			gpreCfg.SetGaussianSize( mConfig.GetSampleRate()*mConfig.GetGaussianWindowSize() );

			mGlobalPREstimator.Configure( gpreCfg );

			// internal parameters setup
			//MRJ: put a maximum on the IOIHist length (maximum difference to be considered
			//is 10s)
			mIOIHistMaxSize = 10 * (TSize)mConfig.GetSampleRate();

			// Internal data objects setup
			mTransientsForHist.Resize( mConfig.GetNTrans() );
			mTransientsForHist.SetSize( mConfig.GetNTrans() );
			//Don't use weights for the building of the histogram:
			mTransientsForHist[0].SetWeight(0); //because the 1st transient is added manually			
			mTransientsForHist[0].SetPosition(0);

			return true;
		}

		bool  TickSequenceTracker::Do(void)
		{
			CLAM_ASSERT(false, "TickSequenceTracker::Do(): Supervised mode not implemented");
			return false;
		}


		bool TickSequenceTracker::Do(const Array<TimeIndex>& transients,
					     Pulse& tickSequence,
					     Pulse& beatSequence,
					     IOIHistogram& IOIHist)
		{

			mTickFirstGuess.SetOffset(0); 
			mTickFirstGuess.SetInterval(1);
		
			mGoodTick.SetOffset(0); 
			mGoodTick.SetInterval(1);
		
			mGoodTempo.SetOffset(0); 
			mGoodTempo.SetInterval(1);


			if ( transients.Size() < 5 ) 
			{
				return false;
			}

			int stop = 0;
			int numbTrans = mConfig.GetNTrans();

			//If one gives a large value for numbTrans, the computation is done
			// in a single loop
			if (numbTrans>transients.Size())
			{
				numbTrans = transients.Size();
				mTransientsForHist.SetSize( numbTrans );
				stop = 1;
			}

			int transHop = mConfig.GetTransHop();
			int firstTransientIndex = 0;
			int lastTransientIndex = numbTrans-1;

			Array<TimeIndex> IOIHistPeaks;
			Array<TimeIndex> tickArray;
			Array<TimeIndex> tempoArray;

			Array<TData> forGlobalTempoCalc;
			Array<TData> forGlobalTickCalc;
		
			int nLoops = 1;

			while (lastTransientIndex<transients.Size() && stop<2)
			{
				int firstTransientPos = (int)transients[firstTransientIndex].GetPosition();
				int lastTransientPos = (int)transients[lastTransientIndex].GetPosition();
				TSize windowSize = lastTransientPos - firstTransientPos;
			
				TSize actualIOIHistSize = std::min((TSize)windowSize,mIOIHistMaxSize);
				
				if ( IOIHist.GetBins().Size() < actualIOIHistSize )
				{
					IOIHist.GetBins().Resize( actualIOIHistSize );
				}
				
				IOIHist.GetBins().SetSize( actualIOIHistSize );

				/// Compute the IOIHistogram
				for (int i=1;i<mTransientsForHist.Size();i++)
				{
					mTransientsForHist[i].SetPosition
						(transients[firstTransientIndex+i].GetPosition()-firstTransientPos);
					mTransientsForHist[i].SetWeight(1); //All weights to 1
				}

				mTemporalDiff.Do( mTransientsForHist, IOIHist );

				///IOI histogram Peak Detection

				mPeakDetector.Do( IOIHist, IOIHistPeaks );

				///Tick Estimation

				//Use of both errors:
				// default value: 2				
				//Use of histogram peak weights
				mTimeSeriesFinder.Do( IOIHistPeaks, mTickFirstGuess );
				
				if ( mConfig.GetTickAdjustForSwing() )
					mTickSwingAdjuster.Do( IOIHist, mTickFirstGuess, mTickFirstGuess );
		
				///Adjust pulses and generate arrays of pulses
				///Tick adjustment
				mTickOnsetsAdjuster.GetInControl("FirstTransientPosition").DoControl( firstTransientPos );
				mTickOnsetsAdjuster.GetInControl("LastTransientPosition").DoControl( lastTransientPos );
				
				//Use of transientsForHist or transients???
				// i.e. use of weights or not??					
				mTickOnsetsAdjuster.Do( transients, mTickFirstGuess, 
							tickArray, mGoodTick);		

				forGlobalTickCalc.AddElem(mGoodTick.GetInterval());

				StorePulseIndexes(nLoops, tickArray, tickSequence.GetIndexes());

				///Compute Tempo (optional)
				if (mConfig.GetComputeBeats()) 
				{

					TimeSeriesSeed initialBeatParams;
					
					mTempoEstimator.Do( IOIHistPeaks, initialBeatParams );

					TimeSeriesSeed tickAdjustedBeatParams;
					mBeatTickAdjuster.Do( mGoodTick, initialBeatParams, tickAdjustedBeatParams );
					
					//get the best phase
					// Computing best beat phase

					mBeatOnsetsAdjuster.GetInControl("FirstTransientPosition").DoControl( firstTransientPos );
					mBeatOnsetsAdjuster.GetInControl("LastTransientPosition").DoControl( lastTransientPos );

					//NB: Use of transients instead of transientsForHist
					// i.e. making use of transient weights
					mBeatOnsetsAdjuster.Do( transients, mGoodTick, tickAdjustedBeatParams,
								tempoArray, mGoodTempo );
					
					forGlobalTempoCalc.AddElem( mGoodTempo.GetInterval() );

					StorePulseIndexes(nLoops, tempoArray, beatSequence.GetIndexes() );
				}

				nLoops +=1;
				firstTransientIndex = nLoops*transHop;
				lastTransientIndex = nLoops*transHop+numbTrans-1;

				if ( lastTransientIndex >= transients.Size() )
				{
					lastTransientIndex = transients.Size()-1;
					firstTransientIndex = lastTransientIndex-numbTrans+1;
					stop += 1;
				}

		
			} //end of while loop


			///Compute Global tempo
			if (mConfig.GetComputeBeats() )
			{
				const TData tempoLimInf = mConfig.GetTempoLimInf(); //BPM
				TData rateLowerBound = (mConfig.GetSampleRate()*60.0)/tempoLimInf;
				mGlobalPREstimator.GetInControl( "RateLowerBound" ).DoControl( rateLowerBound );
				mGlobalPREstimator.Do( forGlobalTempoCalc, beatSequence );
			}

			///Compute Global tick
			const int tickLimInf = int(mConfig.GetTickLimInf()*mConfig.GetSampleRate()); //samples
			mGlobalPREstimator.GetInControl( "RateLowerBound" ).DoControl( tickLimInf );
			mGlobalPREstimator.Do( forGlobalTickCalc, tickSequence );
		
			return true;
		}

		void TickSequenceTracker::StorePulseIndexes(const int nLoops,
							    const Array<TimeIndex>& pulsesArray, Array<TimeIndex>& mPulses)
		{
			int i = 0;
			if (nLoops == 1)
			{

				mPulses.Resize( pulsesArray.Size() );
				mPulses.SetSize( pulsesArray.Size() );

				std::copy( pulsesArray.GetPtr(), pulsesArray.GetPtr() + pulsesArray.Size(),
					   mPulses.GetPtr() );

			}
			else
			{
				// MRJ: Concatenates previously found pulses with the new ones

				TTime lastPosition = mPulses[mPulses.Size()-1].GetPosition();


				while ((pulsesArray[i].GetPosition() <
					lastPosition) &&
				       (i<pulsesArray.Size()))
				{
					i+=1;
				}

				while (i<pulsesArray.Size())
				{
					mPulses.AddElem(pulsesArray[i]);
					i+=1;
				}
			}
		}



	} // namespace RhythmDescription

} // namespace CLAM

