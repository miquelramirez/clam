#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert
#include "AudioCollator.hxx"  // includes audio matching algorithms and audio file helpers


#include "SMSBase.hxx"
#include "StdOutProgress.hxx"
#include "StdOutWaitMessage.hxx"

#include "Audio.hxx"


#include <fstream> // used for open(..) : we want to check if some file exists

namespace CLAMTest
{

/** Simple redifinition of the SMSBase class in order to avoid abstract methods*/
class SMSBaseTest: public CLAM::SMSBase
{
public:
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

	void Run(){}
	
};

class SMSExampleTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SMSExampleTest );


//TODO: Add stress tests that repeatedly analyze/synthesize and analyze/transform/synthesize
//TODO: Look for apparent memory leaks in the above procedure
class SMSExampleTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SMSExampleTest );
	CPPUNIT_TEST( testhelperAudiosAreEqual_WhenDifferentSizes );
	CPPUNIT_TEST( testhelperAudiosAreEqual_WhenDiffersInASampler );
	CPPUNIT_TEST( testhelperAudiosAreEqual_AfterDefaultConstructor );
	CPPUNIT_TEST( testInitConfigs_GenerateCompatibleConfig );
	CPPUNIT_TEST( testhelperFileExist );
	CPPUNIT_TEST( testTestDataPath_TestsFilesAreAccessible );
	CPPUNIT_TEST( testLoadInputSound_WithABadFileName );
	CPPUNIT_TEST( testLoadInputSound_WithAnExistingSoundFile );
	CPPUNIT_TEST( testLoadInputSound_CalledMultipleTimes );
	CPPUNIT_TEST( testhelperLoadAudioFromFile );
	CPPUNIT_TEST( testAnalysisSynthesis_WithDefaultConfig_UsingSine_Wav );
	CPPUNIT_TEST( testAnalysisSynthesis_WithLoadedConfig_UsingSweep_Wav );
	CPPUNIT_TEST( testAnalysisSynthesis_WithLoadedConfig_UsingElvis_Wav );
	CPPUNIT_TEST( testTwoSimpleTransformations_withLoadedScore );
	CPPUNIT_TEST( testTransformations_withLoadedScore_sinusoidalGain );
	CPPUNIT_TEST( testTransformations_withLoadedScore_residualGain );
	CPPUNIT_TEST( testTransformations_withLoadedScore_FreqShift );
	CPPUNIT_TEST( testTransformations_withLoadedScore_PitchShift );
	CPPUNIT_TEST( testTransformations_withLoadedScore_OddEvenHarmonicRatio );
	CPPUNIT_TEST( testTransformations_withLoadedScore_SMSPitchDiscretization );
	CPPUNIT_TEST( testTransformations_withLoadedScore_SMSSpectralShapeShift );
//	CPPUNIT_TEST( testTransformations_withLoadedScore_HarmonizerTimestretch );
//	CPPUNIT_TEST( testTransformations_withLoadedScore_TimestretchMorph );
//	CPPUNIT_TEST( testTransformations_withLoadedScore_Timestretch );
	CPPUNIT_TEST_SUITE_END();



//  TestFixture atributes:
	
	CLAM::Audio mLoadedTestAudio;
	SMSBaseTest* mpBase;

	//! relative path to the CLAM-TestData dir. Defined below the class declaration.
	std::string mPath;

