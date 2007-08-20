
#include "LoopPointExtractor.hxx"

namespace CLAM
{
float dist(feature_t *F1, feature_t *F2)
{
	if (*F1 == 0 || *F2 == 0) return 10.0;
	
	return fabs( (*F1-*F2) / *F2) * 100.0;
}

LoopPointExtractor::LoopPointExtractor()
 :	minimumStartAndEndFrameSeparation( DEFAULT_MINIMUM_FRAME_SEPARATION ),
	frameHopSize( DEFAULT_FRAME_HOPSIZE )
{
}

void LoopPointExtractor::extractOptimalLoopPoints(CLAM::List<CLAM::Frame>& framesArray,
													std::map<int,int>& loopPoints, 
													int startFrame, int endFrame)
{
	std::vector<CLAM::DistanceMeasure> emdVector;

	calculateEarthMoverDistances(framesArray, emdVector, startFrame, endFrame);

	std::vector<CLAM::DistanceMeasure> bestLoopsVector;

	selectBestLoopPoints(emdVector, bestLoopsVector);
	
	convertToMap(bestLoopsVector, loopPoints);
}

void LoopPointExtractor::convertToMap(std::vector<CLAM::DistanceMeasure>& bestLoopsVector, std::map<int,int>& loopPoints)
{
	for (int counter = 0; counter < bestLoopsVector.size(); counter++)
	{
		DistanceMeasure pDistanceMeasure = bestLoopsVector.at(counter);
		
		int frameA = pDistanceMeasure.getPositionA();
		int frameB = pDistanceMeasure.getPositionB();

		loopPoints[frameA] = frameB;
	}
}

void LoopPointExtractor::scaleWeights(TData* weights1Ptr, TData* scaledWeights1Ptr, int arraySize)
{
	double totals = 0.0;
	for (int x = 0; x < arraySize; x++)
		totals += weights1Ptr[x];
	double scaleFactor = 1.0 / totals;
	for (int x = 0; x < arraySize; x++)
		scaledWeights1Ptr[x] = scaleFactor * weights1Ptr[x];
}

void LoopPointExtractor::generateEqualWeights(TData* scaledWeights1Ptr, int arraySize)
{
	double weight = 1.0 / arraySize;
	for (int x = 0; x < arraySize; x++)
		scaledWeights1Ptr[x] = weight;
}

signature_t LoopPointExtractor::generateSignatureWithMagnitudes(SpectralPeakArray& spectralPeakArray)
{
	//TData* featuresPtr = spectralPeakArray.GetFreqBuffer().GetPtr();
	//TData* weightsPtr = spectralPeakArray.GetMagBuffer().GetPtr();
	TData* featuresPtr = spectralPeakArray.GetMagBuffer().GetPtr();
	TData scaledWeightsPtr[ spectralPeakArray.GetnPeaks() ];
	//scaleWeights(weightsPtr, scaledWeightsPtr, spectralPeakArray.GetnPeaks());
	int size = (spectralPeakArray.GetnPeaks() > 100) ? 100 : spectralPeakArray.GetnPeaks();
	generateEqualWeights(scaledWeightsPtr, size);
	signature_t theSignature = { size, featuresPtr, scaledWeightsPtr };
	
	return theSignature;
}

signature_t LoopPointExtractor::generateSignatureWithFrequenciesScaledByMagnitudes(SpectralPeakArray& spectralPeakArray)
{
	TData* featuresPtr = spectralPeakArray.GetFreqBuffer().GetPtr();
	TData* weightsPtr = spectralPeakArray.GetMagBuffer().GetPtr();
	TData scaledWeightsPtr[ spectralPeakArray.GetnPeaks() ];
	int size = (spectralPeakArray.GetnPeaks() > 100) ? 100 : spectralPeakArray.GetnPeaks();
	scaleWeights(weightsPtr, scaledWeightsPtr, size);
	signature_t theSignature = { size, featuresPtr, scaledWeightsPtr };
	
	return theSignature;
}

void LoopPointExtractor::calculateEarthMoverDistances(CLAM::List<CLAM::Frame>& framesArray, 
														std::vector<CLAM::DistanceMeasure>& emdVector,
														int startFrame, int endFrame)
{		
	std::cout << "loopMaker: calculating earth movers distances." << std::endl;
	
	int numberOfFrames = (endFrame == -1) ? framesArray.Size() : endFrame;
	
	for (int outer_counter = startFrame; outer_counter < numberOfFrames; outer_counter += frameHopSize)
	{
		for (int inner_counter = outer_counter + minimumStartAndEndFrameSeparation; inner_counter < numberOfFrames; inner_counter += frameHopSize)
		{
			SpectralPeakArray& spectralPeakArray1 = framesArray[outer_counter].GetSpectralPeakArray();
			signature_t s1 = generateSignatureWithFrequenciesScaledByMagnitudes(spectralPeakArray1);
	
			SpectralPeakArray& spectralPeakArray2 = framesArray[inner_counter].GetSpectralPeakArray();
			signature_t s2 = generateSignatureWithFrequenciesScaledByMagnitudes(spectralPeakArray2);
	
			DistanceMeasure aDistanceMeasure;
			aDistanceMeasure.setDistance( emd(&s1, &s2, dist, NULL, NULL) );
			aDistanceMeasure.setPositionA( outer_counter );
			aDistanceMeasure.setPositionB( inner_counter );
			emdVector.push_back( aDistanceMeasure );

			if (emdVector.size() % 1000 == 0)
				std::cout << "." << std::flush;
		}
		
		//std::cout << "Compared frame " << outer_counter << " to frame " << inner_counter << ". Distance: " << emdVector[outer_counter] << std::endl;
		//printf("Compared frame %i to frame %i. Found a distance of %21.20f\n", outer_counter, inner_counter, emdVector[outer_counter]);
	}

	std::cout << std::endl;
	std::cout << "Calculated " << emdVector.size() << " earth mover distances." << std::endl;

	int numberOfMinima = 10;
	TData minimaArray[ numberOfMinima ];
	int startingPoint = 120;
	//findMinima(emdVector, minimaArray, numberOfFrames, numberOfMinima, startingPoint);
	//nth_element(emdVector.begin(), emdVector.begin()+10, emdVector.end());
	sort(emdVector.begin(), emdVector.end());

	std::cout << "Sorted the array of earth mover distances." << std::endl;

	CLAM_DEACTIVATE_FAST_ROUNDING;
}

bool LoopPointExtractor::areStartAndEndFramesCloseTogether(int posA, int posB)
{
	// this ensures that the loop is at least a second long because with a 
	// hopzise of 256 samples, ceil(44100 / 256) = 173 frames
	int distanceBetweenStartAndEndFrames = abs(posA - posB);
	
	return (distanceBetweenStartAndEndFrames < minimumStartAndEndFrameSeparation);
}

bool LoopPointExtractor::areStartAndEndFramesCloseTogether(DistanceMeasure& pDistanceMeasure)
{
	return areStartAndEndFramesCloseTogether(pDistanceMeasure.getPositionA(), pDistanceMeasure.getPositionB());
}

bool LoopPointExtractor::isCloseToFrameAlreadyReported(DistanceMeasure& currentDistanceMeasure, std::vector<CLAM::DistanceMeasure>& reportedDistanceVector)
{
	int minimumFrameSeparation = 20;
	int oneSecondFrameSeparation = 173;
	for (int counter = 0; counter < reportedDistanceVector.size(); counter++)
	{
		DistanceMeasure& previousDistanceMeasure = reportedDistanceVector.at(counter);

		int changeInFrameAPosition = abs(currentDistanceMeasure.getPositionA() 
											- previousDistanceMeasure.getPositionA());
		int changeInFrameBPosition = abs(currentDistanceMeasure.getPositionB() 
											- previousDistanceMeasure.getPositionB());
		
		// if neither the start nor the end of the loop has changed by a lot,
		// this loop is too close to one already reported
		if (changeInFrameAPosition < minimumFrameSeparation 
				&& changeInFrameBPosition < minimumFrameSeparation)
			return true;

		// if the start of the loop hasn't changed much and the end of the 
		// loop hasn't changed by a lot, this loop is too close to one already
		// reported
		if (changeInFrameAPosition < minimumFrameSeparation
				&& changeInFrameBPosition < oneSecondFrameSeparation)
			return true;
			
		// if the end of the loop hasn't changed much and the start of the 
		// loop hasn't changed by a lot, this loop is too close to one already
		// reported
		if (changeInFrameBPosition < minimumFrameSeparation
				&& changeInFrameAPosition < oneSecondFrameSeparation)
			return true;
	}
	
	return false;
}

void LoopPointExtractor::selectBestLoopPoints(std::vector<CLAM::DistanceMeasure>& emdVector,
												std::vector<CLAM::DistanceMeasure>& bestLoopsVector)
{
	int numberOfStartAndEndFramesTooClose = 0;
	int numberOfLoopsTooCloseToReportedLoop = 0;
	
	for (int x = 0; x < emdVector.size(); x++)
	{
		DistanceMeasure& pDistanceMeasure = emdVector.at(x);
		
		int previousFrameAPosition;
		int previousFrameBPosition;
		bool isFirstIteration = true;

		if ( areStartAndEndFramesCloseTogether(pDistanceMeasure) )
		{
			numberOfStartAndEndFramesTooClose++;
			continue;
		}
		
		if ( isCloseToFrameAlreadyReported(pDistanceMeasure, bestLoopsVector) )
		{
			numberOfLoopsTooCloseToReportedLoop++;
			continue;
		}
				
		/*
		int frameA = startFrame + pDistanceMeasure.getPositionA();
		int frameB = startFrame + pDistanceMeasure.getPositionB();

		std::cout << "loopMaker: distance measurement: " << pDistanceMeasure.GetDistance();
		std::cout << ", position a: " << frameA << " (" << framesToMilliseconds(frameA) << " msecs)";
		std::cout << ", position b: " << frameB << " (" << framesToMilliseconds(frameB) << " msecs)";
		std::cout << ", counter: " << bestLoopsVector.size() << std::endl;
		*/
		
		bestLoopsVector.push_back(pDistanceMeasure);
		
		/*
		if (pDistanceMeasure.GetDistance() < 1)
			std::cout << "Distance measurement: " << pDistanceMeasure.GetDistance() << ", position: " << pDistanceMeasure.getPosition() << std::endl;
		else
			break;
		*/
		
		if (bestLoopsVector.size() > 100)
			break;
	}
	
	/*
	std::cout << "loopMaker: " << numberOfStartAndEndFramesTooClose;
	std::cout << " loops had start and frames that were too close together." << std::endl;
	std::cout << "loopMaker: " << numberOfLoopsTooCloseToReportedLoop;
	std::cout << " loops were too close to previously reported loops." << std::endl;
	*/
}


} //namespace CLAM


