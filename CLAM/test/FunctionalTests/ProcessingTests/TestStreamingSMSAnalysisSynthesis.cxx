
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

class TestStreamingSMSAnalysisSynthesis;

CPPUNIT_TEST_SUITE_REGISTRATION( TestStreamingSMSAnalysisSynthesis );

class TestStreamingSMSAnalysisSynthesis : public CppUnit::TestFixture
{
public:
	
	CPPUNIT_TEST_SUITE( TestStreamingSMSAnalysisSynthesis );
	
	CPPUNIT_TEST( testAnalysisSynthesis );
	CPPUNIT_TEST( testAnalysisSynthesisInaNetwork );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
		ConfigureAnalysisSynthesis();
	}

private:

	CLAM::SMSAnalysisCore mAnalysis;
	CLAM::SMSSynthesis mSynthesis;

	const int helperResAnalWindowSize() { return 1025; }
	const int helperAnalWindowSize() { return 2049; }
	const int helperAnalHopSize() {return 256;}

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
//		CLAM::ErrAssertionFailed::breakpointInCLAMAssertEnabled = true;

		int frameSize = 512;
		CLAM::MonoAudioFileReader audioProvider;
		CLAM::MonoAudioFileReaderConfig readercfg;
		CLAM::MonoAudioFileWriter audioWriter;
		CLAM::MonoAudioFileWriterConfig writercfg;
		CLAM::AudioFileSource file;
		file.OpenExisting(GetTestDataDirectory("sine.wav"));
		readercfg.SetSourceFile(file);
		audioProvider.GetOutPort("Samples Read").SetSize( frameSize );
		audioProvider.GetOutPort("Samples Read").SetHop(frameSize);
		
		const std::string storedResult(GetTestDataDirectory("SMSTests/out_analysis-synthesis-streaming_sine"));
		audioProvider.Configure( readercfg );
		CLAM::AudioFileHeader header;
		header.SetValues(44100, 1, "WAV");
		CLAM::AudioFileTarget fileOut;
		fileOut.CreateNew(storedResult+"_result.wav", header);
		writercfg.SetTargetFile(fileOut);
		audioWriter.Configure( writercfg );
		
		CLAM::SMSAnalysisCore analysis;
		CLAM::SMSSynthesis synthesis;
		analysis.Configure( helperAnalysisConfigInstance() );
		synthesis.Configure( helperSynthesisConfigInstance() );
		
		CLAM::ConnectPorts(audioProvider, "Samples Read", mAnalysis, "Input Audio");
		CLAM::ConnectPorts(mAnalysis, "Sinusoidal Peaks", synthesis, "InputSinPeaks");
		CLAM::ConnectPorts(mAnalysis, "Residual Spectrum", synthesis, "InputResSpectrum");
		CLAM::ConnectPorts(synthesis, "OutputAudio", audioWriter, "Samples Write" );

		audioProvider.Start();
		audioWriter.Start();
		mAnalysis.Start();
		synthesis.Start();
	
		// Processings firings
		CLAM_ASSERT(audioProvider.GetOutPort("Samples Read").CanProduce(), "mono audio file reader should have provided audio");
	
		while (audioProvider.Do())
		{
			mAnalysis.Do();
			if (synthesis.CanConsumeAndProduce())
			{
				synthesis.Do();
			}
			if (audioWriter.CanConsumeAndProduce())
			{
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
	
	// helper methods for the network tests
	const CLAM::SMSAnalysisConfig& helperAnalysisConfigInstance()
	{

		int analHopSize = 256;
//		analHopSize= (resAnalWindowSize-1)/2 ;

//		int synthFrameSize = analHopSize;
		int analZeroPaddingFactor= 2;
		
		// SMS Analysis configuration 
		static CLAM::SMSAnalysisConfig analConfig;
		
		analConfig.SetSinWindowSize(helperAnalWindowSize() );
		analConfig.SetHopSize(analHopSize);
//		analConfig.SetSinWindowType(mGlobalConfig.GetAnalysisWindowType());
		analConfig.SetSinZeroPadding(analZeroPaddingFactor);
		analConfig.SetResWindowSize( helperResAnalWindowSize() );
//		analConfig.SetResWindowType(mGlobalConfig.GetResAnalysisWindowType());

//		analConfig.GetPeakDetect().SetMagThreshold(mGlobalConfig.GetAnalysisPeakDetectMagThreshold());
//		analConfig.GetPeakDetect().SetMaxFreq(mGlobalConfig.GetAnalysisPeakDetectMaxFreq());
//		analConfig.GetSinTracking().SetIsHarmonic(mGlobalConfig.GetAnalysisHarmonic());
//		analConfig.GetFundFreqDetect().SetReferenceFundFreq(mGlobalConfig.GetAnalysisReferenceFundFreq());
//		analConfig.GetFundFreqDetect().SetLowestFundFreq(mGlobalConfig.GetAnalysisLowestFundFreq());
//		analConfig.GetFundFreqDetect().SetHighestFundFreq(mGlobalConfig.GetAnalysisHighestFundFreq());

		return analConfig;
	}
	const CLAM::SMSSynthesisConfig & helperSynthesisConfigInstance()
	{
		static CLAM::SMSSynthesisConfig synthConfig;
		int synthFrameSize = helperAnalHopSize();
		synthConfig.SetAnalWindowSize( helperResAnalWindowSize() );
		synthConfig.SetFrameSize(synthFrameSize);
		synthConfig.SetHopSize(synthFrameSize);
		return synthConfig;
	}

	
	void testAnalysisSynthesisInaNetwork()
	{
		//CLAM::ErrAssertionFailed::breakpointInCLAMAssertEnabled = true;
		
		CLAM::Network net;
		const int audioFrameSize = 256; //!! test with different framesizes
		net.AddProcessing( "AudioIn", new CLAM::MonoAudioFileReader );
		net.AddProcessing( "AudioOut",new CLAM::MonoAudioFileWriter );
		net.AddProcessing( "Analysis", new CLAM::SMSAnalysisCore );
		net.AddProcessing( "Synthesis", new CLAM::SMSSynthesis );

		net.ConnectPorts("AudioIn.Samples Read", "Analysis.Input Audio");
		net.ConnectPorts("Analysis.Sinusoidal Peaks", "Synthesis.InputSinPeaks");
		net.ConnectPorts("Analysis.Residual Spectrum", "Synthesis.InputResSpectrum");
		net.ConnectPorts("Synthesis.OutputAudio", "AudioOut.Samples Write");
		

		CLAM::MonoAudioFileReaderConfig audioInCfg;
		CLAM::AudioFileSource file;
		file.OpenExisting(GetTestDataDirectory("Elvis.wav"));
		audioInCfg.SetSourceFile(file);

		net.ConfigureProcessing("AudioIn", audioInCfg);
		
		CLAM::MonoAudioFileWriterConfig writercfg;
		CLAM::AudioFileTarget fileOut;		
		CLAM::AudioFileHeader header;
		header.SetValues(44100, 1, "WAV");
		std::string storedResult=GetTestDataDirectory("SMSTests/out_sms_net_stream");
		fileOut.CreateNew( storedResult+"_result.wav", header);
		writercfg.SetTargetFile(fileOut);
		
		net.ConfigureProcessing("AudioOut", writercfg );
		net.ConfigureProcessing("Analysis", helperAnalysisConfigInstance() );
		net.ConfigureProcessing("Synthesis", helperSynthesisConfigInstance() );

		
		net.Start();
		//TODO network should control the overall execution
		for(int i=0; i<100; i++) net.Do();
		net.Stop();

		std::string  whyDifferents;
		bool equals=helperCompareTwoAudioFiles(
				storedResult+".wav", storedResult+"_result.wav", 
				whyDifferents);
		CPPUNIT_ASSERT_MESSAGE(whyDifferents, equals);

		std::cout << "end of the test \n";
	}
};

} // namespace CLAMTest