public:
	SMSExampleTest()
	{
		mpBase = NULL;
	}
	
	//! Common initialization, executed before each test method
	void setUp() 
	{ 
		CLAM::ErrAssertionFailed::breakpointInCLAMAssertEnabled = true;
		mPath = "../../../CLAM-TestData/";
		CPPUNIT_ASSERT_EQUAL( true, mpBase == NULL );
		mpBase = new SMSBaseTest;
	}

	//! Common clean up, executed after each test method
	void tearDown() 
	{ 
		CPPUNIT_ASSERT_EQUAL( false, mpBase == NULL );
		delete mpBase;
		mpBase = NULL;
	}

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

	// SMSBase tests :

	void testInitConfigs_GenerateCompatibleConfig()
	{
		mpBase->InitConfigs();
		CPPUNIT_ASSERT_MESSAGE("after InitConfigs, mpBase->HaveCompatibleConfig() should be true", mpBase->HaveCompatibleConfig() );
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
		mpBase->InitConfigs();
		mpBase->GetGlobalConfig().SetInputSoundFile("thisFileShouldAbsolutelyNotExist.wav");
		CPPUNIT_ASSERT_EQUAL( false, mpBase->LoadInputSound() );
		mpBase->GetGlobalConfig().SetInputSoundFile("");

	}

	void testLoadInputSound_WithAnExistingSoundFile()
	{
		mpBase->InitConfigs();
		mpBase->GetGlobalConfig().SetInputSoundFile( mPath+"sweep.wav");
		CPPUNIT_ASSERT_EQUAL( true, mpBase->LoadInputSound() );
		mpBase->GetGlobalConfig().SetInputSoundFile("");
	}

	void testLoadInputSound_CalledMultipleTimes()
	{
		mpBase->InitConfigs();
		mpBase->GetGlobalConfig().SetInputSoundFile( mPath+"sweep.wav");
		CPPUNIT_ASSERT_EQUAL( true, mpBase->LoadInputSound() );
		CPPUNIT_ASSERT_EQUAL( true, mpBase->LoadInputSound() );
		mpBase->GetGlobalConfig().SetInputSoundFile("");
	}
	

	CLAM::Audio& accessorOriginalAudio()
	{
		return mpBase->GetOriginalSegment().GetAudio();
	}
	CLAM::Audio& accessorSynthesizedAudio()
	{
		return mpBase->GetSynthesizedSound();
	}
	CLAM::Audio& accessorSinusoidalAudio()
	{
		return mpBase->GetSynthesizedSinusoidal();
	}
	CLAM::Audio& accessorResidualAudio()
	{
		return mpBase->GetSynthesizedResidual();
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
		mpBase->LoadConfig( mPath + "/SMSTests/sweepConfig.xml");
		mpBase->GetGlobalConfig().SetInputSoundFile( mPath+"sine.wav");
		mpBase->InitConfigs();
		mpBase->LoadInputSound();
		mpBase->Analyze();
		mpBase->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_sine_defaultConfig";		
		double delta=0.09;
		std::string diagnostic;
		std::string diagnosticRes;
		std::string diagnosticSin;
		
		bool synthesizedAudiosAreEqual = 	
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta);

		bool residualAudiosAreEqual = 
			helperAudioIsEqualToAudioFile( accessorResidualAudio(), expectedAudioFile+"_res.wav", diagnosticRes, delta);

		bool sinusoidalAudiosAreEqual = 
			helperAudioIsEqualToAudioFile( accessorSinusoidalAudio(), expectedAudioFile+"_sin.wav", diagnosticSin, delta);
		
		CPPUNIT_ASSERT_MESSAGE( diagnostic, synthesizedAudiosAreEqual );
		CPPUNIT_ASSERT_MESSAGE( diagnosticRes, residualAudiosAreEqual );
		CPPUNIT_ASSERT_MESSAGE( diagnosticSin, sinusoidalAudiosAreEqual );

	}

	void testAnalysisSynthesis_WithLoadedConfig_UsingSweep_Wav()
	{
		mpBase->LoadConfig( mPath + "/SMSTests/sweepConfig.xml");
		mpBase->InitConfigs();
		mpBase->LoadInputSound();
		mpBase->Analyze();
		mpBase->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_sweep_loadedConfig";
		double delta=0.09;
		std::string diagnostic, diagnosticRes, diagnosticSin;
		
		bool synthesizedAudiosAreEqual = 	
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta);

		bool residualAudiosAreEqual = 
			helperAudioIsEqualToAudioFile( accessorResidualAudio(), expectedAudioFile+"_res.wav", diagnosticRes, delta);
		
		bool sinusoidalAudiosAreEqual = 
			helperAudioIsEqualToAudioFile( accessorSinusoidalAudio(), expectedAudioFile+"_sin.wav", diagnosticSin, delta);
		
		CPPUNIT_ASSERT_MESSAGE( diagnostic, synthesizedAudiosAreEqual );
		CPPUNIT_ASSERT_MESSAGE( diagnosticRes, residualAudiosAreEqual );
		CPPUNIT_ASSERT_MESSAGE( diagnosticSin, sinusoidalAudiosAreEqual );

	}

	void testAnalysisSynthesis_WithLoadedConfig_UsingElvis_Wav()
	{
		mpBase->LoadConfig( mPath + "/SMSTests/elvisConfig.xml");
		mpBase->InitConfigs();
		mpBase->LoadInputSound();
		mpBase->Analyze();
		mpBase->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_elvis_loadedConfig";
		double delta=0.09;
		std::string diagnostic, diagnosticRes, diagnosticSin;

		CLAM_ASSERT(22050==accessorOriginalAudio().GetSampleRate(), "original bad SamplRate" );
		CLAM_ASSERT(22050==accessorSinusoidalAudio().GetSampleRate(),  "sinusoidal bad SamplRate" );
		CLAM_ASSERT(22050==accessorSynthesizedAudio().GetSampleRate(), "synth bad SamplRate" );
		
		bool synthesizedAudiosAreEqual = 	
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta);

		bool residualAudiosAreEqual = 
			helperAudioIsEqualToAudioFile( accessorResidualAudio(), expectedAudioFile+"_res.wav", diagnosticRes, delta);

		bool sinusoidalAudiosAreEqual = 
			helperAudioIsEqualToAudioFile( accessorSinusoidalAudio(), expectedAudioFile+"_sin.wav", diagnosticSin, delta);
		
		CPPUNIT_ASSERT_MESSAGE( diagnostic, synthesizedAudiosAreEqual );
		CPPUNIT_ASSERT_MESSAGE( diagnosticSin, sinusoidalAudiosAreEqual );
		CPPUNIT_ASSERT_MESSAGE( diagnosticRes, residualAudiosAreEqual );
	}

	void testTwoSimpleTransformations_withLoadedScore()
	{ 
		mpBase->LoadTransformationScore( mPath + "/SMSTests/wierdfemale-transf.xml" );
		mpBase->LoadConfig( mPath + "/SMSTests/elvisConfig.xml");
		mpBase->InitConfigs();
		mpBase->LoadInputSound();
		mpBase->Analyze();
		mpBase->Transform();
		mpBase->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_wierdfemale-transf";
		double delta = 0.09;
		std::string diagnostic;

		bool transformedAudioAreEqual =
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta );
		CPPUNIT_ASSERT_MESSAGE( diagnostic, transformedAudioAreEqual );
	}

	void testTransformations_withLoadedScore_HarmonizerTimestretch()
	{ 
		mpBase->LoadTransformationScore( mPath + "/SMSTests/harmonizer_timestretch-transf.xml" );
		mpBase->LoadConfig( mPath + "/SMSTests/elvisConfig.xml");
		mpBase->InitConfigs();
		mpBase->LoadInputSound();
		mpBase->Analyze();
		mpBase->Transform();
		mpBase->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_harmonizer-timestretch-transf";
		double delta = 0.09;
		std::string diagnostic;

		bool transformedAudioAreEqual =
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta );
		CPPUNIT_ASSERT_MESSAGE( diagnostic, transformedAudioAreEqual );
	}

      
	void testTransformations_withLoadedScore_Timestretch()
	{ 
		mpBase->LoadTransformationScore( mPath + "/SMSTests/timestretch-transf.xml" );
		mpBase->LoadConfig( mPath + "/SMSTests/elvisConfig.xml");
		mpBase->InitConfigs();
		mpBase->LoadInputSound();
		mpBase->Analyze();
		std::cout<<"After analysis original segment has "<< mpBase->GetOriginalSegment().GetnFrames() <<"frames" <<std::endl;
		mpBase->Transform();
		std::cout<<"After transformation original segment has "<< mpBase->GetOriginalSegment().GetnFrames() <<"frames" <<std::endl;
		std::cout<<"After transformation transformed segment has "<< mpBase->GetTransformedSegment().GetnFrames() <<"frames" <<std::endl;
		mpBase->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_timestretch-transf";
		double delta = 0.09;
		std::string diagnostic;

		bool transformedAudioAreEqual =
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta );
		CPPUNIT_ASSERT_MESSAGE( diagnostic, transformedAudioAreEqual );
	}
	void testTransformations_withLoadedScore_sinusoidalGain()
	{
		mpBase->LoadTransformationScore( mPath + "/SMSTests/sinusoidalGain-transf.xml" );
		mpBase->LoadConfig( mPath + "/SMSTests/default-config_sine.xml");
		mpBase->InitConfigs();
		mpBase->LoadInputSound();
		mpBase->Analyze();
		mpBase->Transform();
		mpBase->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_sinusoidalGain-transf";
		double delta = 0.09;
		std::string diagnostic;

		bool transformedAudioAreEqual =
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta );
		CPPUNIT_ASSERT_MESSAGE( diagnostic, transformedAudioAreEqual );
	}

	void testTransformations_withLoadedScore_residualGain()
	{
		mpBase->LoadTransformationScore( mPath + "/SMSTests/residualGain-transf.xml" );
		mpBase->LoadConfig( mPath + "/SMSTests/default-config_noise.xml");
		mpBase->InitConfigs();
		mpBase->LoadInputSound();
		mpBase->Analyze();
		mpBase->Transform();
		mpBase->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_residualGain-transf";
		double delta = 0.09;
		std::string diagnostic;

		bool transformedAudioAreEqual =
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta );
		CPPUNIT_ASSERT_MESSAGE( diagnostic, transformedAudioAreEqual );

	}

	void testTransformations_withLoadedScore_FreqShift ()
	{
		mpBase->LoadTransformationScore( mPath + "/SMSTests/freqshift-transf.xml" );
		mpBase->LoadConfig( mPath + "/SMSTests/freqshift-config.xml");
		mpBase->InitConfigs();
		mpBase->LoadInputSound();
		mpBase->Analyze();
		mpBase->Transform();
		mpBase->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_freqshift-transf";
		double delta = 0.09;
		std::string diagnostic;

		bool transformedAudioAreEqual =
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta );
		CPPUNIT_ASSERT_MESSAGE( diagnostic, transformedAudioAreEqual );
	}

	void testTransformations_withLoadedScore_PitchShift ()
	{
		mpBase->LoadTransformationScore( mPath + "/SMSTests/pitchshift-transf.xml" );
		mpBase->LoadConfig( mPath + "/SMSTests/elvisConfig.xml");
		mpBase->InitConfigs();
		mpBase->LoadInputSound();
		mpBase->Analyze();
		mpBase->Transform();
		mpBase->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_pitchshift-transf";
		double delta = 0.09;
		std::string diagnostic;

		bool transformedAudioAreEqual =
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta );
		CPPUNIT_ASSERT_MESSAGE( diagnostic, transformedAudioAreEqual );
	}
	
	void testTransformations_withLoadedScore_OddEvenHarmonicRatio()
	{
		mpBase->LoadTransformationScore( mPath + "/SMSTests/oddEvenHarmonicRatio-transf.xml" );
		mpBase->LoadConfig( mPath + "/SMSTests/elvisConfig.xml");
		mpBase->InitConfigs();
		mpBase->LoadInputSound();
		mpBase->Analyze();
		mpBase->Transform();
		mpBase->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_oddEvenHarmonicRatio-transf";
		double delta = 0.09;
		std::string diagnostic;

		bool transformedAudioAreEqual =
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta );
		CPPUNIT_ASSERT_MESSAGE( diagnostic, transformedAudioAreEqual );
	}
	
	void testTransformations_withLoadedScore_SMSPitchDiscretization()
	{
		// the transformation used for this test is actually a chain containing 
		// a pitchshift transformation before the actual pitch discretization
		mpBase->LoadTransformationScore( mPath + "/SMSTests/pitchDiscretization-transf.xml" );
		mpBase->LoadConfig( mPath + "/SMSTests/elvisConfig.xml");
		mpBase->InitConfigs();
		mpBase->LoadInputSound();
		mpBase->Analyze();
		mpBase->Transform();
		mpBase->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_pitchDiscretization-transf";
		double delta = 0.09;
		std::string diagnostic;

		bool transformedAudioAreEqual =
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta );
		CPPUNIT_ASSERT_MESSAGE( diagnostic, transformedAudioAreEqual );
	}
	
	void testTransformations_withLoadedScore_SMSSpectralShapeShift()
	{
		mpBase->LoadTransformationScore( mPath + "/SMSTests/spectralShapeShift-transf.xml" );
		mpBase->LoadConfig( mPath + "/SMSTests/elvisConfig.xml");
		mpBase->InitConfigs();
		mpBase->LoadInputSound();
		mpBase->Analyze();
		mpBase->Transform();
		mpBase->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_spectralShapeShift-transf";
		double delta = 0.09;
		std::string diagnostic;

		bool transformedAudioAreEqual =
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta );
		CPPUNIT_ASSERT_MESSAGE( diagnostic, transformedAudioAreEqual );
	}
	


	void testTransformations_withLoadedScore_TimestretchMorph()
	{
			
		mpBase->LoadTransformationScore( mPath + "/SMSTests/timestretch_morph-transf.xml" );
		mpBase->LoadConfig( mPath + "/SMSTests/elvisMorphConfig.xml");
		mpBase->InitConfigs();
		mpBase->LoadInputSound();
		mpBase->Analyze();
		mpBase->Transform();
		mpBase->Synthesize();

		const std::string expectedAudioFile = mPath+"/SMSTests/out_timestretch_morph-transf";
		double delta = 0.09;
		std::string diagnostic;

		bool transformedAudioAreEqual =
			helperAudioIsEqualToAudioFile( accessorSynthesizedAudio(), expectedAudioFile+".wav", diagnostic, delta );
		CPPUNIT_ASSERT_MESSAGE( diagnostic, transformedAudioAreEqual );
	}

};




} // namespace CLAMTest

