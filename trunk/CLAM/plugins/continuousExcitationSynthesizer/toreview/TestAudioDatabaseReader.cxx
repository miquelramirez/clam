#include "SimpleOscillator.hxx"
#include "SMSAnalysisCore.hxx"
#include "SMSSynthesis.hxx"
#include "Audio.hxx"
#include "AudioOutPort.hxx"
#include "MonoAudioFileWriter.hxx"
#include "AudioDatabaseReader.hxx"
#include "Network.hxx"
#include "FreewheelingNetworkPlayer.hxx"
#include "XMLStorage.hxx"
#include "Err.hxx"

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
	}

    CLAM::AudioDatabaseReader myAudioDatabaseReader;
    CLAM::SMSSynthesis mSynthesis;

	//TODO: fix. it runs but produces a broken sinusoidal
	void runAnalysisSynthesis(const char* wavInput, const char* wavOutput, const char* xmlAnalysisConfig, const char* xmlSynthesisConfig)
	{
//		CLAM::ErrAssertionFailed::breakpointInCLAMAssertEnabled = true;

		int frameSize = 512;
		CLAM::MonoAudioFileWriter audioWriter;
		CLAM::MonoAudioFileWriterConfig writercfg;
		
		const std::string outputFile(wavOutput);
		writercfg.SetTargetFile(outputFile);
		audioWriter.Configure( writercfg );
		
        CLAM::AudioDatabaseReader myAudioDatabaseReader;
        CLAM::AudioDatabaseReader::AudioDatabaseReaderConfig myAudioDatabaseReaderConfig;
        myAudioDatabaseReaderConfig.SetFilename(wavInput);
        CLAM::SMSAnalysisConfig analysisConfig;
		CLAM::XMLStorage::Restore(analysisConfig, xmlAnalysisConfig);
        myAudioDatabaseReaderConfig.SetAnalysisConfig(analysisConfig);
        myAudioDatabaseReader.Configure(myAudioDatabaseReaderConfig);
        
		CLAM::SMSSynthesis synthesis;
		//synthesis.Configure( helperSynthesisConfigInstance() );
        CLAM::SMSSynthesisConfig synthesisConfig;
		CLAM::XMLStorage::Restore(synthesisConfig, xmlSynthesisConfig);
		synthesis.Configure(synthesisConfig);

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
    
};

int main(int argc,char** argv)
{
	try
	{
        const char *wavInput, *wavOutput, *xmlAnalysisConfig, *xmlSynthesisConfig;
        switch (argc)
        {
        case 2:
        case 3:
        case 4:
        case 5:
            wavInput=argv[1];
            wavOutput=(argc < 3 )?"output.wav":argv[2];
            xmlAnalysisConfig=(argc < 4 )?"analysis.xml":argv[3];
            xmlSynthesisConfig=(argc < 5 )?"synthesis.xml":argv[4];
            break;
        default:
            printf("Usage: %s input.wav [ output.wav [ analysis.xml [ synthesis.xml ] ] ]\n",argv[0]);
            exit(1);
            break;
        }

        TestStreamingSMSAnalysisSynthesis app;
		app.runAnalysisSynthesis( wavInput, wavOutput, xmlAnalysisConfig, xmlSynthesisConfig);
        std::cout << "all done" << std::endl;
	}
	catch(CLAM::Err error)
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

	return 0;
}



