/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "rhythmir_process.hxx"
#include "AudioFile.hxx"
#include "MonoAudioFileReader.hxx"
#include "Normalization.hxx"
#include "Segment.hxx"
#include "OnsetDetector.hxx"
#include "Pulse.hxx"
#include "TickSequenceTracker.hxx"
#include "RD_MeterEstimator.hxx"
#include "Meter.hxx"

namespace RhythmIR
{

	void LoadInputAudio( CLAM::DescriptionDataPool& pool,
			     std::string filename )
	{
		CLAM::AudioFile file;
		file.OpenExisting( filename );

		if ( !file.IsReadable() )
		{
			std::string errStr = "Error: file " + file.GetLocation() + " cannot be opened ";
			errStr += "or is encoded in an unrecognized format\n";
			
			throw CLAM::Err( errStr.c_str() );
		}


		std::cerr << "File Location: " << file.GetLocation() << std::endl;
		std::cerr << "File length(ms): " << file.GetHeader().GetLength() << std::endl;
		std::cerr << "File sample rate: " << file.GetHeader().GetSampleRate() << std::endl;
		// Pool setup
		// MRJ: Length is in milliseconds :S
		CLAM::TSize fileSize = int((file.GetHeader().GetLength()/1000.)*file.GetHeader().GetSampleRate());

		pool.SetNumberOfContexts( "Sample",  fileSize );	

		*pool.GetWritePool<CLAM::TData>("Global", "SampleRate" ) = file.GetHeader().GetSampleRate();

		CLAM::Audio     tempAudio;
		tempAudio.GetBuffer().SetPtr( pool.GetWritePool<CLAM::TData>("Sample","Value"), fileSize  );
		tempAudio.SetSampleRate( file.GetHeader().GetSampleRate() );

		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile( file );
		CLAM::MonoAudioFileReader reader;
		reader.Configure( cfg );
				
		//Read Audio File
		reader.Start();		
		reader.Do( tempAudio );
		reader.Stop();

	}

	void NormalizeInputAudio( CLAM::DescriptionDataPool& pool, 
				  const CLAM::TickExtractorConfig& cfg )
	{
		CLAM::Normalization       audioNormalizer;
		CLAM::NormalizationConfig audioNormalizerConfig;

		// Scaling factor is computed from the "dominant" energy level
		audioNormalizerConfig.SetType( 3 ); 

		// Building the dummy Audio objects from the pool
		CLAM::Audio dummyAudioOrig;
		CLAM::Audio dummyAudioNorm;

		dummyAudioOrig.GetBuffer().SetPtr( pool.GetWritePool<CLAM::TData>("Sample","Value"),
						   pool.GetNumberOfContexts( "Sample") );

		dummyAudioNorm.GetBuffer().SetPtr( pool.GetWritePool<CLAM::TData>("Sample","NormalizedValue"),
						   pool.GetNumberOfContexts( "Sample") );

		dummyAudioOrig.SetSampleRate( *pool.GetWritePool<CLAM::TData>("Global","SampleRate") );
		dummyAudioNorm.SetSampleRate( *pool.GetWritePool<CLAM::TData>("Global","SampleRate") );

		// Configuration and execution
		audioNormalizer.Configure( audioNormalizerConfig );
		audioNormalizer.Start();
		audioNormalizer.Do( dummyAudioOrig, dummyAudioNorm );
		audioNormalizer.Stop();
	}


