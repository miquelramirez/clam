#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert


#include "SMSBase.hxx"
#include "StdOutProgress.hxx"
#include "StdOutWaitMessage.hxx"

#include "Audio.hxx"
#include "AudioFileIn.hxx"

#include <fstream> // used for open(..) : we want to check if some file exists

namespace CLAMTest
{


class SMSExampleTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SMSExampleTest );

class SMSExampleTest : public CppUnit::TestFixture, public CLAM::SMSBase
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
	CPPUNIT_TEST( testhelperLoadSynthesizedAudio );
//	CPPUNIT_TEST( testAnalysisSynthesis_WithDefaultConfig_UsingSweep_Wav );
	CPPUNIT_TEST( testAnalysisSynthesis_WithLoadedConfig_UsingSweep_Wav );
	CPPUNIT_TEST_SUITE_END();



public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:

	/// relative path to the CLAM-TestData dir. Defined below the class declaration.
	static const char* testDataPath;

	/// this helper func. should result in the future in a processing or assert. 
	bool helperAudiosAreEqual(CLAM::Audio& first, CLAM::Audio& second, std::string& whyDifferents, double delta=0.000001 )
	{
		if (first.GetSize() != second.GetSize() )
		{	
			std::stringstream formatter;
			formatter << "different audio sizes: first " << first.GetSize() << " second " << second.GetSize();
			whyDifferents = formatter.str();
			return false;
		}

		CLAM::DataArray& samplesFirst = first.GetBuffer();
		CLAM::DataArray& samplesSecond = second.GetBuffer();
		for (int i=0; i<samplesFirst.Size(); i++)
		{
			if( fabs(samplesFirst[i] - samplesSecond[i]) > delta )
			{
				std::stringstream formatter;
				formatter << "found a different sampler in position " << i << " first value is "<< samplesFirst[i]
					<< " second value is " << samplesSecond[i] << " with delta = " << delta;
				whyDifferents = formatter.str();
				return false;
			}
		}

		return true;
		
	}

	void testhelperAudiosAreEqual_WhenDifferentSizes()
	{
		// Setup
		CLAM::Audio a, b;
		a.SetSize(2);
		b.SetSize(0);

		std::string diagnostic;
		// Verification
		bool resultEquals = helperAudiosAreEqual(a, b, diagnostic);
		CPPUNIT_ASSERT_EQUAL( std::string("different audio sizes: first 2 second 0"), diagnostic );
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
			"found a different sampler in position 9 first value is 1 second value is 0.9 with delta = 1e-006"),
			diagnostic );

		CPPUNIT_ASSERT_EQUAL( false, resultEquals );
	}

	void testhelperAudiosAreEqual_AfterDefaultConstructor()
	{
		CLAM::Audio a,b;
		std::string diagnostic;
		CPPUNIT_ASSERT_EQUAL( true, helperAudiosAreEqual(a, b, diagnostic) );
		CPPUNIT_ASSERT_EQUAL( std::string(), diagnostic );
	}
	// TODO:
	// testhelperAudiosAreEqual_WhenDifferentSamplingRate
	
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

	bool helperFileExist( const std::string& filename ) const
	{
		std::ifstream fs;
		fs.open( filename.c_str() );
		const bool result = !fs.fail();
		fs.close();
		return result;
	}

	void testhelperFileExist()
	{

		CPPUNIT_ASSERT_EQUAL_MESSAGE("current dir should contain file: settings.cfg", true, helperFileExist("settings.cfg") );
		CPPUNIT_ASSERT_EQUAL( false, helperFileExist("thisFileShouldNot.Exist") );
	}

	void testTestDataPath_TestsFilesAreAccessible()
	{
		const std::string path( testDataPath );
	
		CPPUNIT_ASSERT_MESSAGE( 
			"file not found when opening sweep.wav. All files are searched in a dir named CLAM-TestData/ in CLAM-Sandboxes", 
			helperFileExist( path+"sweep.wav" ) );

		CPPUNIT_ASSERT_MESSAGE( 
			"file not found when opening /SMSTests/out_sweep_defConfig.wav. All files are searched in a dir named CLAM-TestData/ in CLAM-Sandboxes", 
			helperFileExist( path+"/SMSTests/out_sweep_defconfig.wav") );
		
		CPPUNIT_ASSERT_MESSAGE( 
			"file not found when opening /SMSTests/out_sweep_defConfig_res.wav. All files are searched in a dir named CLAM-TestData/ in CLAM-Sandboxes", 
			helperFileExist( path+"/SMSTests/out_sweep_defconfig_res.wav") );
		
		CPPUNIT_ASSERT_MESSAGE( 
			"file not found when opening /SMSTests/out_sweep_defConfig_sin.wav. All files are searched in a dir named CLAM-TestData/ in CLAM-Sandboxes", 
			helperFileExist( path+"/SMSTests/out_sweep_defConfig_sin.wav") );

		CPPUNIT_ASSERT_MESSAGE( 
			"file not found when opening /SMSTests/config.xml. All files are searched in a dir named CLAM-TestData/ in CLAM-Sandboxes", 
			helperFileExist( path+"/SMSTests/config.xml") );
		
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
		mGlobalConfig.SetInputSoundFile(std::string(testDataPath)+"sweep.wav");
		CPPUNIT_ASSERT_EQUAL( true, LoadInputSound() );
		mGlobalConfig.SetInputSoundFile("");
	}

	void testLoadInputSound_CalledMultipleTimes()
	{
		InitConfigs();
		mGlobalConfig.SetInputSoundFile(std::string(testDataPath)+"sweep.wav");
		CPPUNIT_ASSERT_EQUAL( true, LoadInputSound() );
		CPPUNIT_ASSERT_EQUAL( true, LoadInputSound() );
		mGlobalConfig.SetInputSoundFile("");
	}
	
	// TODO
	void testAnalysisSynthesis_OriginalAudioDiffersFromProcessedAudio()
	{
	}
	
	void testAnalysisSynthesis_WithDefaultConfig_UsingSweep_Wav()
	{
		mGlobalConfig.SetInputSoundFile(std::string(testDataPath)+"sweep.wav");
		InitConfigs();
		LoadInputSound(); 

		Analyze();
		Synthesize();
		
		CLAM::Audio& audioIn = mOriginalSegment.GetAudio();
		CLAM::Audio& synAudio = mAudioOut;
		std::string diagnostic;

		bool audiosAreEquals = helperAudiosAreEqual(audioIn, synAudio, diagnostic);
		CPPUNIT_ASSERT_EQUAL(std::string(), diagnostic);
		CPPUNIT_ASSERT( audiosAreEquals );

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


	// TODO re-order attributes and methods:
	
	CLAM::Audio mLoadedTestAudio;

	CLAM::Audio& accessorLoadedTestAudio()
	{
		return mLoadedTestAudio;
	}

	void helperLoadSynthesizedAudio(const std::string filename )
	{
		CPPUNIT_ASSERT( helperFileExist(filename) );
		CLAM::AudioFileConfig conf;
		conf.SetFilename(filename);
		conf.SetFiletype(CLAM::EAudioFileType::eWave);

		CLAM::AudioFileIn in;
		in.Configure(conf);
		in.Start();
		
		mLoadedTestAudio.SetSize( in.Size() );
		in.Do( accessorLoadedTestAudio() );
		in.Stop();
	}

	
	void testhelperLoadSynthesizedAudio()
	{
		helperLoadSynthesizedAudio( std::string(testDataPath)+"sweep.wav" );
		CPPUNIT_ASSERT_EQUAL_MESSAGE("sweep.wav size", 181588, accessorLoadedTestAudio().GetSize() );
	}

	void testAnalysisSynthesis_WithLoadedConfig_UsingSweep_Wav()
	{
		const std::string path(testDataPath);
		LoadConfig( path + "/SMSTests/config.xml");
		mGlobalConfig.SetInputSoundFile( path + "sweep.wav");
		InitConfigs();
		LoadInputSound();

		Analyze();
		Synthesize();

		helperLoadSynthesizedAudio( path+"/SMSTests/out_sweep_loadedConfig.wav" );
		std::string diagnostic;
		bool audiosAreEquals = helperAudiosAreEqual( accessorLoadedTestAudio(), accessorSynthesizedAudio(), diagnostic, 0.1);
	
		CPPUNIT_ASSERT_EQUAL(std::string(), diagnostic);
		CPPUNIT_ASSERT( audiosAreEquals );

		mGlobalConfig.SetInputSoundFile("");
	}
	
};

const char* SMSExampleTest::testDataPath = "../../../../../CLAM-TestData/";




} // namespace CLAMTest
