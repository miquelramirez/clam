#include <string>
#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert
#include "SMS_ParameterChange.hxx"
#include "SMS_AnalysisTask.hxx"
#include "SMS_Errors.hxx"
#include "SMSAnalysisSynthesisConfig.hxx"
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>

namespace CLAMTest
{


class SMSAnalysisTaskTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SMSAnalysisTaskTest );

class SMSAnalysisTaskTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SMSAnalysisTaskTest );
	CPPUNIT_TEST( testSetAnalysisFrameLength_ValidValue );
	CPPUNIT_TEST( testSetAnalysisFrameLength_InvalidValue );
	CPPUNIT_TEST( testSetAnalysisFramesOverlapFactor_ValidValue );
	CPPUNIT_TEST( testSetAnalysisFramesOverlapFactor_InvalidValue );
	CPPUNIT_TEST( testSetAnalysisWindowFunction_ValidValue );
	CPPUNIT_TEST( testSetAnalysisWindowFunction_InvalidValue );
	CPPUNIT_TEST( testSetZeroPaddingFactor_ValidValue );
	CPPUNIT_TEST( testSetF0SearchRange_InvalidLowerBound );
	CPPUNIT_TEST( testSetF0SearchRange_InvalidUpperBound );
	CPPUNIT_TEST( testSetF0SearchRange_BoundsCrossed );
	CPPUNIT_TEST( testTaskDefinition_equivalenceWithSMSASConfig );
	CPPUNIT_TEST( testTryToLoadANonMonoAudioFile );
	CPPUNIT_TEST( testTryToChangeAnUnexistingParameter );
	CPPUNIT_TEST( testTaskProcessesACollectionOfChanges_NoConstraintsViolated );
	CPPUNIT_TEST( testTaskProcessesACollectionOfChanges_SomeConstraintsViolated );
	CPPUNIT_TEST( testTaskProcessesAnUnorderedCollectionOfChanges_NoConstraintsViolated );
	CPPUNIT_TEST( testTaskProcessesAnUnorderedCollectionOfChanges_SomeConstraintsViolated );
	CPPUNIT_TEST_SUITE_END();

