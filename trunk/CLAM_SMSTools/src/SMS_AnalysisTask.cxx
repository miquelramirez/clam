#include "SMS_AnalysisTask.hxx"
#include "AudioFileIn.hxx"
#include "GlobalEnums.hxx"
#include "SMS_Errors.hxx"
#include "ErrSoundFileIO.hxx"
#include <algorithm>
#include <sstream>

namespace SMS
{
	AnalysisTask::AnalysisTask()
	{
		// Default initialization comes from that of SMSAnalysisSynthesisConfig

		// we assume a 44.1 kHz sample rate by default
		mSubjectAudio.SetSampleRate( 44100. );
		mSubjectAudio.SetSize( 44100 );
		SetAnalysisFrameOverlapFactor( mSMSToolsConfig.GetAnalysisHopSize()/mSMSToolsConfig.GetAnalysisWindowSize() );
		SetAnalysisFrameLength( (mSMSToolsConfig.GetAnalysisWindowSize()/mSubjectAudio.GetSampleRate())*1000. );
		SetAnalysisWindowFunction( mSMSToolsConfig.GetAnalysisWindowType().GetString() );
		SetZeroPaddingFactor( mSMSToolsConfig.GetAnalysisZeroPaddingFactor() );
		SetPeakDetectMagThreshold( mSMSToolsConfig.GetAnalysisPeakDetectMagThreshold() );
		SetPeakFrequencyCutoff( mSMSToolsConfig.GetAnalysisPeakDetectMaxFreq() );
		SetMaximumPeakDeviation( mSMSToolsConfig.GetAnalysisSinTrackingFreqDeviation() );
		SetFundamentalFreqSearchRange( mSMSToolsConfig.GetAnalysisLowestFundFreq(),
					       mSMSToolsConfig.GetAnalysisHighestFundFreq() );
		AssumeHarmonicity( mSMSToolsConfig.GetAnalysisHarmonic() );
		ActivateTrackCleaning( mSMSToolsConfig.GetDoCleanTracks() );
		// These two are not checked on initialization
		mTrackDiscTolerance = mSMSToolsConfig.GetCleanTracksMaxDropOut();
		mTrackMinimumLength = mSMSToolsConfig.GetCleanTracksMinLength();
		
	}

	AnalysisTask::~AnalysisTask()
	{
	}

	void AnalysisTask::Process( const std::vector< ParameterChange >& collectionOfCmds )
	{
		std::vector< ParameterChange >::const_iterator currentPos = collectionOfCmds.begin();

		MultipleParameterConstraintsViolated errorCollection;

		while ( currentPos != collectionOfCmds.end() )
		{
			try
			{
				Process( *currentPos );
				
				currentPos++;
			}
			catch( ParameterConstraintViolated& error )
			{
				errorCollection.Add( error );
				currentPos++;
			}
		}

		if ( errorCollection.TotalConstraintsViolated() > 0 )
			throw errorCollection;
	}

