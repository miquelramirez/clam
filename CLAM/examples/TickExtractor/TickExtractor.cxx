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

#include "TickExtractor.hxx"
#include "TickExtractorConfig.hxx"
#include "Err.hxx"
#include "Pulse.hxx"
#include "Audio.hxx"
#include "XMLStorage.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileReaderConfig.hxx"
#include "AudioFile.hxx"
#include "OnsetDetector.hxx"
#include "TickSequenceTracker.hxx"
#include "IOIHistogram.hxx"
#include "Normalization.hxx"


namespace CLAM
{

	static void LoadAudioFile( CLAM::Audio & audio, const std::string & audioFileName )
	{
		CLAM::AudioFile file;
		file.OpenExisting( audioFileName );
		
		if ( !file.IsReadable() )
		{
			std::string errStr = "Error: file " + file.GetLocation() + " cannot be opened ";
			errStr += "or is encoded in an unrecognized format\n";
			
			throw Err( errStr.c_str() );

		}
		
		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile( file );
		CLAM::MonoAudioFileReader reader;
		reader.Configure( cfg );
		
		CLAM::TSize fileSize = int((file.GetHeader().GetLength()/1000.)*file.GetHeader().GetSampleRate());
		
		audio.SetSize(fileSize);
		audio.SetSampleRate(file.GetHeader().GetSampleRate());
		
		//Read Audio File
		reader.Start();		
		reader.Do(audio);
		reader.Stop();
	}


	void ExtractTicksSequence( std::string pathToFile,
				   CLAM::Pulse& ticksList,
				   CLAM::Pulse& beatsList )
	{
		TickExtractorConfig defaultConfig;

		ExtractTicksSequence( pathToFile, defaultConfig, ticksList, beatsList );
	}

	static void OnsetDetection( const TickExtractorConfig& configuration, 
				    Audio& audioFromFile,
				    Array<TimeIndex>& transients )
	{
		Segment seg;
		seg.AddAudio();
		seg.UpdateData();
		seg.SetHoldsData(true);
		seg.GetAudio().SetSize( audioFromFile.GetSize() );
		seg.GetAudio().SetSampleRate( audioFromFile.GetSampleRate() );
		TTime duration = audioFromFile.GetSize()/audioFromFile.GetSampleRate();			
		TData sampleRate = audioFromFile.GetSampleRate();
		// Audio normalization pass
		
		Normalization audioNormalizer;
		NormalizationConfig audioNormCfg;
		
		audioNormCfg.SetType( 3 ); // Scaling factor computed from "dominant energy"
		
		audioNormalizer.Configure( audioNormCfg );
		
		audioNormalizer.Start();
		
		audioNormalizer.Do( audioFromFile, seg.GetAudio() );
		
		audioNormalizer.Stop();
		
		
		seg.SetEndTime(duration);		
		
		OnsetDetectorConfig onsetconfig;
		OnsetDetector onset;
		
		onsetconfig.SetComputeOffsets(false);
		onsetconfig.SetGlobalThreshold(25);
		
		onset.Configure(onsetconfig);
		
		onset.Start();
		onset.Do(seg, transients);
		
		if ( transients.Size() > 0 )
		{
			TimeIndex nullTransient;
			nullTransient.SetPosition( 0.0 );
			nullTransient.SetWeight( 0.0 );
			
			transients.InsertElem( 0, nullTransient );
			
			for ( int k = 0; k < transients.Size(); k++ )
			{
				transients[k].SetPosition( transients[k].GetPosition()*sampleRate );
				transients[k].SetWeight( transients[k].GetWeight() );
			}
		}
	}
	
	void ExtractTicksSequence( std::string pathToFile,
				   const TickExtractorConfig& configuration,
				   CLAM::Pulse& tickSequence,
				   CLAM::Pulse& beatSequence )	
	{

		//configuration.SetFromAudio( false );		
		Audio   audioFromFile;

		try
		{
			LoadAudioFile( audioFromFile, pathToFile );
		}
		catch( Err& e )
		{
			Err propErr("Error opening file: \n" );
			propErr.Embed(e);
			
			throw propErr;
		}

		Array< TimeIndex > transients;
		TData sampleRate = audioFromFile.GetSampleRate();


		OnsetDetection( configuration, audioFromFile, transients );
		
		// Ticks ( and beats ) computation 

		RhythmDescription::TickSequenceTracker myTickSequenceTracker;
		
		RhythmDescription::TickSequenceTrackerConfig myTickSequenceTrackerConfig;

		myTickSequenceTrackerConfig.SetComputeBeats(configuration.GetComputeBeats());
		myTickSequenceTrackerConfig.SetThreshold_IOIHistPeaks(configuration.GetThreshold_IOIHistPeaks());
		myTickSequenceTrackerConfig.SetTempoLimInf(configuration.GetTempoLimInf());
		myTickSequenceTrackerConfig.SetTempoLimSup(configuration.GetTempoLimSup());
		myTickSequenceTrackerConfig.SetTickLimInf(configuration.GetTickLimInf());
		myTickSequenceTrackerConfig.SetTickLimSup(configuration.GetTickLimSup());
		myTickSequenceTrackerConfig.SetDeviationPenalty(configuration.GetDeviationPenalty());
		myTickSequenceTrackerConfig.SetOverSubdivisionPenalty(configuration.GetOverSubdivisionPenalty());
		myTickSequenceTrackerConfig.SetGaussianWindowSize(configuration.GetGaussianWindowSize());
		myTickSequenceTrackerConfig.SetScope(configuration.GetScope());
		myTickSequenceTrackerConfig.SetAdjustWithOnsets(configuration.GetAdjustWithOnsets());
		myTickSequenceTrackerConfig.SetNTrans(configuration.GetNTrans());
		myTickSequenceTrackerConfig.SetTransHop(configuration.GetTransHop());
		
		myTickSequenceTrackerConfig.SetSampleRate(sampleRate);
		
		myTickSequenceTracker.Configure(myTickSequenceTrackerConfig);		

		RhythmDescription::IOIHistogram ioiHistogram;


		myTickSequenceTracker.Start();
		
		//Use the transients computed in this main
		myTickSequenceTracker.Do( transients, tickSequence, beatSequence, ioiHistogram );

		myTickSequenceTracker.Stop();	

	}
}

