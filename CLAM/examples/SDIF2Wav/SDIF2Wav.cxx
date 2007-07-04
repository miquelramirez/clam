
#include "SMSIo.hxx"
#include "XMLStorage.hxx"
#include "SMSSynthesis.hxx"
#include "SMSSynthesisConfig.hxx"
#include "Audio.hxx"
#include "Processing.hxx"

/**
*  This example shows how to resynthesize a WAVE file from an SDIF file.
*  This example preloads the entire SDIF file into a Segment object
*  and then resynthesizes the Wave file from this Segment. See the
*  example SDIFToWavStreaming for an implementation that streams from
*  the file.
*
*  To run the example, type ./SDIFToWavExe from the examples directory.
*  The application will print out a help message telling you which files it needs in which order.
*
*  \see {http://www.cnmat.berkeley.edu/SDIF/}
*  \see {http://www.mtg.upf.edu/sms/}
*
*  \author greg kellum [gkellum@iua.upf.edu] 6/30/07
*  \since  CLAM v1.1.
*/
namespace CLAM
{
	void Synthesize(SMSSynthesis& synthesis, Segment& segment, Audio& audioOutput)
	{
		CLAM_ACTIVATE_FAST_ROUNDING;

		CLAM::SMSSynthesisConfig theConfig = dynamic_cast<const SMSSynthesisConfig&>(synthesis.GetConfig());
		TSize synthFrameSize = theConfig.GetFrameSize();
		int nFrames = segment.GetnFrames();
		TSize sizeAudioFile = synthFrameSize * nFrames;
		TIndex beginIndex = -synthFrameSize/2;

		audioOutput.SetSize(sizeAudioFile);

		synthesis.Start();
		for( int i=0; i<nFrames; i++ )
		{
			if( !synthesis.Do( segment ))
				continue; // it is an analysis frame with negative center time and thus should not be used

			audioOutput.SetAudioChunk(	beginIndex,
										segment.GetFrame(i).GetSynthAudioFrame());

			beginIndex+=synthFrameSize;
		}
		synthesis.Stop();

		CLAM_DEACTIVATE_FAST_ROUNDING;
	}

} //namespace CLAM

int main(int argc,char** argv)
{
	const char *sdifinput, *wavoutput, *xmlconfig;
	switch (argc)
	{
	case 2:
	case 3:
	case 4:
		sdifinput=argv[1];
		wavoutput=(argc < 3 ) ? "output.wav" : argv[2];
		xmlconfig=(argc < 4 ) ? "SDIF2Wav/synthesis.xml" : argv[3];
		break;
	default:
		printf("Usage: %s input.sdif [ output.wav [ synthesis.xml ] ]\n",argv[0]);
		exit(1);
		break;
	}

	try {
		/** Internal structure used for storing the result of the analysis, contains all data.
		 * @see Segment.hxx */
		CLAM::Segment AnalysisSegment;

		/** Analysis configuration */
		CLAM::SMSSynthesisConfig SynthesisConfig;

		/** Internal Processings used for analysis and synthesis */
		CLAM::SMSSynthesis Synthesis;


		/****************************************\
		 * Config							   *
		\****************************************/
		// SMS Analysis configuration
		CLAM::XMLStorage::Restore(SynthesisConfig, xmlconfig);
		// if you'd like to use your own configuration, either edit the XML file
		// or create a SynthesisConfig object by hand and dump its contents
		// to file for later use
		//XMLStorage::Dump(AnalConfig,"SMSAnalysisConfig",xmlconfig);

		/****************************************\
		 * Load SDIF							*
		\****************************************/
		SDIFLoad(sdifinput, AnalysisSegment);

		CLAM::Audio AudioOutput;
		Synthesis.Configure(SynthesisConfig);
		Synthesize(Synthesis, AnalysisSegment, AudioOutput);

		WAVEStore( wavoutput, AudioOutput );
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

	std::clog << "Finished successfully!\n";
	return 0;
}