	void ExtractOnsets( CLAM::DescriptionDataPool& pool, const CLAM::TickExtractorConfig& config )
	{
		CLAM::Segment seg;
		seg.AddAudio();
		seg.UpdateData();
		seg.SetHoldsData(true);

		// Building dummy objects from pool
		CLAM::Audio dummyAudio;
		dummyAudio.GetBuffer().SetPtr( pool.GetWritePool<CLAM::TData>("Sample","NormalizedValue"),
					       pool.GetNumberOfContexts( "Sample") );

		dummyAudio.SetSampleRate( *pool.GetWritePool<CLAM::TData>("Global","SampleRate") );

		// The array to leave the transients detected
		CLAM::Array<CLAM::TimeIndex> transients;

		seg.SetAudio( dummyAudio );	

		CLAM::TTime duration = seg.GetAudio().GetSize()/seg.GetAudio().GetSampleRate();			
		CLAM::TData sampleRate = seg.GetAudio().GetSampleRate();

		seg.SetEndTime(duration);		
		
		CLAM::OnsetDetectorConfig onsetconfig;
		CLAM::OnsetDetector onset;
		
		onsetconfig.SetComputeOffsets(false);
		onsetconfig.SetGlobalThreshold(25);
			
		onset.Configure(onsetconfig);
		
		onset.Start();
		onset.Do(seg, transients);
		onset.Stop();
		
		if ( transients.Size() > 0 )
		{
			pool.SetNumberOfContexts( "Onset", transients.Size()+1 );

			CLAM::TTime* onsetPositions = pool.GetWritePool<CLAM::TTime>("Onset","Position");
			CLAM::TData* onsetWeights = pool.GetWritePool<CLAM::TData>("Onset","Weight");

			onsetPositions[0] = 0.0;
			onsetWeights[0] = 0.0;
			
			for ( int k = 1; k < transients.Size()+1; k++ )
			{
				onsetPositions[k] = transients[k-1].GetPosition()*sampleRate;
				onsetWeights[k] = transients[k-1].GetWeight();
			}
		}
	}

	void ExtractTicksAndBeats( CLAM::DescriptionDataPool& pool,
				   const CLAM::TickExtractorConfig& config )
	{
		CLAM::TData sampleRate = *pool.GetWritePool<CLAM::TData>("Global","SampleRate");
		
		// building the transients from the pool

		CLAM::Array<CLAM::TimeIndex> transients;

		transients.Resize( pool.GetNumberOfContexts( "Onset" ) );
		transients.SetSize( pool.GetNumberOfContexts( "Onset" ) );

		CLAM::TTime* transientPosVec = pool.GetWritePool<CLAM::TTime>( "Onset", "Position");
		CLAM::TData* transientWeiVec = pool.GetWritePool<CLAM::TData>( "Onset", "Weight");

		for ( int k = 0; k < transients.Size(); k++ )
		{
			transients[k].SetPosition( transientPosVec[k] );
			transients[k].SetWeight( transientWeiVec[k] );
		}

		CLAM::Pulse beatSequence;
		CLAM::Pulse tickSequence;

		// Ticks ( and beats ) computation 

		CLAM::RhythmDescription::TickSequenceTracker myTickSequenceTracker;
		
		CLAM::RhythmDescription::TickSequenceTrackerConfig myTickSequenceTrackerConfig;

		myTickSequenceTrackerConfig.SetComputeBeats( config.GetComputeBeats() );
		myTickSequenceTrackerConfig.SetThreshold_IOIHistPeaks( config.GetThreshold_IOIHistPeaks() );
		myTickSequenceTrackerConfig.SetTempoLimInf( config.GetTempoLimInf() );
		myTickSequenceTrackerConfig.SetTempoLimSup( config.GetTempoLimSup() );
		myTickSequenceTrackerConfig.SetTickLimInf( config.GetTickLimInf() );
		myTickSequenceTrackerConfig.SetTickLimSup( config.GetTickLimSup() );
		myTickSequenceTrackerConfig.SetDeviationPenalty( config.GetDeviationPenalty() );
		myTickSequenceTrackerConfig.SetOverSubdivisionPenalty( config.GetOverSubdivisionPenalty() );
		myTickSequenceTrackerConfig.SetGaussianWindowSize( config.GetGaussianWindowSize() );
		myTickSequenceTrackerConfig.SetScope( config.GetScope() );
		myTickSequenceTrackerConfig.SetAdjustWithOnsets( config.GetAdjustWithOnsets() );
		myTickSequenceTrackerConfig.SetNTrans( config.GetNTrans() );
		myTickSequenceTrackerConfig.SetTransHop( config.GetTransHop() );
		
		myTickSequenceTrackerConfig.SetSampleRate(sampleRate);
		
		myTickSequenceTracker.Configure(myTickSequenceTrackerConfig);		

		CLAM::RhythmDescription::IOIHistogram ioiHistogram;


		myTickSequenceTracker.Start();
		
		//Use the transients computed in this main
		myTickSequenceTracker.Do( transients, tickSequence, beatSequence, ioiHistogram );

		myTickSequenceTracker.Stop();	

		// Storing the obtained beat and tick sequences into the pool
		
		{
			*pool.GetWritePool<unsigned>( "Global","BeatsPerMinute" ) = unsigned( beatSequence.GetRate() );
			*pool.GetWritePool<unsigned>( "Global","TicksPerMinute" ) = unsigned( tickSequence.GetRate() );

			pool.SetNumberOfContexts( "Tick", tickSequence.GetIndexes().Size() );

			CLAM::TTime* tickPositions = pool.GetWritePool<CLAM::TTime>("Tick","Position");

			for ( int k = 0; k < tickSequence.GetIndexes().Size(); k++ )
			{
				tickPositions[k] = tickSequence.GetIndexes()[k].GetPosition();
			}

			pool.SetNumberOfContexts( "Beat", beatSequence.GetIndexes().Size() );

			CLAM::TTime* beatPositions = pool.GetWritePool<CLAM::TTime>("Beat","Position");

			for ( int k = 0; k < beatSequence.GetIndexes().Size(); k++ )
			{
				beatPositions[k] = beatSequence.GetIndexes()[k].GetPosition();
			}
			
		}
	}

