#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert
#include "AudioCollator.hxx"  // includes audio matching algorithms and audio file helpers


#include "SMSBase.hxx"
#include "StdOutProgress.hxx"
#include "StdOutWaitMessage.hxx"

#include "Audio.hxx"
#include "AudioFileIn.hxx"
#include "AudioFileOut.hxx"


#include <fstream> // used for open(..) : we want to check if some file exists

namespace CLAMTest
{


class SMSExampleTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SMSExampleTest );

class SMSExampleTest : public CppUnit::TestFixture, public CLAM::SMSBase
{
	CPPUNIT_TEST_SUITE( SMSExampleTest );
//	CPPUNIT_TEST( testhelperAudiosAreEqual_WhenDifferentSizes );
//	CPPUNIT_TEST( testhelperAudiosAreEqual_WhenDiffersInASampler );
//	CPPUNIT_TEST( testhelperAudiosAreEqual_AfterDefaultConstructor );
//	CPPUNIT_TEST( testInitConfigs_GenerateCompatibleConfig );
//	CPPUNIT_TEST( testhelperFileExist );
//	CPPUNIT_TEST( testTestDataPath_TestsFilesAreAccessible );
//	CPPUNIT_TEST( testLoadInputSound_WithABadFileName );
//	CPPUNIT_TEST( testLoadInputSound_WithAnExistingSoundFile );
//	CPPUNIT_TEST( testLoadInputSound_CalledMultipleTimes );
//	CPPUNIT_TEST( testhelperLoadAudioFromFile );
	CPPUNIT_TEST( testAnalysisSynthesis_WithDefaultConfig_UsingSine_Wav );
//	CPPUNIT_TEST( testAnalysisSynthesis_WithLoadedConfig_UsingSweep_Wav );
//	CPPUNIT_TEST( testAnalysisSynthesis_WithLoadedConfig_UsingElvis_Wav );
//	CPPUNIT_TEST( testTwoSimpleTransformations_withLoadedScore );
	CPPUNIT_TEST( testTransformations_withLoadedScore_FreqShift );
//	CPPUNIT_TEST( testTransformations_withLoadedScore_HarmonizerTimestreach );
//	CPPUNIT_TEST( testTransformations_withLoadedScore_TimestreachMorph );
	CPPUNIT_TEST_SUITE_END();



//  TestFixture atributes:
	
	CLAM::Audio mLoadedTestAudio;

	//! relative path to the CLAM-TestData dir. Defined below the class declaration.
	std::string mPath;

public:
	//! Common initialization, executed before each test method
	void setUp() 
	{ 
		mPath = "../../../CLAM-TestData/";
	}

	//! Common clean up, executed after each test method
	void tearDown() 
	{ 
		mGlobalConfig.SetInputSoundFile("");
	}

	void Run(){}

private:
	
	void testhelperAudiosAreEqual_WhenDifferentSizes()
	{
		// Setup
		CLAM::Audio a, b;
		a.SetSize(2);
		b.SetSize(0);

		std::string diagnostic;
		// Verification
		bool resultEquals = helperAudiosAreEqual(a, b, diagnostic);
		CPPUNIT_ASSERT_EQUAL( std::string("different audio sizes: first 2 second 0\n"), diagnostic );
		CPPUNIT_ASSERT_EQUAL( false, resultEquals );
	}

	void testhelperAudiosAreEqual_WhenDiffersInASampler()
	{
		CLAM::Audio a, b;
		const int size = 10;
		a.SetSize(size);
		b.SetSize(size);
		CLAM::DataArray& aBuff= a.GetBuffer();
		CLAM::DataArray& bBuff= b.GetBuffer();
		const CLAM::TData _1 = (CLAM::TData) 1.0;
		const CLAM::TData _0_9 = (CLAM::TData) 0.9;
		for (int i=0; i<size; i++)
		{
			aBuff[i]=CLAM::TData(1.0);
			bBuff[i]=_1;
		}
		bBuff[size-1]=_0_9;

		std::string diagnostic;
		const bool resultEquals = helperAudiosAreEqual(a, b, diagnostic);
		CPPUNIT_ASSERT_EQUAL( std::string(
			"found a different sampler in position 9 first value is 1 second value is 0.9 with delta = 0.001"),
			diagnostic );

		CPPUNIT_ASSERT_EQUAL( false, resultEquals );
	}

