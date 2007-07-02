
#include <CLAM/XMLStorage.hxx>
#include <CLAM/SMSSynthesis.hxx>
#include <CLAM/SMSSynthesisConfig.hxx>
#include <CLAM/Audio.hxx>
#include "Processing.hxx"
#include "SDIFInConfig.hxx"
#include "SDIFStreamingSource.hxx"
#include "MonoAudioFileWriter.hxx"
#include "MonoAudioFileWriterConfig.hxx"
#include "Spectrum.hxx"
#include "AudioManager.hxx"
#include "AudioIO.hxx"
#include "AudioOut.hxx"
#include "TopLevelProcessing.hxx"

/**
*  This example shows how to stream frames from an SDIF to a spectral modeling resynthesis algorithm
*  in order to resynthesize the analyzed waveform in real-time. However, if you were going to use  
*  something like this to build a synthesizer, you would probably want to preload some of the frames
*  into a cache to prevent glitches in the audio that occur when the SDIF file cannot be read fast 
*  enough from disk.
*
*  To generate an SDIF file to use with this example, see the example WavToSDIF.
*
*  To run the example, type ./SDIFToWavStreamingExe from the examples directory.
*  The application will print out a help message telling you which files it needs in which order.
*
*  \see {http://www.cnmat.berkeley.edu/SDIF/}
*  \see {http://www.mtg.upf.edu/sms/}
*
*  \author Greg Kellum [gkellum@iua.upf.edu] 7/1/2007
*  \since  CLAM v1.1
*/
namespace CLAM
{
	void Synthesize(SDIFStreamingSource& sdifLoader, SMSSynthesis& synthesis, MonoAudioFileWriter& audioWriter)
	{
		std::cout << "Writing audio to file." << std::endl;

		CLAM_ACTIVATE_FAST_ROUNDING;

		CLAM::SMSSynthesisConfig theConfig = dynamic_cast<const SMSSynthesisConfig&>(synthesis.GetConfig());
		TSize synthFrameSize = theConfig.GetFrameSize();
		
		// is this necessary?
		//audioOutput.SetSize(sizeAudioFile);		
		
		sdifLoader.GetOutPort("Sinusoidal Peaks").ConnectToIn( synthesis.GetInPort("InputSinPeaks"));
		sdifLoader.GetOutPort("Residual Spectrum").ConnectToIn( synthesis.GetInPort("InputResSpectrum"));
		synthesis.GetOutPort("OutputAudio").ConnectToIn( audioWriter.GetInPort("Samples Write"));
				
		sdifLoader.Start();
		synthesis.Start();
		audioWriter.Start();
		int counter;
		while ( sdifLoader.Do() == true )
		{			
			if (synthesis.CanConsumeAndProduce())
			{
				synthesis.Do();
			}
			if (audioWriter.CanConsumeAndProduce())
			{
				audioWriter.Do();
			}
			
			counter++;
		}
		std::cout << "Wrote " << counter << " frames of audio to file." << std::endl;

		audioWriter.Stop();
		synthesis.Stop();
		sdifLoader.Stop();

		CLAM_DEACTIVATE_FAST_ROUNDING;
	}
	
	void SynthesizeToSpeakers(SDIFStreamingSource& sdifLoader, SMSSynthesis& synthesis)
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
		
		CLAM::AudioOut outL(outLCfg);
		CLAM::AudioOut outR(outRCfg);

		CLAM::ConnectPorts(sdifLoader, "Sinusoidal Peaks", synthesis, "InputSinPeaks" );
		CLAM::ConnectPorts(sdifLoader, "Residual Spectrum", synthesis, "InputResSpectrum" );					   
		CLAM::ConnectPorts(synthesis, "OutputAudio", outL, "Audio Input" );
		CLAM::ConnectPorts(synthesis, "OutputAudio", outR, "Audio Input" );

		audioManager.Start();
		sdifLoader.Start();
		synthesis.Start();
		outL.Start();
		outR.Start();

		int counter = 0;		
		while ( sdifLoader.Do() == true )
		{			
			if (synthesis.CanConsumeAndProduce())
			{
				synthesis.Do();
			}
			if (outL.CanConsumeAndProduce())
			{
				outL.Do();
			}
			if (outR.CanConsumeAndProduce())
			{
				outR.Do();
			}
			
			counter++;
		}
		
		outL.Stop();
		outR.Stop();		
		synthesis.Stop();
		sdifLoader.Stop();

		CLAM_DEACTIVATE_FAST_ROUNDING;

		std::cout << "Computed audio for " << counter << " frames." << std::endl;		
	}

} //namespace CLAM

int main(int argc,char** argv)
{
	bool isPlayOnSpeakers = false;
	const char *sdifinput, *wavoutput, *xmlconfig;
	switch (argc)
	{
	case 2:
	case 3:
	case 4:
		sdifinput=argv[1];
		wavoutput=(argc < 3 )?"speakers":argv[2];
		xmlconfig=(argc < 4 )?"SDIFToWavStreaming/synthesis.xml":argv[3];
		isPlayOnSpeakers = (strcmp(wavoutput, "speakers") == 0) ? 1 : 0;
		break;
	default:
		printf("Usage: %s input.sdif [ output.wav [ synthesis.xml ] ]\n",argv[0]);
		printf("\n");
		printf("  If no output file is given, the output is played to the speakers.\n");
		printf("  If you would like the output played to the speakers, but would\n");
		printf("  still like to specify a synthesis configuration file other than\n");
		printf("  the default file, synthesis.xml, type 'speakers' instead of \n");
		printf("  giving an input file.\n");
		exit(1);
		break;
	}
	
//	try {

		/** Analysis configuration */
		CLAM::SMSSynthesisConfig SynthesisConfig;

		/** Internal Processings used for analysis and synthesis */
		CLAM::SMSSynthesis Synthesis;


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
		CLAM::SDIFInConfig cfg;
		cfg.SetEnableFundFreq(false);
		cfg.SetEnablePeakArray(true);
		cfg.SetEnablePeakArray(true);
		CLAM::SDIFStreamingSource SDIFLoader;

		cfg.SetFileName( sdifinput );

		if ( !SDIFLoader.Configure( cfg ) )
		{
			// If SDIF file loading file fails we abort the execution
			std::cerr << "ERROR: SDIF file load aborted" << std::endl;
			return false;
		}

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
		if (isPlayOnSpeakers)
			SynthesizeToSpeakers(SDIFLoader, Synthesis);
		else
			Synthesize(SDIFLoader, Synthesis, audioWriter);
/*
	}
	catch(CLAM::Err error)
	{
		error.Print();
		std::cerr << "Abnormal Program Termination!\n" << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
*/
	std::clog << "Finished successfully!\n";
	return 0;
}