	void AnalysisTask::Process( const ParameterChange& cmd )
	{
		if ( cmd.GetFieldKey() == std::string( "Sound Source" ) )
			SetSoundSource( cmd.GetNewFieldValue() );
		else if ( cmd.GetFieldKey() == std::string( "Analysis Frame Length") )
		{
			// Parameter value unmarshalling
			std::stringstream unmarshaller;
			unmarshaller << cmd.GetNewFieldValue();
			
			// unmarshalled value
			CLAM::TData value;
			unmarshaller >> value;

			SetAnalysisFrameLength( value );
		}
		else if ( cmd.GetFieldKey() == std::string( "Analysis Frame Overlap Factor" ) )
		{
			// Parameter value unmarshalling
			std::stringstream unmarshaller;
			unmarshaller << cmd.GetNewFieldValue();
			
			// unmarshalled value
			CLAM::TData value;
			unmarshaller >> value;

			SetAnalysisFrameOverlapFactor( value );
		}
		else if ( cmd.GetFieldKey() == std::string( "Analysis Window Function") )
		{
			// Here there is no need for unmarshalling

			SetAnalysisWindowFunction( cmd.GetNewFieldValue() );
		}
		else if ( cmd.GetFieldKey() == std::string( "Zero Padding Factor") )
		{
			// Unmarshalling
			std::stringstream unmarshaller;
			unmarshaller << cmd.GetNewFieldValue();
			CLAM::TSize zeroPaddingFactor;
			unmarshaller >> zeroPaddingFactor;

			SetZeroPaddingFactor( zeroPaddingFactor );

		}
		else if ( cmd.GetFieldKey() == std::string( "Peak Detection Magnitude Threshold" ))
		{
			// Unmarshalling
			std::stringstream unmarshaller;
			unmarshaller << cmd.GetNewFieldValue();
			CLAM::TSize magThreshold;
			unmarshaller >> magThreshold;

			SetPeakDetectMagThreshold( magThreshold );
		}
		else if ( cmd.GetFieldKey() == std::string( "Peak Detection Frequency Cutoff" ) )
		{
			// Unmarshalling
			std::stringstream unmarshaller;
			unmarshaller << cmd.GetNewFieldValue();
			CLAM::TData freqCutoff;
			unmarshaller >> freqCutoff;

			SetPeakFrequencyCutoff( freqCutoff );
		}
		else if ( cmd.GetFieldKey() == std::string( "Sinusoidal Tracking Maximum Peak Frequency Deviation" ) )
		{
			// Unmarshalling
			std::stringstream unmarshaller;
			unmarshaller << cmd.GetNewFieldValue();
			CLAM::TData freqDevPercentil;
			unmarshaller >> freqDevPercentil;
			
			SetMaximumPeakDeviation( freqDevPercentil );
		}
		else if ( cmd.GetFieldKey() == std::string( "Fundamental Frequency Search Range" ) )
		{
			// Unmarshalling
			std::stringstream unmarshaller;
			unmarshaller << cmd.GetNewFieldValue();
			CLAM::TData lowerBound;
			unmarshaller >> lowerBound;
			CLAM::TData upperBound;
			unmarshaller >> upperBound;

			SetFundamentalFreqSearchRange( lowerBound, upperBound );
		}
		else if ( cmd.GetFieldKey() == std::string( "Peak Detection Assumes Harmonicity" ) )
		{
			// Unmarshalling
			std::stringstream unmarshaller;
			unmarshaller << cmd.GetNewFieldValue();
			int value;
			unmarshaller >> value;
			
			bool safeBooleanValue = false;
			
			if ( value > 0 ) safeBooleanValue = true;
			else if ( value == 0 ) safeBooleanValue = false;
			else
			{
				ParameterConstraintViolated error;
				error.SetMessageString( "This parameter only accepts boolean values ( true/1, false/0 )" );
				error.AddAffectedField( cmd.GetFieldKey() );
			}
			
			AssumeHarmonicity( safeBooleanValue );
			
		}
		else if ( cmd.GetFieldKey() == std::string( "Activate Sinusoidal Track Cleaning" ) )		       
		{
			// Unmarshalling
			std::stringstream unmarshaller;
			unmarshaller << cmd.GetNewFieldValue();
			int value;
			unmarshaller >> value;
			
			bool safeBooleanValue = false;
			
			if ( value > 0 ) safeBooleanValue = true;
			else if ( value == 0 ) safeBooleanValue = false;
			else
			{
				ParameterConstraintViolated error;
				error.SetMessageString( "This parameter only accepts boolean values ( true/1, false/0 )" );
				error.AddAffectedField( cmd.GetFieldKey() );
			}
			
			ActivateTrackCleaning( safeBooleanValue );
		}
		else if ( cmd.GetFieldKey() == std::string( "Tolerance To Track Discontinuation" ) )
		{
			// Unmarshalling
			std::stringstream unmarshaller;
			unmarshaller << cmd.GetNewFieldValue();
			CLAM::TSize numOfFrames;
			unmarshaller >> numOfFrames;

			SetToleranceToTrackDiscontinuation( numOfFrames );
		}
		else if ( cmd.GetFieldKey() == std::string( "Minimum Sinusoidal Track Length" ) )
		{
			// Unmarshalling
			std::stringstream unmarshaller;
			unmarshaller << cmd.GetNewFieldValue();
			CLAM::TSize numOfFrames;
			unmarshaller >> numOfFrames;

			SetMinimumTrackLength( numOfFrames );
		}
		else
		{
			UnexistingParameter error( cmd );
			
			throw error;
		}
	}

