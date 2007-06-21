#include "SimpleOscillator.hxx"
#include "SMSAnalysisCore.hxx"
#include "SMSSynthesis.hxx"
#include "Audio.hxx"
#include "AudioOutPort.hxx"
#include "MonoAudioFileWriter.hxx"
#include "AudioDatabaseReader.hxx"
#include "Network.hxx"
#include "FreewheelingNetworkPlayer.hxx"

#define AUDIO_IN_FILE "/Users/greg/Music/samples/MyPatches/EBowGuitar/EBow_Guitar_A4_RS.aif"
//#define AUDIO_OUT_FILE "/Users/greg/Musics/samples/MyPatches/EBowGuitar/EBow_Guitar_A4_RS_comp.aif"
#define AUDIO_OUT_FILE "/Users/greg/Desktop/EBow_Guitar_A4_RS_comp.aif"

class TestStreamingSMSAnalysisSynthesis;

class TestStreamingSMSAnalysisSynthesis
{
public:
	
public:
	void setUp()
	{
		ConfigureAnalysisSynthesis();
	}

    CLAM::AudioDatabaseReader myAudioDatabaseReader;
    CLAM::SMSSynthesis mSynthesis;

	const int helperResAnalWindowSize() { return 1025; }
	const int helperAnalWindowSize() { return 2049; }
	const int helperAnalHopSize() {return 512;}

	void ConfigureAnalysisSynthesis()
	{
		/*global parameters*/
		int analWindowSize = 2049;
		int resAnalWindowSize = 1025;

		int analHopSize = 512;
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

        CLAM::AudioDatabaseReader::AudioDatabaseReaderConfig myAudioDatabaseReaderConfig;
        char* filename = "/Users/greg/Music/samples/MyPatches/EBowGuitar/EBow_Guitar_A4_RS.aif";
        myAudioDatabaseReaderConfig.SetFilename( filename );
		
		CLAM::SMSSynthesisConfig synthConfig;
		synthConfig.SetAnalWindowSize(resAnalWindowSize);
		synthConfig.SetFrameSize(synthFrameSize);
		synthConfig.SetHopSize(synthFrameSize);
		
	}

	//TODO: fix. it runs but produces a broken sinusoidal
	void testAnalysisSynthesis() //no segment, no frame just streaming inner data
	{
//		CLAM::ErrAssertionFailed::breakpointInCLAMAssertEnabled = true;

		int frameSize = 512;
		CLAM::MonoAudioFileWriter audioWriter;
		CLAM::MonoAudioFileWriterConfig writercfg;
		
		const std::string outputFile(AUDIO_OUT_FILE);
		writercfg.SetTargetFile(outputFile);
		audioWriter.Configure( writercfg );
		
        CLAM::AudioDatabaseReader myAudioDatabaseReader(  helperAudioDatabaseConfigInstance() );

		CLAM::SMSSynthesis synthesis;
		synthesis.Configure( helperSynthesisConfigInstance() );

		CLAM::ConnectPorts(myAudioDatabaseReader, "Sinusoidal Peaks", synthesis, "InputSinPeaks");
		CLAM::ConnectPorts(myAudioDatabaseReader, "Residual Spectrum", synthesis, "InputResSpectrum");
		CLAM::ConnectPorts(synthesis, "OutputAudio", audioWriter, "Samples Write" );

		myAudioDatabaseReader.Start();
		audioWriter.Start();
		synthesis.Start();

		while (myAudioDatabaseReader.Do())
		{
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
					      
	}
	
	// helper methods for the network tests
	const CLAM::SMSAnalysisConfig& helperAnalysisConfigInstance()
	{

		int analHopSize = 512;
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
    
    const CLAM::AudioDatabaseReader::AudioDatabaseReaderConfig & helperAudioDatabaseConfigInstance()
    {
        static CLAM::AudioDatabaseReader::AudioDatabaseReaderConfig myAudioDatabaseReaderConfig;
        char* filename = "/Users/greg/Music/samples/MyPatches/EBowGuitar/EBow_Guitar_A4_RS.aif";
        myAudioDatabaseReaderConfig.SetFilename( filename );
        return myAudioDatabaseReaderConfig;
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


};

int main(int argc,char** argv)
{
//	try
//	{

        TestStreamingSMSAnalysisSynthesis app;
		app.testAnalysisSynthesis();
        std::cout << "all done" << std::endl;
/*
	}
	catch(Err error)
	{
		error.Print();
		std::cerr << "Abnormal Program Termination" << std::endl;
		return -1;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}
*/    
	return 0;
}



