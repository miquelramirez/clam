
#include <CLAM/XMLStorage.hxx>
#include <CLAM/SMSSynthesis.hxx>
#include <CLAM/SMSSynthesisConfig.hxx>
#include <CLAM/SinusoidalSynthesis.hxx>
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
#include "TestScore.hxx"
#include "ControlStreamSegmentator.hxx"

#ifdef WITH_OSC
#include "ContinuousExcitationOSCSource.hxx"
#endif

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

	void SynthesizeToSpeakers(SDIFDatabase& sdifDatabase, SinusoidalSynthesis& synthesis, 
								ContinuousExcitationControlSource* aControlScore, MonoAudioFileWriter* audioWriter)
	{
        if (audioWriter == NULL)
            std::cout << "Playing audio to speakers." << std::endl;
        else
            std::cout << "Playing audio to speakers and writing audio to file." << std::endl;

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
		if (audioWriter != NULL)
		{
			audioWriter->Start();
		}

        outL.Start();
        outR.Start();
        
		ControlStreamSegmentator theSegmentator;

		int counter = 0;
		VoiceId theVoiceId;
		float thePitch;
		float theAmplitude;
		// make this true to have the given score file loop
		aControlScore->SetFeedForever(false);
		while ( aControlScore->GetControlValues(thePitch, theAmplitude) )
		{
			//std::cout << "pitch: " << thePitch << ", amplitude: " << theAmplitude << std::endl;
			
			// TODO do you really need VoiceIds? Wouldn't it be enough to simply
			// tell the sdifDatabase to read from the attack? 
			bool isNewVoice = theSegmentator.IsNewVoice(thePitch, theAmplitude);
			if ( isNewVoice )
			{
				sdifDatabase.DeleteVoice(theVoiceId);
				theVoiceId = sdifDatabase.CreateVoice();
//				std::cout << "Oraison: creating new voice" << std::endl;
			}
			
			Frame* aFrame = sdifDatabase.SelectFrameForVoice(theVoiceId, thePitch, theAmplitude);

			//std::cout << "counter: " << counter << ", pitch: " << thePitch << ", amplitude: " << theAmplitude << std::endl;
			bool isSynthesizeSinusoidsAndResidual = false;
			if ( !synthesis.Do( *aFrame ) )
                continue;

			if (audioWriter != NULL)
			{
				audioWriter->Do( aFrame->GetSynthAudioFrame() );
			}

            outL.Do( aFrame->GetSynthAudioFrame() );
            outR.Do( aFrame->GetSynthAudioFrame() );
            
			counter++;
		}

		sdifDatabase.DeleteVoice(theVoiceId);

		if (audioWriter != NULL)
		{
			audioWriter->Stop();
		}

        outL.Stop();
        outR.Stop();

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
		xmlconfig=(argc < 5 )?"synthesis.xml":argv[4];
		isPlayOnSpeakers = (strcmp(wavoutput, "speakers") == 0) ? 1 : 0;
		break;
	default:
		printf("\n");
		printf("Usage: %s inputSDIFDirectory [(osc | inputScore) [ output.wav [ synthesis.xml ] ] ]\n",argv[0]);
		printf("\n");
		printf("    If osc is given, the application will expect an osc stream\n");
		printf("    with this message format: /ebowSynthesizer pitch amplitude\n");
		printf("    to be sent to port 7000. Pitch should be in Hertz and amplitude\n");
		printf("    should be from 0-1.\n");
		printf("\n");
		printf("    If an input score is given, the application will expect the\n");
		printf("    score to have the same format as the files in testData, i.e.\n");
		printf("    pitch amplitude\n");
		printf("    pitch amplitude ... \n");
		printf("\n");
		printf("    One synthesis frame will be calculated for each OSC message.\n");
		printf("    or each line from a score file. So, given a hopsize of 256,\n");
		printf("    you'll need to generate about 172 control events per second.\n");
		printf("\n");
		exit(1);
		break;
	}

	/** Analysis configuration */
	CLAM::SMSSynthesisConfig SynthesisConfig;

	/** Internal Processings used for analysis and synthesis */
	CLAM::SinusoidalSynthesis Synthesis;

	CLAM::ContinuousExcitationControlSource* aControlScorePtr;
	if ( strcmp(scoreFile, "osc") != 0 )
	{
		CLAM::TestScore* aTestScorePtr = new CLAM::TestScore();
		bool result = aTestScorePtr->LoadScore(scoreFile);
		aControlScorePtr = aTestScorePtr;
	}
	else
	{
#ifdef WITH_OSC
		aControlScorePtr = new CLAM::ContinuousExcitationOSCSource(std::string("/ebowSynthesizer"));
#else
		std::cout << "OSC is not enabled. Loading dummy score rather than activating OSC." << std::endl;
		aControlScorePtr = new CLAM::TestScore();
#endif
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
	bool isResidualEnabled = false;
	theSDIFLoader.LoadProgram( directoryName, isResidualEnabled );

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
	if ( strcmp(wavoutput, "speakers") == 0 )
		SynthesizeToSpeakers(theSDIFLoader, Synthesis, aControlScorePtr, NULL);
	else
		SynthesizeToSpeakers(theSDIFLoader, Synthesis, aControlScorePtr, &audioWriter);

	delete aControlScorePtr;

	std::clog << "Finished successfully!\n";
	return 0;
}

