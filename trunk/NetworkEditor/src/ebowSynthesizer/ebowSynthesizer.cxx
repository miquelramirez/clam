
#include <CLAM/XMLStorage.hxx>
#include <CLAM/SMSSynthesis.hxx>
#include <CLAM/SMSSynthesisConfig.hxx>
#include <CLAM/Audio.hxx>
#include "Processing.hxx"
#include "SDIFInConfig.hxx"
#include "SDIFFileReader.hxx"
#include "BufferedSDIFFileReader.hxx"
#include "SDIFDatabase.hxx"
#include "MonoAudioFileWriter.hxx"
#include "MonoAudioFileWriterConfig.hxx"
#include "Spectrum.hxx"
#include "AudioManager.hxx"
#include "AudioIO.hxx"
#include "AudioOut.hxx"
#include "AudioSink.hxx"
#include "TopLevelProcessing.hxx"
#include "SMSFreqShift.hxx"
#include "ContinuousExcitationOSCSource.hxx"
#include "TestScore.hxx"
#include "ContinuousExcitationOSCSource.hxx"

/**
*  This example shows off an implementation of a synthesizer that uses a database
*  of SDIF files to perform real-time Spectral Modeling Synthesis.
*
*  To generate an SDIF file to use with this example, see the example WavToSDIF.
*
*  To run the example, type ./SDIFToWavStreamingExe from the examples directory.
*  The application will print out a help message telling you which files it needs in which order.
*
*  \see {http://www.cnmat.berkeley.edu/SDIF/}
*  \see {http://www.mtg.upf.edu/sms/}
*
*  \author Greg Kellum [gkellum@iua.upf.edu] 7/15/2007
*  \since  CLAM v1.1
*/
namespace CLAM
{
	/*
	void Synthesize(SDIFFileReader& sdifDatabase, SMSSynthesis& synthesis, MonoAudioFileWriter& audioWriter)
	{
		std::cout << "Writing audio to file." << std::endl;

		CLAM_ACTIVATE_FAST_ROUNDING;

		CLAM::SMSSynthesisConfig theConfig = dynamic_cast<const SMSSynthesisConfig&>(synthesis.GetConfig());
		TSize synthFrameSize = theConfig.GetFrameSize();

		// is this necessary?
		//audioOutput.SetSize(sizeAudioFile);

		synthesis.Start();
		audioWriter.Start();
		int counter;
		Frame* aFrame;
		while ( (aFrame = sdifDatabase.ReadFrame()) != NULL )
		{
			synthesis.Do( *aFrame );

			audioWriter.Do( aFrame->GetSynthAudioFrame() );

			counter++;
		}
		std::cout << "Wrote " << counter << " frames of audio to file." << std::endl;

		audioWriter.Stop();
		synthesis.Stop();

		CLAM_DEACTIVATE_FAST_ROUNDING;
	}
	*/

	void SynthesizeToSpeakers(SDIFDatabase& sdifDatabase, SMSSynthesis& synthesis, ContinuousExcitationControlSource& aControlScore)
	{
		std::cout << "Playing audio to speakers." << std::endl;

		CLAM_ACTIVATE_FAST_ROUNDING;

		unsigned int buffersize = 1024;
		int samplerate = 44100;

		AudioManager audioManager(samplerate,2048);

		AudioIOConfig outLCfg;
		outLCfg.SetDevice("rtaudio:default");
		outLCfg.SetChannelID(0);
		outLCfg.SetSampleRate(samplerate);

		AudioIOConfig outRCfg;
		outRCfg.SetDevice("rtaudio:default");
		outRCfg.SetChannelID(1);
		outRCfg.SetSampleRate(samplerate);

		CLAM::SMSFreqShift smsFreqShift;
		CLAM::AudioOut outL(outLCfg);
		CLAM::AudioOut outR(outRCfg);
		//CLAM::AudioSink out;

		audioManager.Start();
		smsFreqShift.Start();
		synthesis.Start();
		//out.Start();
		outL.Start();
		outR.Start();

		CLAM::VoiceId theVoiceId = sdifDatabase.CreateVoice();

		int counter = 0;
		float thePitch;
		float theAmplitude;
		while ( aControlScore.GetControlValues(thePitch, theAmplitude) )
		{
			//std::cout << "pitch: " << thePitch << ", amplitude: " << theAmplitude << std::endl;

			Frame* aFrame = sdifDatabase.SelectFrameForVoice(theVoiceId, thePitch, theAmplitude);

			synthesis.Do( *aFrame );

			outL.Do( aFrame->GetSynthAudioFrame() );
			outR.Do( aFrame->GetSynthAudioFrame() );

			counter++;
		}

		sdifDatabase.DeleteVoice(theVoiceId);

		outL.Stop();
		outR.Stop();
		//out.Stop();
		synthesis.Stop();
		smsFreqShift.Stop();

		CLAM_DEACTIVATE_FAST_ROUNDING;

		std::cout << "Computed audio for " << counter << " frames." << std::endl;
	}

} //namespace CLAM