protected:

	template < typename ParmType >
	static void printMismatchMessage( const ParmType& lvalue,
									  const ParmType& rvalue,
									  const char* parmName )
	{
			std::cerr << std::endl;
			std::cerr << "Left hand side operand ";
			std::cerr << parmName << " was"<< std::endl;
			std::cerr << lvalue << std::endl;
			std::cerr << "and Right hand side operand ";
			std::cerr << parmName << " was "<< std::endl;
			std::cerr << rvalue << std::endl;
	}
									

	static bool areEquals(const CLAM::SMSAnalysisSynthesisConfig& lhs,
				const CLAM::SMSAnalysisSynthesisConfig& rhs )
	{
		// MRJ: argh, this is a truly exhausting experience :)
		if ( lhs.GetInputSoundFile() != rhs.GetInputSoundFile() )
		{
			printMismatchMessage( lhs.GetInputSoundFile(),
								  rhs.GetInputSoundFile(),
								  "InputSoundFile" );
			return false;
		}
		if ( lhs.GetAnalysisWindowSize() != rhs.GetAnalysisWindowSize() )
		{
			printMismatchMessage( lhs.GetAnalysisWindowSize(),
								  rhs.GetAnalysisWindowSize(),
								  "AnalysisWindowSize" );

			return false;
		}
		if ( lhs.GetAnalysisHopSize() != rhs.GetAnalysisHopSize() )
		{
			printMismatchMessage( lhs.GetAnalysisHopSize(),
								rhs.GetAnalysisHopSize(),
								"AnalysisHopSize" );

			return false;
		}
		if ( lhs.GetAnalysisWindowType() != rhs.GetAnalysisWindowType() )
		{
			printMismatchMessage( lhs.GetAnalysisWindowType(),
								rhs.GetAnalysisWindowType(),
								"AnalysisWindowType" );

			return false;
		}
		if ( lhs.GetResAnalysisWindowSize() != rhs.GetResAnalysisWindowSize() )
		{
			printMismatchMessage( lhs.GetResAnalysisWindowSize(),
								rhs.GetResAnalysisWindowSize(),
								"ResAnalysisWindowSize" );

			return false;
		}
		if ( lhs.GetResAnalysisWindowType() != rhs.GetResAnalysisWindowType() )
		{
			printMismatchMessage( lhs.GetResAnalysisWindowType(),
								rhs.GetResAnalysisWindowType(),
								"ResAnalysisWindowType" );

			return false;
		}
		if ( lhs.GetAnalysisZeroPaddingFactor() != rhs.GetAnalysisZeroPaddingFactor() )
		{
			printMismatchMessage( lhs.GetAnalysisZeroPaddingFactor(),
								rhs.GetAnalysisZeroPaddingFactor(),
								"AnalysisZeroPaddingFactor" );
			return false;
		}
		if ( lhs.GetAnalysisPeakDetectMagThreshold() != rhs.GetAnalysisPeakDetectMagThreshold() )
		{
			printMismatchMessage( lhs.GetAnalysisPeakDetectMagThreshold(),
								rhs.GetAnalysisPeakDetectMagThreshold(),
								"AnalysisPeakDetectMagThreshold" );

			return false;
		}
		if ( lhs.GetAnalysisPeakDetectMaxFreq() != rhs.GetAnalysisPeakDetectMaxFreq() )
		{
			printMismatchMessage( lhs.GetAnalysisPeakDetectMaxFreq(),
								rhs.GetAnalysisPeakDetectMaxFreq(),
								"AnalysisPeakDetectMaxFreq" );

			return false;
		}
		if ( lhs.GetAnalysisSinTrackingFreqDeviation() != rhs.GetAnalysisSinTrackingFreqDeviation() )
		{
			printMismatchMessage( lhs.GetAnalysisSinTrackingFreqDeviation(),
								rhs.GetAnalysisSinTrackingFreqDeviation(),
								"AnalysisSinTrackingFreqDeviation" );

			return false;
		}
		if ( lhs.GetAnalysisLowestFundFreq() != rhs.GetAnalysisLowestFundFreq() )
		{
			printMismatchMessage( lhs.GetAnalysisLowestFundFreq(),
								rhs.GetAnalysisLowestFundFreq(),
								"AnalysisLowestFundFreq" );

			return false;
		}
		if ( lhs.GetAnalysisHighestFundFreq() != rhs.GetAnalysisHighestFundFreq() )
		{
			printMismatchMessage( lhs.GetAnalysisHighestFundFreq(),
								rhs.GetAnalysisHighestFundFreq(),
								"AnalysisHighestFundFreq" );

			return false;
		}
		if ( lhs.GetAnalysisHarmonic() != rhs.GetAnalysisHarmonic() )
		{
			printMismatchMessage( lhs.GetAnalysisHarmonic(),
								rhs.GetAnalysisHarmonic(),
								"AnalysisHarmonic" );

			return false;
		}
		if ( lhs.GetDoCleanTracks() != rhs.GetDoCleanTracks() )
		{
			printMismatchMessage( lhs.GetDoCleanTracks(),
								rhs.GetDoCleanTracks(),
								"DoCleanTracks" );

			return false;
		}
		if ( lhs.GetCleanTracksMaxDropOut() != rhs.GetCleanTracksMaxDropOut() )
		{
			printMismatchMessage( lhs.GetCleanTracksMaxDropOut(),
								rhs.GetCleanTracksMaxDropOut(),
								"CleanTracksMaxDropOut" );

			return false;
		}
		if ( lhs.GetCleanTracksMinLength() != rhs.GetCleanTracksMinLength() )
		{
			printMismatchMessage( lhs.GetCleanTracksMinLength(),
								rhs.GetCleanTracksMinLength(),
								"CleanTracksMinLength" );

			return false;
		}
		if ( lhs.GetSynthesisFrameSize() != rhs.GetSynthesisFrameSize() )
		{
			printMismatchMessage( lhs.GetSynthesisFrameSize(),
								rhs.GetSynthesisFrameSize(),
								"SynthesisFrameSize" );

			return false;
		}
		if ( lhs.GetSynthesisWindowType() != rhs.GetSynthesisWindowType() )
		{
			printMismatchMessage( lhs.GetSynthesisWindowType(),
								rhs.GetSynthesisWindowType(),
								"SynthesisWindowType" );

			return false;
		}
		if ( lhs.GetMorphSoundFile() != rhs.GetMorphSoundFile() )
		{
			printMismatchMessage( lhs.GetMorphSoundFile(),
								rhs.GetMorphSoundFile(),
								"MorphSoundFile" );

			return false;
		}
		
		return ( true );
	}	



	
	void buildMockupObject( CLAM::SMSAnalysisSynthesisConfig& cfg )
	{
		cfg.SetInputSoundFile( mPath + "aeiou_lowpass.wav" );
		cfg.SetAnalysisWindowSize( 883 );
		cfg.SetAnalysisHopSize( 220 );
		cfg.SetAnalysisWindowType( CLAM::EWindowType::eBlackmanHarris92 );
		cfg.SetResAnalysisWindowSize( 883 );
		cfg.SetResAnalysisWindowType( CLAM::EWindowType::eBlackmanHarris92 );
		cfg.SetAnalysisZeroPaddingFactor( 2 );
		cfg.SetAnalysisPeakDetectMagThreshold( -150 );
		cfg.SetAnalysisPeakDetectMaxFreq( 8000 );
		cfg.SetAnalysisSinTrackingFreqDeviation( 20 );
		cfg.SetAnalysisLowestFundFreq( 100 );
		cfg.SetAnalysisHighestFundFreq( 1000 );
		cfg.SetAnalysisHarmonic( true );
		cfg.SetDoCleanTracks( false );
		cfg.SetCleanTracksMaxDropOut( 4 );
		cfg.SetCleanTracksMinLength( 3 );
		cfg.SetSynthesisFrameSize( 220 );
		cfg.SetSynthesisWindowType( CLAM::EWindowType::eTriangular );
		cfg.SetMorphSoundFile( "" );
	}


	void shuffleCollectionOfChanges( std::vector< SMS::ParameterChange >& collection )
	{
		srand( time(NULL) );


		for ( int k = 0; k < 10; k++ )
		{
			int i = rand() % collection.size();
			int j = rand() % collection.size();

			SMS::ParameterChange tmp;
			tmp = collection[i];
			collection[i] = collection[j];
			collection[i] = tmp;
		}

	}

	void buildCollectionOfChanges_with4InvalidValues( std::vector< SMS::ParameterChange >& collection )
	{

		SMS::ParameterChange changeFrameCmd;
		changeFrameCmd.SetFieldKey( "Analysis Frame Length" );
		changeFrameCmd.SetNewFieldValue( "20" );

		collection.push_back( changeFrameCmd );

		// Invalid value
		SMS::ParameterChange changeOverlapCmd;
		changeOverlapCmd.SetFieldKey( "Analysis Frame Overlap Factor" );
		changeOverlapCmd.SetNewFieldValue( "1.3" );

		collection.push_back( changeOverlapCmd );

		// Invalid value
		SMS::ParameterChange changeWindowCmd;
		changeWindowCmd.SetFieldKey( "Analysis Window Function" );
		changeWindowCmd.SetNewFieldValue( "Kolmogorov107" );

		collection.push_back( changeWindowCmd );

		SMS::ParameterChange changeZeroPaddingCmd;
		changeZeroPaddingCmd.SetFieldKey( "Zero Padding Factor" );
		changeZeroPaddingCmd.SetNewFieldValue( "2");

		collection.push_back( changeZeroPaddingCmd );

		SMS::ParameterChange changePDMTCmd;
		changePDMTCmd.SetFieldKey( "Peak Detection Magnitude Threshold");
		changePDMTCmd.SetNewFieldValue( "-150");

		collection.push_back( changePDMTCmd );

		SMS::ParameterChange changePDFCCmd;
		changePDFCCmd.SetFieldKey( "Peak Detection Frequency Cutoff" );
		changePDFCCmd.SetNewFieldValue( "8000" );

		collection.push_back( changePDFCCmd );

		// Invalid value
		SMS::ParameterChange changeSTMPDCmd;
		changeSTMPDCmd.SetFieldKey( "Sinusoidal Tracking Maximum Peak Frequency Deviation" );
		changeSTMPDCmd.SetNewFieldValue( "200");

		collection.push_back( changeSTMPDCmd );


		// Invalid value
		SMS::ParameterChange changeSTF0SRCmd;
		changeSTF0SRCmd.SetFieldKey( "Fundamental Frequency Search Range" );
		changeSTF0SRCmd.SetNewFieldValue( "1000 100");

		collection.push_back( changeSTF0SRCmd );

		SMS::ParameterChange changeAHCmd;
		changeAHCmd.SetFieldKey( "Peak Detection Assumes Harmonicity" );
		changeAHCmd.SetNewFieldValue( "1" );
		
		collection.push_back( changeAHCmd );

		SMS::ParameterChange changeATCCmd;
		changeATCCmd.SetFieldKey( "Activate Sinusoidal Track Cleaning" );
		changeATCCmd.SetNewFieldValue( "0" );

		collection.push_back( changeATCCmd );

		SMS::ParameterChange changeTTTDCmd;
		changeTTTDCmd.SetFieldKey( "Tolerance To Track Discontinuation" );
		changeTTTDCmd.SetNewFieldValue( "4" );

		collection.push_back( changeTTTDCmd );

		SMS::ParameterChange changeMTLCmd;
		changeMTLCmd.SetFieldKey( "Minimum Sinusoidal Track Length" );
		changeMTLCmd.SetNewFieldValue( "3" );

		collection.push_back( changeMTLCmd );

	}

	void buildCollectionOfChanges( std::vector< SMS::ParameterChange >& collection )
	{

		SMS::ParameterChange changeFrameCmd;
		changeFrameCmd.SetFieldKey( "Analysis Frame Length" );
		changeFrameCmd.SetNewFieldValue( "20" );

		collection.push_back( changeFrameCmd );

		SMS::ParameterChange changeOverlapCmd;
		changeOverlapCmd.SetFieldKey( "Analysis Frame Overlap Factor" );
		changeOverlapCmd.SetNewFieldValue( "0.25" );

		collection.push_back( changeOverlapCmd );

		SMS::ParameterChange changeWindowCmd;
		changeWindowCmd.SetFieldKey( "Analysis Window Function" );
		changeWindowCmd.SetNewFieldValue( "BlackmanHarris92" );

		collection.push_back( changeWindowCmd );

		SMS::ParameterChange changeZeroPaddingCmd;
		changeZeroPaddingCmd.SetFieldKey( "Zero Padding Factor" );
		changeZeroPaddingCmd.SetNewFieldValue( "2");

		collection.push_back( changeZeroPaddingCmd );

		SMS::ParameterChange changePDMTCmd;
		changePDMTCmd.SetFieldKey( "Peak Detection Magnitude Threshold");
		changePDMTCmd.SetNewFieldValue( "-150");

		collection.push_back( changePDMTCmd );

		SMS::ParameterChange changePDFCCmd;
		changePDFCCmd.SetFieldKey( "Peak Detection Frequency Cutoff" );
		changePDFCCmd.SetNewFieldValue( "8000" );

		collection.push_back( changePDFCCmd );

		SMS::ParameterChange changeSTMPDCmd;
		changeSTMPDCmd.SetFieldKey( "Sinusoidal Tracking Maximum Peak Frequency Deviation" );
		changeSTMPDCmd.SetNewFieldValue( "20");

		collection.push_back( changeSTMPDCmd );

		SMS::ParameterChange changeSTF0SRCmd;
		changeSTF0SRCmd.SetFieldKey( "Fundamental Frequency Search Range" );
		changeSTF0SRCmd.SetNewFieldValue( "100 1000");

		collection.push_back( changeSTF0SRCmd );

		SMS::ParameterChange changeAHCmd;
		changeAHCmd.SetFieldKey( "Peak Detection Assumes Harmonicity" );
		changeAHCmd.SetNewFieldValue( "1" );
		
		collection.push_back( changeAHCmd );

		SMS::ParameterChange changeATCCmd;
		changeATCCmd.SetFieldKey( "Activate Sinusoidal Track Cleaning" );
		changeATCCmd.SetNewFieldValue( "0" );

		collection.push_back( changeATCCmd );

		SMS::ParameterChange changeTTTDCmd;
		changeTTTDCmd.SetFieldKey( "Tolerance To Track Discontinuation" );
		changeTTTDCmd.SetNewFieldValue( "4" );

		collection.push_back( changeTTTDCmd );

		SMS::ParameterChange changeMTLCmd;
		changeMTLCmd.SetFieldKey( "Minimum Sinusoidal Track Length" );
		changeMTLCmd.SetNewFieldValue( "3" );

		collection.push_back( changeMTLCmd );
		
	}

	std::string mPath;

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{ 
		mPath = "../../../../../CLAM-TestData/SMSTests/";
	}

	/// Common clean up, executed after each test method
	void tearDown() 
	{ 
	}

