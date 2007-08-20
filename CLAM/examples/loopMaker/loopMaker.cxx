
#include "SMSIo.hxx"
#include "XMLStorage.hxx"
#include "SMSAnalysis.hxx"
#include "LoopPointExtractor.hxx"
#include "Segment.hxx"
#include "Frame.hxx"
#include "SampleMetadata.hxx"
#include "SimpleLoopMetadata.hxx"
#include "Filename.hxx"
#include "Audio.hxx"
#include "DataTypes.hxx"
#include <vector>
#include <algorithm>

/**
*  This example when given a directory full of sound files converts these files
*  to SDIF format and saves extracted information about their pitch, amplitude, and
*  optimal loop points to XML metadata files.
*
*  To run the example, type ./loopMakerExe from the examples directory.
*  The application will print out a help message telling you which files it needs in which order.
*
*  \see {http://www.cnmat.berkeley.edu/SDIF/}
*  \see {http://www.mtg.upf.edu/sms/}
*
*  \author Greg Kellum [gkellum@iua.upf.edu] 7/1/2007
*  \since  CLAM v1.1
*/

static int millisecondsToFrames(double milliseconds)
{
	return milliseconds / 1000. * 44100 / 256.;
}

static int framesToMilliseconds(int frames)
{
	return frames * 256. / 44100. * 1000.;
}

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
	
	double SelectBestF0(Segment& segment)
	{
		int nFrames = segment.GetnFrames();
		double minError = 100000.;
		double f0;
		for (int counter = 0; counter < nFrames; counter++)
		{
			Fundamental& aFundamental = segment.GetFrame(counter).GetFundamental();
			aFundamental.SortByError();
			DataArray candidateF0s = aFundamental.GetCandidatesFreq();
			DataArray candidateErrors = aFundamental.GetCandidatesErr();
			int arraySize = candidateF0s.Size();
			for (int inner_counter = 0; inner_counter < arraySize; inner_counter++)
			{
				if ( fabs(candidateErrors[inner_counter]) < minError )
				{
					minError = fabs(candidateErrors[inner_counter]);
					f0 = fabs(candidateF0s[inner_counter]);
				}
			}
		}
		return f0;
	}
	
	bool ordering_function(const TData& a, const TData& b) 
	{ 
		return fabs(a) < fabs(b); 
	}
	
	double CalculateAmplitude(Segment& segment)
	{
		Audio& allAudio = segment.GetAudio();
		int numberOfSamples = allAudio.GetSize();
		int hopSize = 512;
		double maxAmplitude = 0.;
		DataArray& aBuffer = allAudio.GetBuffer();
		for (int counter = 0; (counter+hopSize) < numberOfSamples; counter += hopSize)
		{
			int beginSample = counter;
			int endSample = counter + hopSize;
			double meanAmplitudeOfWindow = 0.;
			std::vector<TData> v( aBuffer.GetPtr() + beginSample, aBuffer.GetPtr() + endSample );
			nth_element(v.begin(), v.begin()+(hopSize-5), v.end(), ordering_function);
			std::vector<TData>::iterator i;
			for(i = v.begin()+(hopSize-5); i != v.end(); i++)
			{
				meanAmplitudeOfWindow += fabs(*i);
			}
			std::cout << std::endl;
			
			meanAmplitudeOfWindow = meanAmplitudeOfWindow / 5;
			
			if ( maxAmplitude < meanAmplitudeOfWindow)
			{
				maxAmplitude = meanAmplitudeOfWindow;
			}
		}

		return maxAmplitude;
	}

} //namespace CLAM

int main(int argc,char** argv)
{
	bool isPlayOnSpeakers = false;
	const char *wavinput;
	double startTime, endTime;
	switch (argc)
	{
	case 2:
	case 3:
	case 4:
	case 5:
		wavinput = argv[1];
		startTime = (argc < 3 ) ? 0 : atof(argv[2]);
		endTime = (argc < 4 ) ?  (44100*100) : atof(argv[3]);
		break;
	default:
		printf("Usage: loopMaker inputWavFile [startTime [endTime] ]\n");
		printf("\n");
		printf("       This application converts a wav file into an SDIF file..\n");
		printf("       and saves metadata about the file in an XML file.\n");
		printf("\n");
		printf("       Start time gives the msec when the steady state begins.\n");
		printf("       End time gives the msec when the steady state ends,\n");
		printf("       and sets the limit of where loops should be looked for.\n");
		printf("\n");
		printf("       The input file should be a wav file with a 44100 sample rate.\n");
		printf("       Start and end times should be given in milliseconds.\n");
		exit(1);
		break;
	}

	char* xmlconfig = "loopMaker/analysis.xml";
	// we create the filename for the SDIF file
	std::string strwavinput(wavinput);
	const char* sdifoutput = strwavinput.replace(strwavinput.find_last_of("wav")-2,4,"sdif").c_str();
	// we create the filename for the XML metadata file
	std::string strwavinput2(wavinput);
	const char* xmloutput = strwavinput2.replace(strwavinput2.find_last_of("wav")-2,3,"xml").c_str();

	try {
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

		CLAM::SampleMetadata theSampleMetadata;
		std::string sdifoutputstr(sdifoutput);
		CLAM::Filename sdifFilename(sdifoutputstr);
		theSampleMetadata.SetFilename( sdifFilename );
		
		double bestF0 = SelectBestF0( AnalysisSegment );
		std::cout << "Best f0: " << bestF0 << std::endl;
		theSampleMetadata.SetPitch( bestF0 );

		double amplitude = CalculateAmplitude(AnalysisSegment);
		std::cout << "Maximum window amplitude: " << amplitude << std::endl;
		theSampleMetadata.SetAmplitude( amplitude );
		
		theSampleMetadata.SetStartSteadyState( startTime );

		// here we convert from milliseconds to frame number 
		int startFrame = millisecondsToFrames(startTime);
		int endFrame;
		if (endTime != -1)
			endFrame = millisecondsToFrames(endTime);

		// now we analyze the frames to find a pair with the smallest earth
		// mover's distance
		CLAM::List<CLAM::Frame>& framesArray = AnalysisSegment.GetFramesArray();
		CLAM::LoopPointExtractor loopExtractor;
		std::map<int, int> optimalLoopPoints;
		loopExtractor.extractOptimalLoopPoints(framesArray, optimalLoopPoints, startFrame, endFrame);

		std::vector<CLAM::SimpleLoopMetadata>& listOfLoops = theSampleMetadata.GetListOfLoops();
		std::map<int,int>::iterator iter;   
		for( iter = optimalLoopPoints.begin(); iter != optimalLoopPoints.end(); iter++ ) 
		{
			CLAM::SimpleLoopMetadata aSimpleLoop;
			aSimpleLoop.SetStart( framesToMilliseconds( iter->first ) );
			aSimpleLoop.SetEnd( framesToMilliseconds( iter->second ) );
			listOfLoops.push_back( aSimpleLoop );
		}

		CLAM::XMLStorage::Dump(theSampleMetadata,"SampleMetadata",xmloutput);
		
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