int main(int argc,char** argv)
{
	bool isPlayOnSpeakers = false;
	const char *sdifDirectory, *scoreFile, *wavoutput, *xmlconfig;
	switch (argc)
	{
	case 2:
	case 3:
	case 4:
	case 5:
		sdifDirectory=argv[1];
		scoreFile=(argc < 3 )?"osc":argv[2];
		wavoutput=(argc < 4 )?"speakers":argv[3];
		xmlconfig=(argc < 5 )?"ebowSynthesizer/synthesis.xml":argv[4];
		isPlayOnSpeakers = (strcmp(wavoutput, "speakers") == 0) ? 1 : 0;
		break;
	default:
		printf("Usage: %s inputSDIFDirectory [inputScore [ output.wav [ synthesis.xml ] ] ]\n",argv[0]);
		printf("\n");
		exit(1);
		break;
	}

	/** Analysis configuration */
	CLAM::SMSSynthesisConfig SynthesisConfig;

	/** Internal Processings used for analysis and synthesis */
	CLAM::SMSSynthesis Synthesis;

	CLAM::ContinuousExcitationControlSource* aControlScorePtr;
	if ( strcmp(scoreFile, "osc") != 0 )
	{
		CLAM::TestScore* aTestScorePtr = new CLAM::TestScore();
		bool result = aTestScorePtr->LoadScore(scoreFile);
		aControlScorePtr = aTestScorePtr;
	}
	else
	{
		aControlScorePtr = new CLAM::ContinuousExcitationOSCSource(std::string("/ebowSynthesizer"));
	}

	/****************************************\
	 * Configure Synthesis				  *
	\****************************************/
	// SMS Analysis configuration
	CLAM::XMLStorage::Restore(SynthesisConfig, xmlconfig);
	// if you'd like to use your own configuration, either edit the XML file
	// or create a SynthesisConfig object by hand and dump its contents
	// to file for later use
	//XMLStorage::Dump(AnalConfig,"SMSAnalysisConfig",xmlconfig);
	Synthesis.Configure(SynthesisConfig);

	/****************************************\
	 * Load SDIF							*
	\****************************************/
//		CLAM::SDIFInConfig cfg;
//		cfg.SetEnableFundFreq(false);
//		cfg.SetEnablePeakArray(true);
//		cfg.SetEnablePeakArray(true);
	std::string directoryName = std::string(sdifDirectory);
	CLAM::SDIFDatabase theSDIFLoader;
	theSDIFLoader.LoadProgram( directoryName );

	//CLAM::Thread threadPtr;
	//SDIFLoader.LoadFramesIntoBufferOnThread( &threadPtr );

	/****************************************\
	 * Configure AudioFileWriter			*
	\****************************************/
	CLAM::MonoAudioFileWriter audioWriter;
	CLAM::MonoAudioFileWriterConfig writercfg;

	const std::string outputFile(wavoutput);
	writercfg.SetTargetFile(outputFile);
	audioWriter.Configure( writercfg );

	/****************************************\
	 * Let's now synthesize the audio	   *
	\****************************************/
	SynthesizeToSpeakers(theSDIFLoader, Synthesis, *aControlScorePtr);

//		else
//			Synthesize(SDIFLoader, Synthesis, audioWriter);

	delete aControlScorePtr;

	std::clog << "Finished successfully!\n";
	return 0;
}