	void testhelperAudiosAreEqual_AfterDefaultConstructor()
	{
		CLAM::Audio a,b;
		std::string diagnostic;
		CPPUNIT_ASSERT_EQUAL( true, helperAudiosAreEqual(a, b, diagnostic) );
		CPPUNIT_ASSERT_EQUAL( std::string(""), diagnostic );
	}
	
// ---------------------------------------------------------------------------------------------------

	// implementation of CLAM::SMSBase virtual methods 
	CLAMGUI::Progress* CreateProgress(const char* title, float from, float to)
	{
		return new CLAMGUI::StdOutProgress(title, from, to);
	}
	CLAMGUI::WaitMessage* CreateWaitMessage(const char * title)
	{
		return new CLAMGUI::StdOutWaitMessage(title);
	}

// ---------------------------------------------------------------------------------------------------

	// SMSBase tests :

	void testInitConfigs_GenerateCompatibleConfig()
	{
		InitConfigs();
		CPPUNIT_ASSERT_MESSAGE("after InitConfigs, HaveCompatibleConfig() should be true", HaveCompatibleConfig() );
	}


	void testhelperFileExist()
	{

		CPPUNIT_ASSERT_EQUAL_MESSAGE("current dir should contain file: settings.cfg", true, helperFileExist("settings.cfg") );
		CPPUNIT_ASSERT_EQUAL( false, helperFileExist("thisFileShouldNot.Exist") );
	}

	void testTestDataPath_TestsFilesAreAccessible()
	{
		
		CPPUNIT_ASSERT_MESSAGE( 
			"file not found when opening sweep.wav."
			" All files are searched in a dir named CLAM-TestData/ in CLAM-Sandboxes", 
			helperFileExist( mPath+"sweep.wav" ) );

		CPPUNIT_ASSERT_MESSAGE( 
			"file not found when opening /SMSTests/out_sweep_defConfig.wav."
			" All files are searched in a dir named CLAM-TestData/ in CLAM-Sandboxes", 
			helperFileExist( mPath+"/SMSTests/out_sweep_defConfig.wav") );
		
		CPPUNIT_ASSERT_MESSAGE( 
			"file not found when opening /SMSTests/out_sweep_defConfig_res.wav."
			" All files are searched in a dir named CLAM-TestData/ in CLAM-Sandboxes", 
			helperFileExist( mPath+"/SMSTests/out_sweep_defConfig_res.wav") );
		
		CPPUNIT_ASSERT_MESSAGE( 
			"file not found when opening /SMSTests/out_sweep_defConfig_sin.wav."
			" All files are searched in a dir named CLAM-TestData/ in CLAM-Sandboxes", 
			helperFileExist( mPath+"/SMSTests/out_sweep_defConfig_sin.wav") );

		CPPUNIT_ASSERT_MESSAGE( 
			"file not found when opening /SMSTests/config.xml."
			" All files are searched in a dir named CLAM-TestData/ in CLAM-Sandboxes", 
			helperFileExist( mPath+"/SMSTests/sweepConfig.xml") );
		
	}

	void testLoadInputSound_WithABadFileName()
	{
		InitConfigs();
		mGlobalConfig.SetInputSoundFile("thisFileShouldAbsolutelyNotExist.wav");
		CPPUNIT_ASSERT_EQUAL( false, LoadInputSound() );
		mGlobalConfig.SetInputSoundFile("");

	}

