
#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"	
#include "AudioCollator.hxx"

#include "SimpleOscillator.hxx"
#include "SMSAnalysisCore.hxx"
#include "SMSSynthesis.hxx"
#include "Audio.hxx"
#include "AudioOutPort.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
#include "Network.hxx"
#include "BasicFlowControl.hxx"


namespace CLAMTest
{

class SMSSynthesisTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SMSSynthesisTest );

class SMSSynthesisTest : public CppUnit::TestFixture
{
public:
	
	CPPUNIT_TEST_SUITE( SMSSynthesisTest );
	
//	CPPUNIT_TEST( testAnalysisSynthesis );
//	CPPUNIT_TEST( foo );
	CPPUNIT_TEST( testAnalysisSynthesisInaNetwork );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
		// instantiate processings: TODO
		ConfigureAnalysisSynthesis();
	}

private:

	CLAM::SMSAnalysisCore mAnalysis;
	CLAM::SMSSynthesis mSynthesis;

	void ConfigureAnalysisSynthesis()
	{
		/*global parameters*/
		int analWindowSize = 2049;
		int resAnalWindowSize = 1025;

		int analHopSize = 256;
//		analHopSize= (resAnalWindowSize-1)/2 ;

		int synthFrameSize = analHopSize;
		int analZeroPaddingFactor= 2;
		
		// SMS Analysis configuration 
		CLAM::SMSAnalysisConfig analConfig;
		
		analConfig.SetSinWindowSize(analWindowSize);
		analConfig.SetHopSize(analHopSize);
//		analConfig.SetSinWindowType(mGlobalConfig.GetAnalysisWindowType());
		analConfig.SetSinZeroPadding(analZeroPaddingFactor);
		analConfig.SetResWindowSize(resAnalWindowSize);
//		analConfig.SetResWindowType(mGlobalConfig.GetResAnalysisWindowType());

//		analConfig.GetPeakDetect().SetMagThreshold(mGlobalConfig.GetAnalysisPeakDetectMagThreshold());
//		analConfig.GetPeakDetect().SetMaxFreq(mGlobalConfig.GetAnalysisPeakDetectMaxFreq());
//		analConfig.GetSinTracking().SetIsHarmonic(mGlobalConfig.GetAnalysisHarmonic());
//		analConfig.GetFundFreqDetect().SetReferenceFundFreq(mGlobalConfig.GetAnalysisReferenceFundFreq());
//		analConfig.GetFundFreqDetect().SetLowestFundFreq(mGlobalConfig.GetAnalysisLowestFundFreq());
//		analConfig.GetFundFreqDetect().SetHighestFundFreq(mGlobalConfig.GetAnalysisHighestFundFreq());
		
		CLAM::SMSSynthesisConfig synthConfig;
		synthConfig.SetAnalWindowSize(resAnalWindowSize);
		synthConfig.SetFrameSize(synthFrameSize);
		synthConfig.SetHopSize(synthFrameSize);

//		mAnalysis.Configure(analConfig);	
//		mSynthesis.Configure(synthConfig);
		
	}

	//TODO: fix. it runs but produces a broken sinusoidal
	void testAnalysisSynthesis() //no segment, no frame just streaming inner data
	{
		CLAM::ErrAssertionFailed::breakpointInCLAMAssertEnabled = true;

		int frameSize = 256;
		CLAM::MonoAudioFileReader audioProvider;
		CLAM::MonoAudioFileReaderConfig readercfg;
		CLAM::MonoAudioFileWriter audioWriter;
		CLAM::MonoAudioFileWriterConfig writercfg;
		CLAM::AudioFile file;
		file.OpenExisting(GetTestDataDirectory("sine.wav"));
		readercfg.SetSourceFile(file);
		audioProvider.GetOutPort("Samples Read").SetSize( frameSize );
		audioProvider.GetOutPort("Samples Read").SetHop(frameSize);
		
		const std::string storedResult(GetTestDataDirectory("SMSTests/out_analysis-synthesis-streaming_sine"));
		audioProvider.Configure( readercfg );
		CLAM::AudioFileHeader header;
		header.SetValues(44100, 1, "WAV");
		file.CreateNew(storedResult+"_result.wav", header);
		writercfg.SetTargetFile(file);
		audioWriter.Configure( writercfg );
		
//TODO new (alternative) interface:
//		CLAM::ConnectPorts(audioProvider, "Samples Read", mAnalysis, "Input Audio");

		audioProvider.GetOutPort("Samples Read").ConnectToIn( mAnalysis.GetInPort("Input Audio"));
		mAnalysis.GetOutPort("Sinusoidal Peaks").ConnectToIn( mSynthesis.GetInPort("InputSinPeaks"));
		mAnalysis.GetOutPort("Residual Spectrum").ConnectToIn( mSynthesis.GetInPort("InputResSpectrum"));
		mSynthesis.GetOutPort("OutputAudio").ConnectToIn( audioWriter.GetInPort("Samples Write") );

		audioProvider.Start();
		audioWriter.Start();
		mAnalysis.Start();
		mSynthesis.Start();
	
		// Processings firings
		CLAM_ASSERT(audioProvider.GetOutPort("Samples Read").CanProduce(), "mono audio file reader should have provided audio");
	
		while (audioProvider.Do())
		{
			mAnalysis.Do();
			if (mSynthesis.CanConsumeAndProduce())
			{
				mSynthesis.Do();
				audioWriter.Do();
			}
		}
		audioWriter.Stop();

		std::string whyDifferents;
		bool equals=helperCompareTwoAudioFiles(
				storedResult+".wav", storedResult+"_result.wav", 
				whyDifferents);
		CPPUNIT_ASSERT_MESSAGE(whyDifferents, equals);

					      
	}
	
	void foo()
	{
		CLAM::ErrAssertionFailed::breakpointInCLAMAssertEnabled = true;
		CLAM::MonoAudioFileWriter proc;
		CLAM::MonoAudioFileWriterConfig conf;
		CLAM::AudioFileHeader header;
		header.SetValues(44100, 1, "WAV");
		CLAM::AudioFile file;
		file.CreateNew("foo.wav", header);
		CPPUNIT_ASSERT_MESSAGE("header has channels", header.HasChannels() );	
		CPPUNIT_ASSERT_MESSAGE("file have channels", file.GetHeader().HasChannels() );	
		
		
		conf.SetTargetFile( file );
		CPPUNIT_ASSERT_MESSAGE("config have channels", conf.GetTargetFile().GetHeader().HasChannels() );	

		CLAM::MonoAudioFileWriterConfig configcopy;
		configcopy = conf;
		CPPUNIT_ASSERT_MESSAGE("config copy have channels", configcopy.GetTargetFile().GetHeader().HasChannels() );	
		proc.Configure(conf);
	}
	
	void testAnalysisSynthesisInaNetwork()
	{
		//CLAM::ErrAssertionFailed::breakpointInCLAMAssertEnabled = true;
		
		CLAM::Network net;
		const int audioFrameSize = 512; //!! test with different framesizes
		net.AddFlowControl( new CLAM::BasicFlowControl( audioFrameSize ) );
		net.AddProcessing( "AudioIn", new CLAM::MonoAudioFileReader );
		net.AddProcessing( "AudioOut",new CLAM::MonoAudioFileWriter );
		net.AddProcessing( "Analysis", new CLAM::SMSAnalysisCore );
		net.AddProcessing( "Synthesis", new CLAM::SMSSynthesis );
		net.ConnectPorts("AudioIn.Samples Read", "Analysis.Input Audio");
		net.ConnectPorts("Analysis.Sinusoidal Peaks", "Synthesis.InputResSpectrum");
		net.ConnectPorts("Synthesis.OutputAudio", "AudioOut.Samples Write");
		
//TODO refactor filenames		

		CLAM::MonoAudioFileReaderConfig audioInCfg;
		CLAM::AudioFile file;
		file.OpenExisting(GetTestDataDirectory("sine.wav"));
		audioInCfg.SetSourceFile(file);
		net.ConfigureProcessing("AudioIn", audioInCfg);
		
		CLAM::MonoAudioFileWriterConfig writercfg;		
		CLAM::AudioFileHeader header;
		header.AddChannels();
		header.UpdateData();
		header.SetValues(44100, 1, "WAV");
		file.CreateNew(GetTestDataDirectory("out_sms_net_stream_result.wav"), header);
		writercfg.SetTargetFile(file);
		CLAM::MonoAudioFileWriter foo;
		foo.Configure(writercfg);
//		net.GetProcessing("AudioOut").Configure(writercfg);
//		net.ConfigureProcessing("AudioOut", writercfg );
		
		net.Start();
//		for(int i=0; i<100; i++) net.DoProcessings();
		net.Stop();
/*
		std::string whyDifferents;
		bool equals=helperCompareTwoAudioFiles(
				storedResult+".wav", storedResult+"_result.wav", 
				whyDifferents);
		CPPUNIT_ASSERT_MESSAGE(whyDifferents, equals);
*/
		std::cout << "end of the test \n";
	}
};

} // namespace CLAMTest