	bool AnalysisTask::SetSoundSource( std::string filename )
	{
		CLAM::AudioFileConfig fileLoaderCfg;
		fileLoaderCfg.SetFilename( filename );
		
		if ( filename.find( ".wav" ) != std::string::npos )			
			fileLoaderCfg.SetFiletype( CLAM::EAudioFileType::eWave );
		else if ( filename.find( ".aif" ) != std::string::npos )
			fileLoaderCfg.SetFiletype( CLAM::EAudioFileType::eAIFF );
		else
		{
			UnsupportedFileFormat error;
			error.SetOffendingFilename( filename );

			throw error;
		}

		CLAM::AudioFileIn fileLoader;

		try
		{
			fileLoader.Configure( fileLoaderCfg );
		}
		catch( CLAM::UnavailableSoundFile& thrownError )
		{
			UnavailableFile error;
			error.SetOffendingFilename( filename );
			
			throw error;
		}
		catch( CLAM::UnsupportedSoundFileSampleEncoding& thrownError )
		{
			UnsupportedAudioSampleFormat error;
			error.SetOffendingFilename( filename );

			throw error;
		}
		catch( CLAM::UnsupportedSoundFileFormat& thrownError )
		{
			UnsupportedFileFormat error;
			error.SetOffendingFilename( filename );

			throw error;
		}
		catch( CLAM::Err& thrownError )
		{
			FileError error;
			error.SetOffendingFilename( filename );

			throw error;
		}
		catch( ... )
		{
			FileError error;
			error.SetOffendingFilename( filename );
			
			throw error;
		}
		

		if ( fileLoader.Channels() > 1 )
		{
			NotMono error;
			error.SetOffendingFilename( filename );

			throw error;
		}
		
		mSubjectAudio.SetSampleRate( fileLoader.SampleRate() );
		mSubjectAudio.SetSize( fileLoader.Size() );

		fileLoader.Start();

		bool result;
		result = fileLoader.Do( mSubjectAudio );

		if ( !result )
		{
			FileError error;
			error.SetOffendingFilename( filename );

			throw error;
		}

		fileLoader.Stop();

		if ( result )
		{
			mSubjectAudioFilename = filename;
			// And now we apply the pertinent side effects
			CLAM::TData newLength = mSMSToolsConfig.GetAnalysisWindowSize()/(mSubjectAudio.GetSampleRate());
			newLength *= 1000.; // from secs to millisecs
			SetAnalysisFrameLength( newLength );
			mSMSToolsConfig.SetInputSoundFile( mSubjectAudioFilename );
		}
		return result;

	}

	bool AnalysisTask::SetAnalysisFrameLength( CLAM::TTime value )
	{
		if ( value <= 0.0f || value > GetSoundLength() )
		{
			ParameterConstraintViolated error;
			error.SetMessageString( "The specified value was out of range. Valid Analysis frame length values "
						"should be below the sound length.");
			error.AddAffectedField( std::string("Analysis Frame Length" ));

			throw error;
		}
		
		mAnalysisFrameLength = value;
		
		// Applying side effects
		CLAM::TSize frameSize = (mAnalysisFrameLength/1000.) * mSubjectAudio.GetSampleRate();

		// oddity enforcing
		frameSize = ( frameSize % 2 == 0 ) ? frameSize+1 : frameSize;
		

		mSMSToolsConfig.SetAnalysisWindowSize( frameSize );
		mSMSToolsConfig.SetResAnalysisWindowSize( frameSize );
		mSMSToolsConfig.SetAnalysisHopSize( frameSize * mAnalysisFrameOverlapFactor );
		mSMSToolsConfig.SetSynthesisFrameSize( mSMSToolsConfig.GetAnalysisHopSize() );
		UpdateExpectedNumberOfFrames();
		
		return true;
	}

	void AnalysisTask::UpdateExpectedNumberOfFrames()
	{
		mExpectedNumberOfFrames = GetSoundLength() / mAnalysisFrameLength;
		mExpectedNumberOfFrames *= 1.0/ ( ( mAnalysisFrameOverlapFactor > 0.0 )? mAnalysisFrameOverlapFactor : 1.0);
	}

