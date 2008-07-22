#include "SDIFFileReaderWithLoop.hxx"
#include "DataUtil.hxx"
#include <time.h>
#include <vector>

#include <CLAM/SpecTypeFlags.hxx>
#include "SpectrumConfig.hxx"
#include "Spectrum.hxx"
#include "SpectrumConversions.hxx"

namespace CLAM
{
	
SDIFFileReaderWithLoop::SDIFFileReaderWithLoop(const SDIFInConfig& argSDIFInConfig)
:	SDIFFileReaderWithBuffer(argSDIFInConfig), 
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

SDIFFileReaderWithLoop::~SDIFFileReaderWithLoop()
{
	delete outgoingFrame;
}

bool SDIFFileReaderWithLoop::Configure(const SDIFInConfig& c)
{
	/* initialize random seed: */
	srand ( time(NULL) );
	
	return true;
}

bool SDIFFileReaderWithLoop::isLooping()
{
	return listOfLoops.size() != 0;
}

void SDIFFileReaderWithLoop::AddLoop(SimpleLoopMetadata& aSimpleLoop)
{
	listOfLoops.push_back(aSimpleLoop);
}

void SDIFFileReaderWithLoop::ClearLoops()
{
	listOfLoops.clear();
}

std::vector<SimpleLoopMetadata>& SDIFFileReaderWithLoop::GetListOfLoops()
{
	return listOfLoops;
}

void SDIFFileReaderWithLoop::SetListOfLoops(std::vector<SimpleLoopMetadata>& argListOfLoops)
{
	listOfLoops.clear();
	std::vector<SimpleLoopMetadata>::iterator theIterator;
	for( theIterator = argListOfLoops.begin(); theIterator != argListOfLoops.end(); theIterator++)
	{
		SimpleLoopMetadata& aSimpleLoop = *theIterator;
		listOfLoops.push_back(aSimpleLoop);
		
		//std::cout << "SDIFFileReaderWithLoop: received loop point <" << aSimpleLoop.GetStart();
		//std::cout << ">, <" << aSimpleLoop.GetEnd() << ">" << std::endl;
	}
}

Frame* SDIFFileReaderWithLoop::ReadFrame()
{
	int lFrameBufferPosition = SDIFFileReaderWithBuffer::GetFrameBufferPosition();
	SimpleLoopMetadata& simpleLoop = listOfLoops.at(indexOfCurrentLoop);

	std::cout << "SimpleLoopMetadata& simpleLoop = listOfLoops.at(indexOfCurrentLoop); indexOfCurrentLoop ==" << indexOfCurrentLoop << std::endl;

	
	// check to see whether we are at the loop point. if so, push the framePosition
	// back to the start of the loop
	if (lFrameBufferPosition == simpleLoop.GetEnd())
	{
		printf( "simpleLoop.GetStart()==%d, lFrameBufferPosition == simpleLoop.GetEnd() == %d\n", simpleLoop.GetStart(), simpleLoop.GetEnd() );
		lFrameBufferPosition = simpleLoop.GetStart();
		SDIFFileReaderWithBuffer::SetFrameBufferPosition(lFrameBufferPosition);
		printf( "ChooseLoopRandomly(%d,%d)", lFrameBufferPosition, indexOfCurrentLoop );
		indexOfCurrentLoop = ChooseLoopRandomly(lFrameBufferPosition, indexOfCurrentLoop);
		printf( "ChooseLoopRandomly returns %d\n", indexOfCurrentLoop );

		SimpleLoopMetadata& tempSimpleLoop = listOfLoops.at(indexOfCurrentLoop);
		std::cout << "LoopingSDIFFIleREader: switching to loop with start: " << tempSimpleLoop.GetStart();
		std::cout << " and end: " << tempSimpleLoop.GetEnd() << std::endl;
	}

	// get the current frame from the SDIFFileReaderWithBuffer
	Frame* nextFrame = SDIFFileReaderWithBuffer::ReadFrame();
	//std::cout << "get the current frame from the SDIFFileReaderWithBuffer" << std::endl;

	// we will later modify the frame, but we don't want the frame object
	// being cached by the BufferedSDIFFIleReader to be modified
	// so let's copy it.
	DataUtil::CopyFrameMembers(*nextFrame, *outgoingFrame);
	
	//std::cout << "copied. DataUtil::CopyFrameMembers(*nextFrame, *outgoingFrame);" << std::endl;

	// Check to see whether we are near the loop point. If so, we'll have
	// to crossfade this frame with the frame at the other end of the loop.
	if ( ( lFrameBufferPosition + numberOfCrossfadeFrames) >= simpleLoop.GetEnd() )
	{
		std::cout << "Near the loop point: if ( (lFrameBufferPosition + numberOfCrossfadeFrames) >= simpleLoop.GetEnd() )..." << std::endl;
//		if( lFrameBufferPosition > simpleLoop.GetStart() ) std::cout << "WARNING:: lFrameBufferPosition = " << lFrameBufferPosition << " > " << "simpleLoop.GetStart() = " << simpleLoop.GetStart() << std::endl;
		std::cout << lFrameBufferPosition << " + " << numberOfCrossfadeFrames << " start == " << simpleLoop.GetStart() << " end == " <<simpleLoop.GetEnd() << std::endl;
		Frame* startLoopFrame = SDIFFileReaderWithBuffer::GetFrame( simpleLoop.GetStart() );
		std::cout << "done: Frame* startLoopFrame = SDIFFileReaderWithBuffer::GetFrame( simpleLoop.GetStart() );" << std::endl;
		Frame* frameNearEndLoopFrame = nextFrame;
	
		// if we are crossfading 10 frames and we are currently on frame 3, the 
		// crossfade factor is 3/10
		float crossfadeFactor = ( (lFrameBufferPosition + numberOfCrossfadeFrames) 
								- simpleLoop.GetEnd() * 1.0 ) /
								numberOfCrossfadeFrames;

		std::cout << "done: Frame* frameNearEndLoopFrame = nextFrame; crossfadeFactor == " << crossfadeFactor << std::endl;
													
		CrossfadeSpectralPeakArrays( frameNearEndLoopFrame->GetSpectralPeakArray(), 
									 startLoopFrame->GetSpectralPeakArray(),
									 outgoingFrame->GetSpectralPeakArray(),
									 crossfadeFactor );
									
//		std::cout << "done: CrossfadeSpectralPeakArrays in ReadFrame." << std::endl;
									
		
		CrossfadeResidualSpectrum( frameNearEndLoopFrame->GetResidualSpec(), 
									startLoopFrame->GetResidualSpec(),
									outgoingFrame->GetResidualSpec(),
 									crossfadeFactor );
		
//		std::cout << "skip: CrossfadeResidualSpectrum." << std::endl;
//		std::cout << "exiting if ( (lFrameBufferPosition + numberOfCrossfadeFrames) >= simpleLoop.GetEnd() )" << std::endl;
	
	}
	
	return outgoingFrame;
}

int SDIFFileReaderWithLoop::ChooseLoopRandomly(int argFrameBufferPosition, int argIndexOfCurrentLoop)
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
//	std::cout << "validLoopPositions.end() =" << validLoopPositions.end() <<  std::endl;
//	if( validLoopPositions.end() == 0 ) printf( "validLoopPositions.end() == 0 \n" );
	
