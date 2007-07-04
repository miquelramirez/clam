
#include "SMSIo.hxx"
#include "XMLStorage.hxx"
#include "SMSAnalysis.hxx"

/**
*  This example shows how to analyze a wave file into a representation consisting  
*  of the sinusoidal parts, the residual part, and the fundamental frequency
*  for each frame, and then store this analysis in an SDIF file.
*  
*  To generate an SDIF file to use with this example, see the example WavToSDIF.
*
*  To run the example, type ./WavToSDIFExe from the examples directory.
*  The application will print out a help message telling you which files it needs in which order.
*
*  \see {http://www.cnmat.berkeley.edu/SDIF/}
*  \see {http://www.mtg.upf.edu/sms/}
*
*  \author Giacomo Sommavilla 6/30/07
*  \since  CLAM v1.1.
*/
namespace CLAM
{
	void Analyze(SMSAnalysis& analysis, Segment& segment)
	{
		CLAM_ACTIVATE_FAST_ROUNDING;

		/** Cleans up segment from pre-existing data*/
		segment.SetFramesArray(List<Frame>());
		segment.SetChildren(List<Segment>());
		segment.mCurrentFrameIndex=0;

		analysis.Start();
		while(analysis.Do(segment));
		analysis.Stop();

		//remove first two frame
		for (int j = 0; j < 2; j++)
		{
			segment.DeleteFrame( 0 );
			segment.UpdateData();
		}

		CLAM_DEACTIVATE_FAST_ROUNDING;
	}

} //namespace CLAM

int main(int argc,char** argv)
{
	const char *wavinput, *sdifoutput, *xmlconfig;
	switch (argc)
	{
	case 2:
	case 3:
	case 4:
		wavinput=argv[1];
		sdifoutput=(argc < 3 )?"output.sdif":argv[2];
		xmlconfig=(argc < 4 )?"WavToSDIF/analysis.xml":argv[3];
		break;
	default:
		printf("Usage: %s input.wav [ output.sdif [ analysis.xml ] ]\n",argv[0]);
		exit(1);
		break;
	}
	try{
		/** Internal structure used for storing the result of the analysis, contains all data.
		 * @see Segment.hxx */
		CLAM::Segment AnalysisSegment;

		/** Analysis configuration */
		CLAM::SMSAnalysisConfig AnalConfig;

		/** Internal Processings used for analysis and synthesis */
		CLAM::SMSAnalysis Analysis;


		/****************************************\
		 * Config                               *
		\****************************************/
		// SMS Analysis configuration
		CLAM::XMLStorage::Restore(AnalConfig,xmlconfig);
		//if window size is even we add one !
		if (AnalConfig.GetSinWindowSize()%2==0) AnalConfig.SetSinWindowSize(AnalConfig.GetSinWindowSize()+1);
		if (AnalConfig.GetResWindowSize()%2==0) AnalConfig.SetResWindowSize(AnalConfig.GetResWindowSize()+1);
		if (AnalConfig.GetHopSize()<0) AnalConfig.SetHopSize((AnalConfig.GetResWindowSize()-1)/2 );


		/****************************************\
		 * Load Sound                           *
		\****************************************/
		AnalConfig.SetSamplingRate(WAVELoad(wavinput,AnalysisSegment));

		//XMLStorage::Dump(AnalConfig,"SMSAnalysisConfig",xmlconfig);

		Analysis.Configure(AnalConfig);
		Analyze(Analysis, AnalysisSegment);

		SDIFStore( sdifoutput, AnalysisSegment );
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

