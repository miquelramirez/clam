
#include <CLAM/XMLStorage.hxx>
#include <CLAM/SMSSynthesis.hxx>
#include <CLAM/SMSSynthesisConfig.hxx>
#include <CLAM/SinusoidalSynthesis.hxx>
#include <CLAM/Audio.hxx>
#include "Processing.hxx"
#include "SDIFInConfig.hxx"
#include "SDIFFileReader.hxx"
#include "SDIFFileReaderWithBuffer.hxx"
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
// adapted from Greg Kellum's ContinuousExcitationSynthesizer

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

//	void SynthesizeToSpeakers(SDIFDatabase& sdifDatabase, SinusoidalSynthesis& synthesis, 
	void SynthesizeToSpeakers(SDIFDatabase& sdifDatabase, SMSSynthesis& synthesis, 
								ContinuousExcitationControlSource* aControlScore, MonoAudioFileWriter* audioWriter)
	{
        if (audioWriter == NULL)
            std::cout << "Playing audio to speakers." << std::endl;
        else
            std::cout << "Playing audio to speakers and writing audio to file." << std::endl;

		CLAM_ACTIVATE_FAST_ROUNDING;

	//	unsigned int buffersize = 1024;
		int samplerate = 44100;
 
		std::cout << "checkpoint 1." << std::endl;
 
        AudioManager audioManager(samplerate, 2048);
		AudioIOConfig outLCfg;
		outLCfg.SetDevice("rtaudio:default");
		outLCfg.SetChannelID(0);
		outLCfg.SetSampleRate(samplerate);

		std::cout << "checkpoint 2." << std::endl;

		AudioIOConfig outRCfg;
		outRCfg.SetDevice("rtaudio:default");
		outRCfg.SetChannelID(1);
		outRCfg.SetSampleRate(samplerate);

		std::cout << "checkpoint 3." << std::endl;

		CLAM::SMSFreqShift smsFreqShift;

		CLAM::AudioOut outL( outLCfg );
		CLAM::AudioOut outR( outRCfg );
		//CLAM::AudioSink out;

		std::cout << "checkpoint 4." << std::endl;

        audioManager.Start();

		std::cout << "checkpoint 5." << std::endl;

		smsFreqShift.Start();
		synthesis.Start();
		if (audioWriter != NULL)
		{
			audioWriter->Start();
		}

		std::cout << "checkpoint 6." << std::endl;

        outL.Start();
        outR.Start();
        
		ControlStreamSegmentator theSegmentator;

		std::cout << "checkpoint 7." << std::endl;

		int counter = 0;
		VoiceId theVoiceId;
		float thePitch;
		float theAmplitude;
		// make this true to have the given score file loop
		aControlScore->SetFeedForever(false);
		
		std::cout << "checkpoint 8." << std::endl;

		while ( aControlScore->GetControlValues(thePitch, theAmplitude) )
		{
			std::cout << std::endl << std::endl;
			std::cout << "GetControlValues. pitch: " << thePitch << ", amplitude: " << theAmplitude << std::endl;

			// TODO do you really need VoiceIds? Wouldn't it be enough to simply
			// tell the sdifDatabase to read from the attack? 
			bool isNewVoice = theSegmentator.IsNewVoice( thePitch, theAmplitude );
			if ( isNewVoice )
			{
				sdifDatabase.DeleteVoice(theVoiceId);
				theVoiceId = sdifDatabase.CreateVoice();
				std::cout << "Oraison: creating new voiceId " << theVoiceId << std::endl;
			}

			Frame* aFrame = sdifDatabase.SelectFrameForVoice(theVoiceId, thePitch, theAmplitude);

			std::cout << "counter: " << counter << ", pitch: " << thePitch << ", amplitude: " << theAmplitude << std::endl;

			if ( !synthesis.Do( *aFrame ) ) {
				std::cout << "!synthesis.Do( *aFrame )" <<std::endl;
				continue;
			}

			if (audioWriter != NULL)
			{
				audioWriter->Do( aFrame->GetSynthAudioFrame() );
			}
	/** Sinusoidal synthesized Audio */
//	DYN_ATTRIBUTE (10, public, Audio, SinusoidalAudioFrame);
	/** Residual synthesized Audio */
//	DYN_ATTRIBUTE (11, public, Audio, ResidualAudioFrame);

//            outL.Do( aFrame->GetSynthAudioFrame() );
//            outR.Do( aFrame->GetSynthAudioFrame() );

//            outL.Do( aFrame->GetResidualAudioFrame() );
//            outR.Do( aFrame->GetResidualAudioFrame() );

            outL.Do( aFrame->GetResidualAudioFrame() );
            outR.Do( aFrame->GetSinusoidalAudioFrame() );
            
			counter++;
		}
		
		std::cout << "finishing..." << std::endl;

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
	bool isSynthesizeSinusoidsAndResidual = true;  // used to be useless
	//	bool isResidualEnabled = false;
	bool isResidualEnabled = true;
	
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
	CLAM::SinusoidalSynthesis theSinSynthesis;
	CLAM::SMSSynthesis theSMSSynthesis;

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
		std::cout << "aControlScorePtr = new CLAM::ContinuousExcitationOSCSource(std::string("/ebowSynthesizer"));" << std::endl;
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
	
//	if( isSynthesizeSinusoidsAndResidual ) theSinSynthesis.Configure( SynthesisConfig ) else theSMSSynthesis.Configure( SynthesisConfig );
	theSMSSynthesis.Configure( SynthesisConfig );

	/****************************************\
	 * Load SDIF							*
	\****************************************/
//		CLAM::SDIFInConfig cfg;
//		cfg.SetEnableFundFreq(false);
//		cfg.SetEnablePeakArray(true);
//		cfg.SetEnablePeakArray(true);
	std::string directoryName = std::string(sdifDirectory);
	CLAM::SDIFDatabase theSDIFLoader;
	theSDIFLoader.LoadProgram( directoryName, isResidualEnabled );

	std::cout << "theSDIFLoader.LoadProgram()" << std::endl;

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
		SynthesizeToSpeakers( theSDIFLoader, theSMSSynthesis, aControlScorePtr, NULL );
	else
		SynthesizeToSpeakers( theSDIFLoader, theSMSSynthesis, aControlScorePtr, &audioWriter );

	delete aControlScorePtr;

	std::clog << "Finished successfully!\n";
	return 0;
}