	for (unsigned int counter = 0; counter < listOfLoops.size(); counter++)
	{
		SimpleLoopMetadata& simpleLoop = listOfLoops.at( counter ); //indexOfCurrentLoop );
		
//		if ( argFrameBufferPosition < simpleLoop.GetEnd() 
//			&& SDIFFileReaderWithBuffer::GetNumberOfFramesLoaded() >= simpleLoop.GetEnd() )
		if ( argFrameBufferPosition < simpleLoop.GetEnd()
			&& SDIFFileReaderWithBuffer::GetNumberOfFramesLoaded() >= simpleLoop.GetStart () )
		{
			validLoopPositions.push_back(counter);
			if( counter == argIndexOfCurrentLoop )
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

void SDIFFileReaderWithLoop::CrossfadeSpectralPeakArrays(SpectralPeakArray& sourceSpectralPeaks1,
														SpectralPeakArray& sourceSpectralPeaks2,
														SpectralPeakArray& targetSpectralPeaks,
														float crossfadeFactor)
{
	// todo why does this sound bad?
	//sourceSpectralPeaks1.TodB();
	//sourceSpectralPeaks2.TodB();
	//targetSpectralPeaks.SetScale(EScale::eLog);
	
//	typedef Array<TData> DataArray; in array.hxx
	
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
	while ( srcCounter1 < numberOfPeaks1 &&  srcCounter2 < numberOfPeaks2 )  
	// used to be while ( srcCounter1 < numberOfPeaks1 || srcCounter2 < numberOfPeaks2 )
	{
//		printf( "freq1[%d/%d]=%f freq2[%d/%d]=%f\n", srcCounter1, numberOfPeaks1, srcFreqBuffer1[srcCounter1], srcCounter2, numberOfPeaks2, srcFreqBuffer2[srcCounter2] );
	
		float peakDifference = srcFreqBuffer1[srcCounter1] / srcFreqBuffer2[srcCounter2];
		
//		std::cout << "peakDifference==" << peakDifference << " targetCounter==" << targetCounter << std::endl;
		// if the peaks are less than a semitone apart, everything is fine. crossfade them.
		if ( peakDifference > semitoneDown && peakDifference < semitoneUp )
		{
//			std::cout << "get frequency , mag and phase" << std::endl;

			// get frequency , mag and phase
			targetFreqBuffer[targetCounter] = (srcFreqBuffer1[srcCounter1] * flippedCrossfadeFactor)
									+ (srcFreqBuffer2[srcCounter2] * crossfadeFactor );
			targetMagBuffer[targetCounter] = (srcMagBuffer1[srcCounter1] * flippedCrossfadeFactor)
									+ (srcMagBuffer2[srcCounter2] * crossfadeFactor );
			
//			if (targetCounter >= 0)
//			{
//				printf("targetCounter==%d. mag 1: %f, mag 2: %f, result: %f \n", targetCounter, srcMagBuffer1[srcCounter1], srcMagBuffer2[srcCounter2], targetMagBuffer[targetCounter]);
//				printf("scale is linear? %i \n", (sourceSpectralPeaks1.GetScale()==EScale::eLinear));
//			}
		
			targetCounter++;
			if (srcCounter1 < numberOfPeaks1)
				srcCounter1++;
			if (srcCounter2 < numberOfPeaks2)
				srcCounter2++;
//			printf("srcCounter=(%d,%d), numberOfPeaks=(%d,%d)\n", srcCounter1, srcCounter2, numberOfPeaks1, numberOfPeaks2 );
				
		}
		// the first frequency is a over a semitone below the second. let's fade the
		// first freqency out and skip on to the next frequency
		else if (peakDifference < semitoneDown)
		{
			printf( "peakDifference == %f  < semitoneDown == %f\n", peakDifference, semitoneDown );
			if (srcCounter1 < numberOfPeaks1)
				srcCounter1++;

			targetFreqBuffer[targetCounter] = (srcFreqBuffer1[srcCounter1] * flippedCrossfadeFactor);
			targetMagBuffer[targetCounter] = (srcMagBuffer1[srcCounter1] * flippedCrossfadeFactor);
		}
		// the first frequency is a over a semitone above the second. let's fade 
		// the second frequency in and skip on to the second frequency
		else
		{
		
			printf( "peakDifference == %f >= semitoneDown == %f\n", peakDifference, semitoneDown );

			if (srcCounter2 < numberOfPeaks2)
				srcCounter2++;

			targetFreqBuffer[targetCounter] = (srcFreqBuffer2[srcCounter2] * crossfadeFactor);
			targetMagBuffer[targetCounter] = (srcMagBuffer2[srcCounter2] * crossfadeFactor);
		}
	}

	//targetSpectralPeaks.ToLinear();
	
	std::cout << "done: CrossfadeSpectralPeakArrays" << std::endl;
	
}


void SDIFFileReaderWithLoop::CrossfadeResidualSpectrum(Spectrum& sourceSpectrum1,
								Spectrum& sourceSpectrum2,
								Spectrum& targetSpectrum,
								float crossfadeFactor)
{
	int size = sourceSpectrum1.GetSize();
	targetSpectrum.SetSize( size );

//	void GetConfig(SpectrumConfig& c) const;

//	SDIFInConfig sdifCfg;
	
	CLAM::SpectrumConfig spectCfg1;
	CLAM::SpectrumConfig spectCfg2;
	sourceSpectrum1.GetConfig( spectCfg1 );
	sourceSpectrum2.GetConfig( spectCfg2 );
	
	std::cout << "spectCfg1.SpecTypeFlags.bComplex==" << spectCfg1.GetType().bComplex << " spectCfg1.SpecTypeFlags.bMagPhase==" << spectCfg1.GetType().bMagPhase << std::endl;
	std::cout << "spectCfg2.SpecTypeFlags.bComplex==" << spectCfg2.GetType().bComplex << " spectCfg1.SpecTypeFlags.bMagPhase==" << spectCfg2.GetType().bMagPhase << std::endl;

// CLAM::SpecTypeFlags::bComplex

	Array<Complex>& srcComplexBuffer1 = sourceSpectrum1.GetComplexArray();
	Array<Complex>& srcComplexBuffer2 = sourceSpectrum2.GetComplexArray();
	Array<Complex>& targetComplexBuffer = targetSpectrum.GetComplexArray();

	float flippedCrossfadeFactor = 1.0 - crossfadeFactor;
	for( int r = 0; r < size; r++ )
	{
		// TODO figure out how to implement this using complex numbers
		targetComplexBuffer[r] = ( srcComplexBuffer1[r] * flippedCrossfadeFactor )
									+ ( srcComplexBuffer2[r] * crossfadeFactor );
	}
	
	int i;
	std::cout << "srcComplexBuffer1 (" << flippedCrossfadeFactor << ")";
	for( i = 0; i < 10; i++ ) std::cout << "[" << i << "]=" << srcComplexBuffer1[i] << " ";
	std::cout << std::endl;

	std::cout << "srcComplexBuffer2 (" << crossfadeFactor << ")";
	for( i = 0; i < 10; i++ ) std::cout << "[" << i << "]=" << srcComplexBuffer2[i] << " ";
	std::cout << std::endl;

	std::cout << "targetComplexBuffer";
	for( i = 0; i < 10; i++ ) std::cout << "[" << i << "]=" << targetComplexBuffer[i] << " ";
	std::cout << std::endl;

/*	SpecTypeFlags sflags;
	sflags.bComplex=true;
	sflags.bMagPhase=true;
	sflags.bMagPhaseBPF=false;
	sflags.bPolar=false;
	targetSpectrum.SynchronizeTo( sflags ); */
	
	targetSpectrum.Complex2MagPhase( targetSpectrum );  // what's wrong with this?
	
//	CLAM::Complex2MagPhase( targetComplexBuffer, targetSpectrum.getMagBuffer(), targetSpectrum.getPhaseBuffer() );
		
	std::cout << "CrossfadeResidualSpectrum done." << std::endl;

}

void SDIFFileReaderWithLoop::Reset()
{
	SDIFFileReaderWithBuffer::SetFrameBufferPosition( 0 );
}

} // end namespace CLAM