	void testLoadInputSound_WithAnExistingSoundFile()
	{
		InitConfigs();
		mGlobalConfig.SetInputSoundFile( mPath+"sweep.wav");
		CPPUNIT_ASSERT_EQUAL( true, LoadInputSound() );
		mGlobalConfig.SetInputSoundFile("");
	}

	void testLoadInputSound_CalledMultipleTimes()
	{
		InitConfigs();
		mGlobalConfig.SetInputSoundFile( mPath+"sweep.wav");
		CPPUNIT_ASSERT_EQUAL( true, LoadInputSound() );
		CPPUNIT_ASSERT_EQUAL( true, LoadInputSound() );
		mGlobalConfig.SetInputSoundFile("");
	}
	

	CLAM::Audio& accessorOriginalAudio()
	{
		return mOriginalSegment.GetAudio();
	}
	CLAM::Audio& accessorSynthesizedAudio()
	{
		return mAudioOut;
	}
	CLAM::Audio& accessorSinusoidalAudio()
	{
		return mAudioOutSin;
	}
	CLAM::Audio& accessorResidualAudio()
	{
		return mAudioOutRes;
	}



	CLAM::Audio& accessorLoadedTestAudio()
	{
		return mLoadedTestAudio;
	}

	
	void testhelperLoadAudioFromFile()
	{
		CLAM::Audio loaded;
		helperLoadAudioFromFile( mPath+"sweep.wav", loaded );
		CPPUNIT_ASSERT_EQUAL_MESSAGE("sweep.wav size", 181588, loaded.GetSize() );
	}


//-------------------------------------------------------------------------

	void testAnalysisSynthesis_WithDefaultConfig_UsingSine_Wav()
	{
		// TODO: make it with default config	
		LoadConfig( mPath + "/SMSTests/sweepConfig.xml");
		mGlobalConfig.SetInputSoundFile( mPath+"sine.wav");
		InitConfigs();
		LoadInputSound();
		Analyze();
		Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_sine_defaultConfig";		
		double delta=0.09;
		std::string diagnostic;
		std::string diagnosticRes;
		std::string diagnosticSin;
		
		bool synthesizedAudiosAreEqual = 	
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta);

		bool residualAudiosAreEqual = 
			helperAudioIsEqualToAudioFile( accessorResidualAudio(), expectedAudioFile+"_res.wav", diagnostic, delta);

		bool sinusoidalAudiosAreEqual = 
			helperAudioIsEqualToAudioFile( accessorSinusoidalAudio(), expectedAudioFile+"_sin.wav", diagnostic, delta);
		
