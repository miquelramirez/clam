#include "LoopingSDIFFileReader.hxx"
#include "DataUtil.hxx"
#include <time.h>
#include <vector>

namespace CLAM
{
	
LoopingSDIFFileReader::LoopingSDIFFileReader(const SDIFInConfig& argSDIFInConfig)
:	BufferedSDIFFileReader(argSDIFInConfig), 
	numberOfCrossfadeFrames(DEFAULT_NUMBER_OF_CROSSFADE_FRAMES),
	indexOfCurrentLoop(0)
{
	outgoingFrame = new Frame();
	outgoingFrame->AddSpectralPeakArray();
	outgoingFrame->AddResidualSpec();
	outgoingFrame->AddFundamental();
	outgoingFrame->AddSynthAudioFrame();
	outgoingFrame->UpdateData();
	
	Configure(argSDIFInConfig);
}

LoopingSDIFFileReader::~LoopingSDIFFileReader()
{
	delete outgoingFrame;
}

bool LoopingSDIFFileReader::Configure(const SDIFInConfig& c)
{
	/* initialize random seed: */
	srand ( time(NULL) );
	
	return true;
}

bool LoopingSDIFFileReader::isLooping()
{
	return listOfLoops.size() != 0;
}

void LoopingSDIFFileReader::AddLoop(SimpleLoopMetadata& aSimpleLoop)
{
	listOfLoops.push_back(aSimpleLoop);
}

void LoopingSDIFFileReader::ClearLoops()
{
	listOfLoops.clear();
}

std::vector<SimpleLoopMetadata>& LoopingSDIFFileReader::GetListOfLoops()
{
	return listOfLoops;
}

void LoopingSDIFFileReader::SetListOfLoops(std::vector<SimpleLoopMetadata>& argListOfLoops)
{
	listOfLoops.clear();
	std::vector<SimpleLoopMetadata>::iterator theIterator;
	for( theIterator = argListOfLoops.begin(); theIterator != argListOfLoops.end(); theIterator++)
	{
		SimpleLoopMetadata& aSimpleLoop = *theIterator;
		listOfLoops.push_back(aSimpleLoop);
		
		//std::cout << "LoopingSDIFFileReader: received loop point <" << aSimpleLoop.GetStart();
		//std::cout << ">, <" << aSimpleLoop.GetEnd() << ">" << std::endl;
	}
}

Frame* LoopingSDIFFileReader::ReadFrame()
{
	int lFrameBufferPosition = BufferedSDIFFileReader::GetFrameBufferPosition();
	SimpleLoopMetadata& simpleLoop = listOfLoops.at(indexOfCurrentLoop);
	
	// check to see whether we are at the loop point. if so, push the framePosition
	// back to the start of the loop
	if (lFrameBufferPosition == simpleLoop.GetEnd())
	{
		lFrameBufferPosition = simpleLoop.GetStart();
		BufferedSDIFFileReader::SetFrameBufferPosition(lFrameBufferPosition);
		indexOfCurrentLoop = ChooseLoopRandomly(lFrameBufferPosition, indexOfCurrentLoop);
		
		//SimpleLoopMetadata& tempSimpleLoop = listOfLoops.at(indexOfCurrentLoop);
		//std::cout << "LoopingSDIFFIleREader: switching to loop with start: " << tempSimpleLoop.GetStart();
		//std::cout << " and end: " << tempSimpleLoop.GetEnd() << std::endl;
	}

	// get the current frame from the BufferedSDIFFileReader
	Frame* nextFrame = BufferedSDIFFileReader::ReadFrame();

	// we will later modify the frame, but we don't want the frame object
	// being cached by the BufferedSDIFFIleReader to be modified
	// so let's copy it.
	DataUtil::CopyFrameMembers(*nextFrame, *outgoingFrame);

	// Check to see whether we are near the loop point. If so, we'll have
	// to crossfade this frame with the frame at the other end of the loop.
	if ( (lFrameBufferPosition + numberOfCrossfadeFrames) >= simpleLoop.GetEnd() )
	{
		Frame* startLoopFrame = BufferedSDIFFileReader::GetFrame( simpleLoop.GetStart() );
		Frame* frameNearEndLoopFrame = nextFrame;
		
		// if we are crossfading 10 frames and we are currently on frame 3, the 
		// crossfade factor is 3/10
		float crossfadeFactor = ( (lFrameBufferPosition + numberOfCrossfadeFrames) 
								- simpleLoop.GetEnd() * 1.0 ) /
								numberOfCrossfadeFrames;
								
		CrossfadeSpectralPeakArrays(frameNearEndLoopFrame->GetSpectralPeakArray(), 
									startLoopFrame->GetSpectralPeakArray(),
									outgoingFrame->GetSpectralPeakArray(),
									crossfadeFactor);
		
		CrossfadeResidualSpectrum(frameNearEndLoopFrame->GetResidualSpec(), 
									startLoopFrame->GetResidualSpec(),
									outgoingFrame->GetResidualSpec(),
									crossfadeFactor);
	}
	
	return outgoingFrame;
}

int LoopingSDIFFileReader::ChooseLoopRandomly(int argFrameBufferPosition, int argIndexOfCurrentLoop)
{
	// suppose that the current frame number is 1413. If we have a list of possible
	// loops whose start and end points are (580, 1163), (850, 2100), and (982, 2310)
	// we can only choose the second two loops, because the end point of the first
	// is before the current frame position.
	// but if we've only loaded 2200 frames into memory, then actually only the middle
	// loop is valid.
	// so here we look for valid loops that pass these two tests.
	std::deque<int> validLoopPositions;
	std::deque<int>::iterator currentLoopIterator = validLoopPositions.end();
	for (unsigned int counter = 0; counter < listOfLoops.size(); counter++)
	{
		SimpleLoopMetadata& simpleLoop = listOfLoops.at(indexOfCurrentLoop);
		
		if ( argFrameBufferPosition < simpleLoop.GetEnd() 
			&& BufferedSDIFFileReader::GetNumberOfFramesLoaded() >= simpleLoop.GetEnd() )
		{
			validLoopPositions.push_back(counter);
			if (counter == argIndexOfCurrentLoop)
			{
				currentLoopIterator = validLoopPositions.end();
				currentLoopIterator--;
			}
		}
	}
	
	// if we have a couple of loops to choose from, don't allow the algorithm to
	// choose the loop we are already in. 
	if (validLoopPositions.size() > 1 && currentLoopIterator != validLoopPositions.end())
	{
		validLoopPositions.erase(currentLoopIterator);
	}
	
	double randomNumber0To1 = (rand()*1.0) / RAND_MAX;
	int randomIndex = floor( randomNumber0To1 * validLoopPositions.size() );
	
	return validLoopPositions.at(randomIndex);
}

void LoopingSDIFFileReader::CrossfadeSpectralPeakArrays(SpectralPeakArray& sourceSpectralPeaks1,
														SpectralPeakArray& sourceSpectralPeaks2,
														SpectralPeakArray& targetSpectralPeaks,
														float crossfadeFactor)
{
	// todo why does this sound bad?
	//sourceSpectralPeaks1.TodB();
	//sourceSpectralPeaks2.TodB();
	//targetSpectralPeaks.SetScale(EScale::eLog);
	
	DataArray& srcFreqBuffer1 = sourceSpectralPeaks1.GetFreqBuffer();
	DataArray& srcMagBuffer1 = sourceSpectralPeaks1.GetMagBuffer();

	DataArray& srcFreqBuffer2 = sourceSpectralPeaks2.GetFreqBuffer();
	DataArray& srcMagBuffer2 = sourceSpectralPeaks2.GetMagBuffer();

	DataArray& targetFreqBuffer = targetSpectralPeaks.GetFreqBuffer();
	DataArray& targetMagBuffer = targetSpectralPeaks.GetMagBuffer();

	float flippedCrossfadeFactor = 1.0 - crossfadeFactor;
	int numberOfPeaks1 = sourceSpectralPeaks1.GetnPeaks();
	int numberOfPeaks2 = sourceSpectralPeaks2.GetnPeaks();
	int srcCounter1 = 0;
	int srcCounter2 = 0;
	int targetCounter = 0;
	float semitoneDown = 0.944;
	float semitoneUp = 1.059;
	while ( srcCounter1 < numberOfPeaks1 || srcCounter2 < numberOfPeaks2 )
	{
		float peakDifference = srcFreqBuffer1[srcCounter1] / srcFreqBuffer2[srcCounter2];
		
		// if the peaks are less than a semitone apart, everything is fine. crossfade them.
		if ( peakDifference > semitoneDown && peakDifference < semitoneUp )
		{
			// get frequency , mag and phase
			targetFreqBuffer[targetCounter] = (srcFreqBuffer1[srcCounter1] * flippedCrossfadeFactor)
									+ (srcFreqBuffer2[srcCounter2] * crossfadeFactor);
			targetMagBuffer[targetCounter] = (srcMagBuffer1[srcCounter1] * flippedCrossfadeFactor)
									+ (srcMagBuffer2[srcCounter2] * crossfadeFactor);
			/*
			if (targetCounter == 0)
			{
				printf("mag 1: %f, mag 2: %f, result: %f \n", srcMagBuffer1[srcCounter1], srcMagBuffer2[srcCounter2], targetMagBuffer[targetCounter]);
				printf("scale is linear? %i \n", (sourceSpectralPeaks1.GetScale()==EScale::eLinear));
			}
			*/
			targetCounter++;
			if (srcCounter1 < numberOfPeaks1)
				srcCounter1++;
			if (srcCounter2 < numberOfPeaks2)
				srcCounter2++;
		}
		// the first frequency is a over a semitone below the second. let's fade the
		// first freqency out and skip on to the next frequency
		else if (peakDifference < semitoneDown)
		{
			if (srcCounter1 < numberOfPeaks1)
				srcCounter1++;

			targetFreqBuffer[targetCounter] = (srcFreqBuffer1[srcCounter1] * flippedCrossfadeFactor);
			targetMagBuffer[targetCounter] = (srcMagBuffer1[srcCounter1] * flippedCrossfadeFactor);
		}
		// the first frequency is a over a semitone above the second. let's fade 
		// the second frequency in and skip on to the second frequency
		else
		{
			if (srcCounter2 < numberOfPeaks2)
				srcCounter2++;

			targetFreqBuffer[targetCounter] = (srcFreqBuffer2[srcCounter2] * crossfadeFactor);
			targetMagBuffer[targetCounter] = (srcMagBuffer2[srcCounter2] * crossfadeFactor);
		}
	}

	//targetSpectralPeaks.ToLinear();
}


void LoopingSDIFFileReader::CrossfadeResidualSpectrum(Spectrum& sourceSpectrum1,
								Spectrum& sourceSpectrum2,
								Spectrum& targetSpectrum,
								float crossfadeFactor)
{
	int size = sourceSpectrum1.GetSize();
	targetSpectrum.SetSize( size );

	Array<Complex>& srcComplexBuffer1 = sourceSpectrum1.GetComplexArray();
	Array<Complex>& srcComplexBuffer2 = sourceSpectrum2.GetComplexArray();
	Array<Complex>& targetComplexBuffer = targetSpectrum.GetComplexArray();

	float flippedCrossfadeFactor = 1.0 - crossfadeFactor;
	for (int r=0; r < size; r++)
	{
		// TODO figure out how to implement this using complex numbers
		targetComplexBuffer[r] = (srcComplexBuffer1[r] * flippedCrossfadeFactor)
									+ (srcComplexBuffer2[r] * crossfadeFactor);
	}
}

void LoopingSDIFFileReader::Reset()
{
	BufferedSDIFFileReader::SetFrameBufferPosition( 0 );
}

} // end namespace CLAM

