
#include <CLAM/XMLStorage.hxx>
#include <CLAM/SMSSynthesis.hxx>
#include <CLAM/SMSSynthesisConfig.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/List.hxx>
#include "Processing.hxx"
#include "SDIFInConfig.hxx"
#include "SDIFFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
#include "MonoAudioFileWriterConfig.hxx"
#include "Spectrum.hxx"
#include "AudioManager.hxx"
#include "AudioIO.hxx"
#include "AudioOut.hxx"
#include "TopLevelProcessing.hxx"
#include "DataTypes.hxx"
#include <math.h>
#include "emd.hxx"
#include <algorithm>
#include <float.h>

/**
*  This class extracts the best loop points from a collection of 
*  analysis frames using the earth movers distance. This algorithm
*  calculates the distances between two sets of spectral peak arrays
*  by calculating the cost of tranforming one into the other. The
*  actual code for calculating this distance comes from one of
*  the original authors of the algorithm, Y. Rubner.
*
*  \see http://ai.stanford.edu/~rubner/emd/default.htm  
*
*  \author Greg Kellum [gkellum@iua.upf.edu] 7/1/2007
*  \since  CLAM v1.1
*/

namespace CLAM
{

// this ensures that the loop is at least a second long because with a 
// hopzise of 256 samples, ceil(44100 / 256) = 173 frames
#define DEFAULT_MINIMUM_FRAME_SEPARATION 173
// it's computationally too expensive to compare every frame for files larger
// than a few seconds. so, use this parameter to set how many frames you would
// like to hop over between each emd evaluation.
#define DEFAULT_FRAME_HOPSIZE 10

class DistanceMeasure
{
public:
	bool operator<(const DistanceMeasure& argDistanceMeasure) const
	{
		return getDistance() < argDistanceMeasure.getDistance();
	}
	bool operator>(const DistanceMeasure& argDistanceMeasure) const
	{
		return getDistance() > argDistanceMeasure.getDistance();
	}
	bool operator==(const DistanceMeasure& argDistanceMeasure) const
	{
		return getDistance() == argDistanceMeasure.getDistance();
	}
	int getPositionA() const { return positionA; }
	void setPositionA(int argPosition) { positionA = argPosition; }
	int getPositionB() const { return positionB; }
	void setPositionB(int argPosition) { positionB = argPosition; }
	float getDistance() const { return distance; }
	void setDistance(float argDistance) { distance = argDistance; }
private:
	int positionA;
	int positionB;
	float distance;
};

class LoopPointExtractor
{
public:
	LoopPointExtractor();
	
	void extractOptimalLoopPoints(CLAM::List<CLAM::Frame>& framesArray,
									std::map<int,int>& loopPoints,
									int startFrame=0, int endFrame=-1);

private:

	int millisecondsToFrames(double milliseconds)
	{
		return milliseconds / 1000. * 44100 / 256.;
	}
	
	int framesToMilliseconds(int frames)
	{
		return frames * 256. / 44100. * 1000.;
	}


	void scaleWeights(TData* weights1Ptr, TData* scaledWeights1Ptr, int arraySize);

	void generateEqualWeights(TData* scaledWeights1Ptr, int arraySize);

	signature_t generateSignatureWithMagnitudes(SpectralPeakArray& spectralPeakArray);
	
	signature_t generateSignatureWithFrequenciesScaledByMagnitudes(SpectralPeakArray& spectralPeakArray);
	
	void calculateEarthMoverDistances(CLAM::List<CLAM::Frame>& framesArray, std::vector<CLAM::DistanceMeasure>& emdVector, int startFrame=0, int endFrame=-1);

	bool areStartAndEndFramesCloseTogether(int startFrameNumber, int endFrameNumber);

	bool areStartAndEndFramesCloseTogether(DistanceMeasure& pDistanceMeasure);
	
	bool isCloseToFrameAlreadyReported(DistanceMeasure& currentDistanceMeasure, std::vector<CLAM::DistanceMeasure>& reportedDistanceVector);
	
	void selectBestLoopPoints(std::vector<CLAM::DistanceMeasure>& emdVector, std::vector<CLAM::DistanceMeasure>& bestLoopsVector);

	void convertToMap(std::vector<CLAM::DistanceMeasure>& bestLoopsVector, std::map<int,int>& loopPoints);

	std::vector<CLAM::DistanceMeasure> emdVector;
	int minimumStartAndEndFrameSeparation;
	int frameHopSize;

};

} //namespace CLAM