		CPPUNIT_ASSERT_MESSAGE( diagnostic, synthesizedAudiosAreEqual );
		CPPUNIT_ASSERT_MESSAGE( diagnosticRes, residualAudiosAreEqual );
		CPPUNIT_ASSERT_MESSAGE( diagnosticSin, sinusoidalAudiosAreEqual );

	}

	void testAnalysisSynthesis_WithLoadedConfig_UsingSweep_Wav()
	{
		LoadConfig( mPath + "/SMSTests/sweepConfig.xml");
		InitConfigs();
		LoadInputSound();
		Analyze();
		Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_sweep_loadedConfig";
		double delta=0.09;
		std::string diagnostic;
		
		bool synthesizedAudiosAreEqual = 	
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta);
		CPPUNIT_ASSERT_MESSAGE( diagnostic, synthesizedAudiosAreEqual );

		bool residualAudiosAreEqual = 
			helperAudioIsEqualToAudioFile( accessorResidualAudio(), expectedAudioFile+"_res.wav", diagnostic, delta);
		CPPUNIT_ASSERT_MESSAGE( diagnostic, residualAudiosAreEqual );

		bool sinusoidalAudiosAreEqual = 
			helperAudioIsEqualToAudioFile( accessorSinusoidalAudio(), expectedAudioFile+"_sin.wav", diagnostic, delta);
		CPPUNIT_ASSERT_MESSAGE( diagnostic, sinusoidalAudiosAreEqual );

	}

	void testAnalysisSynthesis_WithLoadedConfig_UsingElvis_Wav()
	{
		LoadConfig( mPath + "/SMSTests/elvisConfig.xml");
		InitConfigs();
		LoadInputSound();
		Analyze();
		Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_elvis_loadedConfig";
		double delta=0.09;
		std::string diagnostic;

		CLAM_ASSERT(22050==accessorOriginalAudio().GetSampleRate(), "original bad SamplRate" );
		CLAM_ASSERT(22050==accessorSinusoidalAudio().GetSampleRate(),  "sinusoidal bad SamplRate" );
		CLAM_ASSERT(22050==accessorSynthesizedAudio().GetSampleRate(), "synth bad SamplRate" );
		
		bool synthesizedAudiosAreEqual = 	
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta);
		CPPUNIT_ASSERT_MESSAGE( diagnostic, synthesizedAudiosAreEqual );

		bool residualAudiosAreEqual = 
			helperAudioIsEqualToAudioFile( accessorResidualAudio(), expectedAudioFile+"_res.wav", diagnostic, delta);
		CPPUNIT_ASSERT_MESSAGE( diagnostic, residualAudiosAreEqual );

		bool sinusoidalAudiosAreEqual = 
			helperAudioIsEqualToAudioFile( accessorSinusoidalAudio(), expectedAudioFile+"_sin.wav", diagnostic, delta);
		CPPUNIT_ASSERT_MESSAGE( diagnostic, sinusoidalAudiosAreEqual );
	}

	void testTwoSimpleTransformations_withLoadedScore()
	{ 
		LoadTransformationScore( mPath + "/SMSTests/wierdfemale-transf.xml" );
		LoadConfig( mPath + "/SMSTests/elvisConfig.xml");
		InitConfigs();
		LoadInputSound();
		Analyze();
		Transform();
		Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_wierdfemale-transf";
		double delta = 0.09;
		std::string diagnostic;

		bool transformedAudioAreEqual =
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta );
		CPPUNIT_ASSERT_MESSAGE( diagnostic, transformedAudioAreEqual );
	}

	void testTransformations_withLoadedScore_HarmonizerTimestreach()
	{ 
		LoadTransformationScore( mPath + "/SMSTests/harmonizer_timestreach-transf.xml" );
		LoadConfig( mPath + "/SMSTests/elvisConfig.xml");
		InitConfigs();
		LoadInputSound();
		Analyze();
		Transform();
		Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_harmonizer-timestreach-transf";
		double delta = 0.09;
		std::string diagnostic;

		bool transformedAudioAreEqual =
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta );
		CPPUNIT_ASSERT_MESSAGE( diagnostic, transformedAudioAreEqual );
	}

	void testTransformations_withLoadedScore_FreqShift ()
	{
		LoadTransformationScore( mPath + "/SMSTests/freqshift-transf.xml" );
		LoadConfig( mPath + "/SMSTests/freqshift-config.xml");
		InitConfigs();
		LoadInputSound();
		Analyze();
		Transform();
		Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_freqshift-transf";
		double delta = 0.09;
		std::string diagnostic;

		bool transformedAudioAreEqual =
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta );
		CPPUNIT_ASSERT_MESSAGE( diagnostic, transformedAudioAreEqual );
	}
	
	void testTransformations_withLoadedScore_TimestreachMorph()
	{
//		CLAM::ErrAssertionFailed::breakpointInCLAMAssertEnabled = true;
			
		LoadTransformationScore( mPath + "/SMSTests/timestreach_morph-transf.xml" );
		LoadConfig( mPath + "/SMSTests/elvisMorphConfig.xml");
		InitConfigs();
		LoadInputSound();
		Analyze();
		Transform();
		Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_timestreach_morph-transf";
		double delta = 0.09;
		std::string diagnostic;

		bool transformedAudioAreEqual =
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta );
		CPPUNIT_ASSERT_MESSAGE( diagnostic, transformedAudioAreEqual );
	}

};




} // namespace CLAMTest