private:

	void testTryToChangeAnUnexistingParameter()
	{
		// Setup

		// Exercise
		SMS::AnalysisTask newTask;

		SMS::ParameterChange changeCmd;
		changeCmd.SetFieldKey( "Momonga rulez!" );
		changeCmd.SetNewFieldValue( mPath + "aeiou_lowpass.wav" );

		try
		{
			newTask.Process( changeCmd );
		}
		catch( SMS::UnexistingParameter& error )
		{
			error.WhatHappened();
			return;
		}
		catch( ... )
		{
			CPPUNIT_FAIL( "No constraint should have been violated!" );
		}
		
		CPPUNIT_FAIL("No error was thrown!");
	}


	void testSetAnalysisFrameLength_ValidValue()
	{
		SMS::AnalysisTask newTask;

		SMS::ParameterChange changeCmd;
		changeCmd.SetFieldKey( "Sound Source" );
		changeCmd.SetNewFieldValue( mPath+ "aeiou_lowpass.wav" );

		try
		{
			newTask.Process( changeCmd );
			
		}
		catch( ... )
		{
			CPPUNIT_FAIL( "Unexpected error thrown!" ) ;
		}

		CPPUNIT_ASSERT_EQUAL( true, newTask.SetAnalysisFrameLength( 20. ) );
	}

	void testSetAnalysisFrameLength_InvalidValue()
	{
		SMS::AnalysisTask newTask;
		
		SMS::ParameterChange changeSoundSourceCmd;
		changeSoundSourceCmd.SetFieldKey( "Sound Source" );
		changeSoundSourceCmd.SetNewFieldValue( mPath+ "aeiou_lowpass.wav" );

		SMS::ParameterChange changeFrameCmd;
		changeFrameCmd.SetFieldKey( "Analysis Frame Length" );
		changeFrameCmd.SetNewFieldValue( "4501" );

		try
		{
			newTask.Process( changeSoundSourceCmd );
			newTask.Process( changeFrameCmd );
		}
		catch( SMS::ParameterConstraintViolated& error )
		{
			std::string culpritField( "Analysis Frame Length" );

			std::list< std::string >::const_iterator i;

			i = std::find( error.GetAffectedFieldsList().begin(),
						error.GetAffectedFieldsList().end(),
						culpritField );

			CPPUNIT_ASSERT( i!=error.GetAffectedFieldsList().end() );

			return; // test passed
		}
		catch( ... )
		{
			CPPUNIT_FAIL( "Unexpected error found!" );
		}

		CPPUNIT_FAIL("The error was not detected!");
	}

	void testSetAnalysisFramesOverlapFactor_ValidValue()
	{
		SMS::AnalysisTask newTask;

		SMS::ParameterChange changeSoundSourceCmd;
		changeSoundSourceCmd.SetFieldKey( "Sound Source" );
		changeSoundSourceCmd.SetNewFieldValue( mPath+ "aeiou_lowpass.wav" );

		SMS::ParameterChange changeFrameCmd;
		changeFrameCmd.SetFieldKey( "Analysis Frame Length" );
		changeFrameCmd.SetNewFieldValue( "20" );

		SMS::ParameterChange changeOverlapCmd;
		changeOverlapCmd.SetFieldKey( "Analysis Frame Overlap Factor" );
		changeOverlapCmd.SetNewFieldValue( "0.25" );

		try
		{
			newTask.Process( changeSoundSourceCmd );
			newTask.Process( changeFrameCmd );
			newTask.Process( changeOverlapCmd );
		}
		catch( ... )
		{
			CPPUNIT_FAIL( "Unexpected error found!" );
		}

	}

	void testSetAnalysisFramesOverlapFactor_InvalidValue()
	{
		SMS::AnalysisTask newTask;

		SMS::ParameterChange changeSoundSourceCmd;
		changeSoundSourceCmd.SetFieldKey( "Sound Source" );
		changeSoundSourceCmd.SetNewFieldValue( mPath+ "aeiou_lowpass.wav" );

		SMS::ParameterChange changeFrameCmd;
		changeFrameCmd.SetFieldKey( "Analysis Frame Length" );
		changeFrameCmd.SetNewFieldValue( "20" );

		SMS::ParameterChange changeOverlapCmd;
		changeOverlapCmd.SetFieldKey( "Analysis Frame Overlap Factor" );
		changeOverlapCmd.SetNewFieldValue( "1.0" );

		try
		{
			newTask.Process( changeSoundSourceCmd );
			newTask.Process( changeFrameCmd );
			newTask.Process( changeOverlapCmd );

		}
		catch( SMS::ParameterConstraintViolated& error )
		{
			std::string culpritField( "Analysis Frame Overlap Factor" );

			std::list< std::string >::const_iterator i;

			i = std::find( error.GetAffectedFieldsList().begin(),
						error.GetAffectedFieldsList().end(),
						culpritField );

			CPPUNIT_ASSERT( i!=error.GetAffectedFieldsList().end() );

			return;
		}
		catch( ... )
		{
			CPPUNIT_FAIL("Unexpected error thrown!");
		}

		CPPUNIT_FAIL("Expected error was not thrown!");

	}

	void testSetAnalysisWindowFunction_ValidValue()
	{
		SMS::AnalysisTask newTask;

		SMS::ParameterChange changeSoundSourceCmd;
		changeSoundSourceCmd.SetFieldKey( "Sound Source" );
		changeSoundSourceCmd.SetNewFieldValue( mPath+ "aeiou_lowpass.wav" );

		SMS::ParameterChange changeFrameCmd;
		changeFrameCmd.SetFieldKey( "Analysis Frame Length" );
		changeFrameCmd.SetNewFieldValue( "20" );

		SMS::ParameterChange changeOverlapCmd;
		changeOverlapCmd.SetFieldKey( "Analysis Frame Overlap Factor" );
		changeOverlapCmd.SetNewFieldValue( "0.5" );


		SMS::ParameterChange changeWindowCmd;
		changeWindowCmd.SetFieldKey( "Analysis Window Function" );
		changeWindowCmd.SetNewFieldValue( "BlackmanHarris92" );

		try
		{
			newTask.Process( changeSoundSourceCmd );
			newTask.Process( changeFrameCmd );
			newTask.Process( changeOverlapCmd );
			newTask.Process( changeWindowCmd );
		}
		catch( ... )
		{
			CPPUNIT_FAIL("Unexpected error was thrown");
		}

	}

	void testSetAnalysisWindowFunction_InvalidValue()
	{
		SMS::AnalysisTask newTask;

		SMS::ParameterChange changeSoundSourceCmd;
		changeSoundSourceCmd.SetFieldKey( "Sound Source" );
		changeSoundSourceCmd.SetNewFieldValue( mPath+ "aeiou_lowpass.wav" );

		SMS::ParameterChange changeFrameCmd;
		changeFrameCmd.SetFieldKey( "Analysis Frame Length" );
		changeFrameCmd.SetNewFieldValue( "20" );

		SMS::ParameterChange changeOverlapCmd;
		changeOverlapCmd.SetFieldKey( "Analysis Frame Overlap Factor" );
		changeOverlapCmd.SetNewFieldValue( "0.5" );


		SMS::ParameterChange changeWindowCmd;
		changeWindowCmd.SetFieldKey( "Analysis Window Function" );
		changeWindowCmd.SetNewFieldValue( "Kishigura73" );

		try
		{
			newTask.Process( changeSoundSourceCmd );
			newTask.Process( changeFrameCmd );
			newTask.Process( changeOverlapCmd );
			newTask.Process( changeWindowCmd );
		}
		catch( SMS::ParameterConstraintViolated& error )
		{
			std::string culpritField( "Analysis Window Function" );

			std::list< std::string >::const_iterator i;

			i = std::find( error.GetAffectedFieldsList().begin(),
						error.GetAffectedFieldsList().end(),
						culpritField );

			CPPUNIT_ASSERT( i!=error.GetAffectedFieldsList().end() );
			
			return;
		}
		catch( ... )
		{
			CPPUNIT_FAIL("Unexpected error was thrown");
		}

		CPPUNIT_FAIL("No error was thrown" );
	}

	void testSetZeroPaddingFactor_ValidValue()
	{
		SMS::AnalysisTask newTask;

		SMS::ParameterChange changeSoundSourceCmd;
		changeSoundSourceCmd.SetFieldKey( "Sound Source" );
		changeSoundSourceCmd.SetNewFieldValue( mPath+ "aeiou_lowpass.wav" );

		SMS::ParameterChange changeFrameCmd;
		changeFrameCmd.SetFieldKey( "Analysis Frame Length" );
		changeFrameCmd.SetNewFieldValue( "20" );

		SMS::ParameterChange changeOverlapCmd;
		changeOverlapCmd.SetFieldKey( "Analysis Frame Overlap Factor" );
		changeOverlapCmd.SetNewFieldValue( "0.5" );


		SMS::ParameterChange changeWindowCmd;
		changeWindowCmd.SetFieldKey( "Analysis Window Function" );
		changeWindowCmd.SetNewFieldValue( "BlackmanHarris92" );

		SMS::ParameterChange changeZeroPaddingCmd;
		changeZeroPaddingCmd.SetFieldKey( "Zero Padding Factor" );
		changeZeroPaddingCmd.SetNewFieldValue( "2");

		try
		{
			newTask.Process( changeSoundSourceCmd );
			newTask.Process( changeFrameCmd );
			newTask.Process( changeOverlapCmd );
			newTask.Process( changeWindowCmd );		
			newTask.Process( changeZeroPaddingCmd );
		}
		catch( ... )
		{
			CPPUNIT_FAIL( "Unexpected error was thrown!" );
		}

	}

	void testTaskDefinition_equivalenceWithSMSASConfig()
	{
		CLAM::SMSAnalysisSynthesisConfig cfg;
		buildMockupObject( cfg );

		SMS::AnalysisTask newTask;

		SMS::ParameterChange changeSoundSourceCmd;
		changeSoundSourceCmd.SetFieldKey( "Sound Source" );
		changeSoundSourceCmd.SetNewFieldValue( mPath+ "aeiou_lowpass.wav" );

		SMS::ParameterChange changeFrameCmd;
		changeFrameCmd.SetFieldKey( "Analysis Frame Length" );
		changeFrameCmd.SetNewFieldValue( "20" );

		SMS::ParameterChange changeOverlapCmd;
		changeOverlapCmd.SetFieldKey( "Analysis Frame Overlap Factor" );
		changeOverlapCmd.SetNewFieldValue( "0.25" );


		SMS::ParameterChange changeWindowCmd;
		changeWindowCmd.SetFieldKey( "Analysis Window Function" );
		changeWindowCmd.SetNewFieldValue( "BlackmanHarris92" );

		SMS::ParameterChange changeZeroPaddingCmd;
		changeZeroPaddingCmd.SetFieldKey( "Zero Padding Factor" );
		changeZeroPaddingCmd.SetNewFieldValue( "2");

		SMS::ParameterChange changePDMTCmd;
		changePDMTCmd.SetFieldKey( "Peak Detection Magnitude Threshold");
		changePDMTCmd.SetNewFieldValue( "-150");

		SMS::ParameterChange changePDFCCmd;
		changePDFCCmd.SetFieldKey( "Peak Detection Frequency Cutoff" );
		changePDFCCmd.SetNewFieldValue( "8000" );

		SMS::ParameterChange changeSTMPDCmd;
		changeSTMPDCmd.SetFieldKey( "Sinusoidal Tracking Maximum Peak Frequency Deviation" );
		changeSTMPDCmd.SetNewFieldValue( "20");

		SMS::ParameterChange changeSTF0SRCmd;
		changeSTF0SRCmd.SetFieldKey( "Fundamental Frequency Search Range" );
		changeSTF0SRCmd.SetNewFieldValue( "100 1000");

		SMS::ParameterChange changeAHCmd;
		changeAHCmd.SetFieldKey( "Peak Detection Assumes Harmonicity" );
		changeAHCmd.SetNewFieldValue( "1" );
		
		SMS::ParameterChange changeATCCmd;
		changeATCCmd.SetFieldKey( "Activate Sinusoidal Track Cleaning" );
		changeATCCmd.SetNewFieldValue( "0" );

		SMS::ParameterChange changeTTTDCmd;
		changeTTTDCmd.SetFieldKey( "Tolerance To Track Discontinuation" );
		changeTTTDCmd.SetNewFieldValue( "4" );

		SMS::ParameterChange changeMTLCmd;
		changeMTLCmd.SetFieldKey( "Minimum Sinusoidal Track Length" );
		changeMTLCmd.SetNewFieldValue( "3" );

		try
		{
			newTask.Process( changeSoundSourceCmd );
			newTask.Process( changeFrameCmd );
			newTask.Process( changeOverlapCmd );
			newTask.Process( changeWindowCmd );
			newTask.Process( changeZeroPaddingCmd );
			newTask.Process( changePDMTCmd );
			newTask.Process( changePDFCCmd );
			newTask.Process( changeSTMPDCmd );
			newTask.Process( changeSTF0SRCmd );
			newTask.Process( changeATCCmd );
			newTask.Process( changeTTTDCmd );
			newTask.Process( changeMTLCmd );
		}
		catch( ... )
		{
			CPPUNIT_FAIL("Unexpected error was thrown!");
		}

		CPPUNIT_ASSERT_EQUAL( true, areEquals( cfg, newTask.GetConfig() ) );
	}

	void testTryToLoadANonMonoAudioFile()
	{
		SMS::AnalysisTask newTask;
		bool result;
		
		try
		{
			result = newTask.SetSoundSource( mPath+ "stereo-file.wav" );
		}
		catch( SMS::NotMono& e )
		{
			e.WhatHappened();
		}
		catch( ... )
		{
			CPPUNIT_FAIL( "Unexpected exception thrown!" );
		}

				
	}

	void testSetF0SearchRange_InvalidLowerBound()
	{
		SMS::AnalysisTask newTask;

		SMS::ParameterChange changeSoundSourceCmd;
		changeSoundSourceCmd.SetFieldKey( "Sound Source" );
		changeSoundSourceCmd.SetNewFieldValue( mPath+ "aeiou_lowpass.wav" );

		SMS::ParameterChange changeSTF0SRCmd;
		changeSTF0SRCmd.SetFieldKey( "Fundamental Frequency Search Range" );
		changeSTF0SRCmd.SetNewFieldValue( "-100 1000");

		try
		{
			newTask.Process( changeSoundSourceCmd );
			newTask.Process( changeSTF0SRCmd );
		}
		catch( SMS::ParameterConstraintViolated& error )
		{
			std::string culpritField( "Fundamental Frequency Search Range" );

			std::list< std::string >::const_iterator i;

			i = std::find( error.GetAffectedFieldsList().begin(),
						error.GetAffectedFieldsList().end(),
						culpritField );

			CPPUNIT_ASSERT( i!=error.GetAffectedFieldsList().end() );
			
			return;
		}
		catch( ... )
		{
			CPPUNIT_FAIL( "Unexpected error was thrown!" );
		}

		CPPUNIT_FAIL( "No error was thrown!" );

	}

	void testSetF0SearchRange_InvalidUpperBound()
	{
		SMS::AnalysisTask newTask;

		SMS::ParameterChange changeSoundSourceCmd;
		changeSoundSourceCmd.SetFieldKey( "Sound Source" );
		changeSoundSourceCmd.SetNewFieldValue( mPath+ "aeiou_lowpass.wav" );

		SMS::ParameterChange changeSTF0SRCmd;
		changeSTF0SRCmd.SetFieldKey( "Fundamental Frequency Search Range" );
		changeSTF0SRCmd.SetNewFieldValue( "100 100000");

		try
		{
			newTask.Process( changeSoundSourceCmd );
			newTask.Process( changeSTF0SRCmd );
		}
		catch( SMS::ParameterConstraintViolated& error )
		{
			std::string culpritField( "Fundamental Frequency Search Range" );

			std::list< std::string >::const_iterator i;

			i = std::find( error.GetAffectedFieldsList().begin(),
						error.GetAffectedFieldsList().end(),
						culpritField );

			CPPUNIT_ASSERT( i!=error.GetAffectedFieldsList().end() );
			
			return;
		}
		catch( ... )
		{
			CPPUNIT_FAIL( "Unexpected error was thrown!" );
		}

		CPPUNIT_FAIL( "No error was thrown!" );

	}

	void testSetF0SearchRange_BoundsCrossed()
	{
		SMS::AnalysisTask newTask;

		SMS::ParameterChange changeSoundSourceCmd;
		changeSoundSourceCmd.SetFieldKey( "Sound Source" );
		changeSoundSourceCmd.SetNewFieldValue( mPath+ "aeiou_lowpass.wav" );

		SMS::ParameterChange changeSTF0SRCmd;
		changeSTF0SRCmd.SetFieldKey( "Fundamental Frequency Search Range" );
		changeSTF0SRCmd.SetNewFieldValue( "3000 100");

		try
		{
			newTask.Process( changeSoundSourceCmd );
			newTask.Process( changeSTF0SRCmd );
		}
		catch( SMS::ParameterConstraintViolated& error )
		{
			std::string culpritField( "Fundamental Frequency Search Range" );

			std::list< std::string >::const_iterator i;

			i = std::find( error.GetAffectedFieldsList().begin(),
				       error.GetAffectedFieldsList().end(),
				       culpritField );

			CPPUNIT_ASSERT( i!=error.GetAffectedFieldsList().end() );
			
			return;
		}
		catch( ... )
		{
			CPPUNIT_FAIL( "Unexpected error was thrown!" );
		}

		CPPUNIT_FAIL( "No error was thrown!" );
	}

	void testTaskProcessesACollectionOfChanges_NoConstraintsViolated()
	{
		std::vector< SMS::ParameterChange > changesCollection;
		
		buildCollectionOfChanges( changesCollection );

		SMS::AnalysisTask newTask;

		SMS::ParameterChange changeSoundSourceCmd;
		changeSoundSourceCmd.SetFieldKey( "Sound Source" );
		changeSoundSourceCmd.SetNewFieldValue( mPath+ "aeiou_lowpass.wav" );

		try
		{
			newTask.Process( changeSoundSourceCmd );
			newTask.Process( changesCollection );
		}
		catch( ... )
		{
			CPPUNIT_FAIL( "Unexpected error thrown!" );
		}

	}

	void testTaskProcessesACollectionOfChanges_SomeConstraintsViolated()
	{
		std::vector< SMS::ParameterChange > changesCollection;
		
		buildCollectionOfChanges_with4InvalidValues( changesCollection );

		SMS::AnalysisTask newTask;
		SMS::ParameterChange changeSoundSourceCmd;
		changeSoundSourceCmd.SetFieldKey( "Sound Source" );
		changeSoundSourceCmd.SetNewFieldValue( mPath+ "aeiou_lowpass.wav" );


		try
		{
			newTask.Process( changeSoundSourceCmd );
			newTask.Process( changesCollection );
		}
		catch( SMS::MultipleParameterConstraintsViolated& error )
		{
			CPPUNIT_ASSERT( error.TotalConstraintsViolated() == 4 );
			return;
		}
		catch( ... )
		{
			CPPUNIT_FAIL( "Unexpected error thrown!" );
		}

		CPPUNIT_FAIL("No error was thrown!");
	}

	void testTaskProcessesAnUnorderedCollectionOfChanges_NoConstraintsViolated()
	{
		std::vector< SMS::ParameterChange > changesCollection;
		
		buildCollectionOfChanges( changesCollection );

		shuffleCollectionOfChanges( changesCollection );

		SMS::AnalysisTask newTask;
		SMS::ParameterChange changeSoundSourceCmd;
		changeSoundSourceCmd.SetFieldKey( "Sound Source" );
		changeSoundSourceCmd.SetNewFieldValue( mPath+ "aeiou_lowpass.wav" );

		try
		{
			newTask.Process( changeSoundSourceCmd );

			newTask.Process( changesCollection );
		}
		catch( ... )
		{
			CPPUNIT_FAIL( "Unexpected error thrown!" );
		}

	}

	void testTaskProcessesAnUnorderedCollectionOfChanges_SomeConstraintsViolated()
	{
		std::vector< SMS::ParameterChange > changesCollection;
		
		buildCollectionOfChanges_with4InvalidValues( changesCollection );

		shuffleCollectionOfChanges( changesCollection );

		SMS::AnalysisTask newTask;
		SMS::ParameterChange changeSoundSourceCmd;
		changeSoundSourceCmd.SetFieldKey( "Sound Source" );
		changeSoundSourceCmd.SetNewFieldValue( mPath+ "aeiou_lowpass.wav" );


		try
		{
			newTask.Process( changeSoundSourceCmd );
			newTask.Process( changesCollection );
		}
		catch( SMS::MultipleParameterConstraintsViolated& error )
		{
			CPPUNIT_ASSERT( error.TotalConstraintsViolated() == 4 );
			return;
		}
		catch( ... )
		{
			CPPUNIT_FAIL( "Unexpected error thrown!" );
		}

		CPPUNIT_FAIL("No error was thrown!");
	}

};






} // namespace CLAMTest