	void ExtractMeter( CLAM::DescriptionDataPool& pool,
			   const CLAM::TickExtractorConfig& config )
	{
		// Building the dummy audio

		CLAM::Audio signal;
		signal.GetBuffer().SetPtr( pool.GetWritePool<CLAM::TData>("Sample","Value"),
					   pool.GetNumberOfContexts( "Sample") );

		signal.SetSampleRate( *pool.GetWritePool<CLAM::TData>("Global","SampleRate") );

		// Building the beat sequence for the Meter estimation processing
		
		CLAM::Pulse extractedBeats;
		extractedBeats.SetRate( *pool.GetWritePool<unsigned>( "Global","BeatsPerMinute" ) );
		extractedBeats.GetIndexes().Resize( pool.GetNumberOfContexts( "Beat" ) );
		extractedBeats.GetIndexes().SetSize( pool.GetNumberOfContexts( "Beat" ) );

		CLAM::TTime* beatPositions = pool.GetWritePool<CLAM::TTime>("Beat","Position");
		
		for ( int k = 0; k < extractedBeats.GetIndexes().Size(); k++ )
		{
			extractedBeats.GetIndexes()[k].SetPosition(beatPositions[k] );
			extractedBeats.GetIndexes()[k].SetWeight( 1.0 );
		}
		

		CLAM::RhythmDescription::MeterEstimatorConfig meterEstCfg;
		meterEstCfg.SetTempoLimInf( 50 );
		meterEstCfg.SetTempoLimSup( 200 );
		meterEstCfg.SetAutomaticIntegTime( true );
		meterEstCfg.SetACFIntegrationTime( 40 );
		
		CLAM::RhythmDescription::MeterEstimator meterEstimator;
		meterEstimator.Configure( meterEstCfg );
		meterEstimator.Start();
		meterEstimator.Log() << "Processing " <<  *pool.GetWritePool<std::string>("Global","Path") << std::endl;
		meterEstimator.Do( signal, 
				   extractedBeats, 
				   *pool.GetWritePool<CLAM::RhythmDescription::Meter>("Global", "Meter") );
		
		meterEstimator.Stop();

	}
	
}