	bool AnalysisTask::SetAnalysisFrameOverlapFactor( double percentage )
	{
		if ( percentage < 0. || percentage > 0.995 )
		{
			ParameterConstraintViolated error;
			error.SetMessageString( "Valid range for analysis frame overlap factor is 0.0 to 0.99" );
			error.AddAffectedField( std::string( "Analysis Frame Overlap Factor") );
			
			throw error;
		}

		mAnalysisFrameOverlapFactor = percentage;
		
		mSMSToolsConfig.SetAnalysisHopSize( mSMSToolsConfig.GetAnalysisWindowSize()*
						    mAnalysisFrameOverlapFactor );
		mSMSToolsConfig.SetSynthesisFrameSize( mSMSToolsConfig.GetAnalysisHopSize() );	
		UpdateExpectedNumberOfFrames();
		
		return true;
	}

	void AnalysisTask::GetAvailableWindowFunctions( std::list< std::string >& winFuncs )
	{
		CLAM::Enum::tEnumValue* pCurrentEnumValue = 
			CLAM::EWindowType::sEnumValues;

		while( pCurrentEnumValue != NULL 
		       && pCurrentEnumValue->name != NULL )
		{
			winFuncs.push_back( pCurrentEnumValue->name );
			pCurrentEnumValue++;
		}
	}

	bool AnalysisTask::SetAnalysisWindowFunction( std::string funcName )
	{
		std::list< std::string > availableFuncs;

		GetAvailableWindowFunctions( availableFuncs );

		std::list< std::string >::iterator position =
			std::find( availableFuncs.begin(),
				   availableFuncs.end(),
				   funcName );

		if ( position == availableFuncs.end() )
		{
			ParameterConstraintViolated error;
			std::string errorMsgStr;
			errorMsgStr = "The specified window function you specified is not currently supported. \n";
			errorMsgStr += "The set of window functions we currently support are:\n";
				
			for ( std::list< std::string >::iterator currentPos = availableFuncs.begin();
			      currentPos != availableFuncs.end();
			      currentPos++ )
			{
				errorMsgStr += "\t+";
				errorMsgStr += *currentPos;
				errorMsgStr += "\n";
			}

			error.SetMessageString( errorMsgStr );
			error.AddAffectedField( "Analysis Window Function" );

			throw error;

			return false;
		}

		mAnalysisWindowFunction = funcName;

		mSMSToolsConfig.SetAnalysisWindowType( mAnalysisWindowFunction );
		mSMSToolsConfig.SetResAnalysisWindowType( mAnalysisWindowFunction );

		return true;
	}

	bool AnalysisTask::SetZeroPaddingFactor( CLAM::TSize factor )
	{
		mZeroPaddingFactor = factor;

		mSMSToolsConfig.SetAnalysisZeroPaddingFactor( mZeroPaddingFactor );

		return true;
	}

	bool AnalysisTask::SetPeakDetectMagThreshold( CLAM::TData threshold )
	{
		mPeakDetectMagThreshold = threshold;
		
		mSMSToolsConfig.SetAnalysisPeakDetectMagThreshold( mPeakDetectMagThreshold );
		
		return true;
	}

	bool AnalysisTask::SetPeakFrequencyCutoff( CLAM::TData freq )
	{
		if ( freq < 0
		     || freq > mSubjectAudio.GetSampleRate()/2. )
		{
			ParameterConstraintViolated error;
			std::string errorMsgStr = "Spectral peak frequency cutoff values must be between 0 Hz and";
			std::stringstream marshaller;
			marshaller << mSubjectAudio.GetSampleRate()/2.;
			errorMsgStr += marshaller.str();
			errorMsgStr += " Hz";
			error.SetMessageString( errorMsgStr );

			error.AddAffectedField( "Peak Detection Frequency Cutoff" );

			throw error;
			
			return false;
		}

		mPeakFrequencyCutoff = freq;

		mSMSToolsConfig.SetAnalysisPeakDetectMaxFreq( mPeakFrequencyCutoff );

		return true;
	}

	bool AnalysisTask::SetMaximumPeakDeviation( CLAM::TData percentage )
	{
		if ( percentage < 0.
		     || percentage > 100. )
		{
			ParameterConstraintViolated error;
			error.SetMessageString( "Maximum allowed spectral peak frequency deviation must be"
						" a percentage between 0% and 100%" );

			error.AddAffectedField( "Sinusoidal Tracking Maximum Peak Frequency Deviation" );

			throw error;
			
			return false;
		}
		mMaxPeakDeviation = percentage;

		mSMSToolsConfig.SetAnalysisSinTrackingFreqDeviation( mMaxPeakDeviation );

		return true;
	}

	bool AnalysisTask::SetFundamentalFreqSearchRange( CLAM::TData freqLow,
								 CLAM::TData freqHi )
	{
		
		bool validLowerBound = true;
		bool validUpperBound = true;
		bool boundsDoNotCross = true;

		if ( freqLow < 0
		     || freqLow > mSubjectAudio.GetSampleRate()/2. )
			validLowerBound =  false;

		if ( freqHi < 0
		     || freqHi > mSubjectAudio.GetSampleRate()/2. )
			validUpperBound = false;

		if ( freqLow >= freqHi )
			boundsDoNotCross = false;

		if ( !validLowerBound || !validUpperBound || !boundsDoNotCross ) // Some problem arised with specified range
		{
			ParameterConstraintViolated error;
			std::stringstream marshaller;
			marshaller << mSubjectAudio.GetSampleRate()/2.;
			std::string errorMessageStr = "";

			if ( !validLowerBound )
			{
				errorMessageStr += "Lower bound to fundamental frequency search must be in the 0Hz to ";
				errorMessageStr += marshaller.str();
				errorMessageStr += " Hz\n";
			}
			if ( !validUpperBound )
			{
				errorMessageStr += "Upper bound to fundamental frequency search must be in the 0 Hz to ";
				errorMessageStr += marshaller.str();
				errorMessageStr += " Hz\n";
			}
			if ( !boundsDoNotCross )
			{
				errorMessageStr += "Bounds frequency to fundamental frequency search cannot cross.\n";
			}

			error.SetMessageString( errorMessageStr );
			error.AddAffectedField( "Fundamental Frequency Search Range" );

			throw error;

			return false;
		}

		mF0SearchLowerBound = freqLow;
		mF0SearchUpperBound = freqHi;

		mSMSToolsConfig.SetAnalysisLowestFundFreq( mF0SearchLowerBound );
		mSMSToolsConfig.SetAnalysisHighestFundFreq( mF0SearchUpperBound );
		
		return true;
	}

	bool AnalysisTask::AssumeHarmonicity( bool status )
	{
		mMustAssumeHarmonicity = status;

		mSMSToolsConfig.SetAnalysisHarmonic( mMustAssumeHarmonicity );

		return true;
	}

	bool AnalysisTask::ActivateTrackCleaning( bool status )
	{
		mTrackCleaningIsActivated = status;

		mSMSToolsConfig.SetDoCleanTracks( mTrackCleaningIsActivated );

		return true;
	}

	bool AnalysisTask::SetToleranceToTrackDiscontinuation( CLAM::TSize maxDiscont )
	{
		if ( maxDiscont < 0
		     || maxDiscont > mExpectedNumberOfFrames )
		{
			ParameterConstraintViolated error;
			std::string errorMsgStr;

			std::stringstream marshaller;
			marshaller << mExpectedNumberOfFrames;
			
			errorMsgStr += "This value must be a positive integer greater or equal than zero ";
			errorMsgStr += "and less than ";
			errorMsgStr += marshaller.str();
			errorMsgStr += ", the expected number of frames to be produced by the SMS Analysis algorithm\n";

			error.SetMessageString( errorMsgStr );
			error.AddAffectedField( "Tolerance To Track Discontinuation" );

			throw error;
			
			return false;
		}

		mTrackDiscTolerance = maxDiscont;

		mSMSToolsConfig.SetCleanTracksMaxDropOut( mTrackDiscTolerance );

		return true;
	}

	bool AnalysisTask::SetMinimumTrackLength( CLAM::TSize minLength )
	{
		if ( minLength < 0
		     || minLength > mExpectedNumberOfFrames )
		{
			ParameterConstraintViolated error;

			std::stringstream marshaller;
			marshaller << mExpectedNumberOfFrames;

			std::string errorMsgStr;
			
			errorMsgStr += "This value must be a positive integer greater or equal than zero ";
			errorMsgStr += "and less than ";
			errorMsgStr += marshaller.str();
			errorMsgStr += ", the expected number of frames to be produced by the SMS Analysis algorithm\n";

			error.SetMessageString( errorMsgStr );
			error.AddAffectedField( "Minimum Sinusoidal Track Length" );

			throw error;	

			return false;
		}

		mTrackMinimumLength = minLength;
		
		mSMSToolsConfig.SetCleanTracksMinLength( mTrackMinimumLength );
		
		return true;
	}

}
