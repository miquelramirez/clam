#include "BufferedSDIFFileReader.hxx"

namespace CLAM
{

BufferedSDIFFileReader::BufferedSDIFFileReader() :
	mSDIFFileReader(),
	mFrameLoadChunkSize( DEFAULT_FRAME_LOAD_CHUNK_SIZE ),
	mThreshholdForPreloading( DEFAULT_THRESHHOLD_FOR_PRELOADING ),
	mThreshholdForPreloadingOnThread( DEFAULT_THRESHHOLD_FOR_PRELOADING_ON_THREAD ),
	mMaximumThreadIdleTime( DEFAULT_MAXIMUM_THREAD_IDLE_TIME ),
	mReaderHasMoreFrames( true ),
	dequeMutex(),
	readSDIFMutex(),
	framePosition(1)
{
	mFunctor = makeMemberFunctor0( *this, BufferedSDIFFileReader, Run );
}

BufferedSDIFFileReader::BufferedSDIFFileReader(const SDIFInConfig& argSDIFInConfig) :
	mSDIFFileReader(argSDIFInConfig),
	mFrameLoadChunkSize( DEFAULT_FRAME_LOAD_CHUNK_SIZE ),
	mThreshholdForPreloading( DEFAULT_THRESHHOLD_FOR_PRELOADING ),
	mThreshholdForPreloadingOnThread( DEFAULT_THRESHHOLD_FOR_PRELOADING_ON_THREAD ),
	mMaximumThreadIdleTime( DEFAULT_MAXIMUM_THREAD_IDLE_TIME ),
	mReaderHasMoreFrames( true ),
	dequeMutex(),
	readSDIFMutex(),
	framePosition(1)
{
	mFunctor = makeMemberFunctor0( *this, BufferedSDIFFileReader, Run );

	LoadFramesIntoBuffer( DEFAULT_INITIAL_NUMBER_OF_FRAMES_TO_BUFFER );
}

BufferedSDIFFileReader::~BufferedSDIFFileReader()
{
	for (int counter = 0; counter < pendingFrameBuffer.size(); counter++)
	{
		delete pendingFrameBuffer[counter];
	}

	for (int counter = 0; counter < processedFrameBuffer.size(); counter++)
	{
		delete processedFrameBuffer[counter];
	}
}

bool BufferedSDIFFileReader::Configure(const SDIFInConfig& c)
{
	bool response;
	if ( response = mSDIFFileReader.Configure(c) )
	{
		LoadFramesIntoBuffer( DEFAULT_INITIAL_NUMBER_OF_FRAMES_TO_BUFFER );
	}

	return response;
}

const SDIFInConfig BufferedSDIFFileReader::GetConfig()
{
	return dynamic_cast<const SDIFInConfig&>(mSDIFFileReader.GetConfig());
}

Frame* BufferedSDIFFileReader::ReadFrame()
{
	if ((mReaderHasMoreFrames == true) && (pendingFrameBuffer.size() < mThreshholdForPreloading))
	{
		mReaderHasMoreFrames = LoadFramesIntoBuffer(mFrameLoadChunkSize);
	}

	if ( pendingFrameBuffer.size() == 0 )
	{
		return NULL;
	}
	else
	{
		Mutex::ScopedLock lock( dequeMutex );

		Frame* nextFrame = pendingFrameBuffer.at(0);
		pendingFrameBuffer.pop_front();
		processedFrameBuffer.push_back(nextFrame);

		return nextFrame;
	}
}

bool BufferedSDIFFileReader::LoadFramesIntoBuffer(int argNumberOfBuffers)
{
	for (int counter = 0; counter < argNumberOfBuffers; counter++)
	{
		Frame* aFrame = new Frame();
		aFrame->AddSpectralPeakArray();
		aFrame->AddResidualSpec();
		aFrame->AddFundamental();
		aFrame->AddSynthAudioFrame();
		aFrame->UpdateData();

		TTime frameCenterTime;

		{
			// this object locks the mSDIFFileReader until the scopedlock goes
			// out of scope and is garbage collected. its destructor frees the lock
			Mutex::ScopedLock lock( readSDIFMutex );

			if (framePosition == endLoopPosition.GetFramePosition())
			{
				mSDIFFileReader.SetReadPosition( startLoopPosition.GetSDIFReadPosition() );
				framePosition = startLoopPosition.GetFramePosition();

				// BEGIN DEBUG
				//std::cout << "BufferedSDIFFileReader> LoadFramesIntoBuffer; looping " << GetConfig().GetFileName() << std::endl;
				//std::cout << "  Pushed SDIFFileReader read position back to: " << startLoopPosition.GetSDIFReadPosition() << std::endl;
				// END DEBUG
			}

			mReaderHasMoreFrames = mSDIFFileReader.ReadFrame( aFrame->GetFundamental(), aFrame->GetSpectralPeakArray(), aFrame->GetResidualSpec(), frameCenterTime );
			framePosition++;

			//std::cout << "BufferedSDIFFIleReader::LoadFrame; framePosition <" << framePosition << ">, startLoopPosition <" << startLoopPosition.GetFramePosition() << ">" << std::endl;
			if (framePosition == startLoopPosition.GetFramePosition())
			{
				//std::cout << "BufferedSDIFFIleReader::LoadFrame; setting read position of start loop to <" << mSDIFFileReader.GetReadPosition() << ">" << std::endl;

				startLoopPosition.SetSDIFReadPosition( mSDIFFileReader.GetReadPosition() );
			}
		}

		if (mReaderHasMoreFrames)
		{
			aFrame->SetCenterTime(frameCenterTime);

			Mutex::ScopedLock lock( dequeMutex );

			pendingFrameBuffer.push_back( aFrame );
		}
		else
		{
			delete aFrame;
			break;
		}
	}

	return mReaderHasMoreFrames;
}

void BufferedSDIFFileReader::LoadFramesIntoBufferOnThread(Thread* argThread)
{
	CLAM_ASSERT(argThread != NULL, "Thread* given to BufferedSDIFFileReader may not be null!");

	mThreadPtr = argThread;
	try // Note the exception handling
	{
		// TODO
		// are vectors thread safe?
		// the method LoadFramesIntoBuffer should check whether any more frames need to
		// be loaded and if not it should yield or sleep
		mThreadPtr->SetThreadCode( mFunctor );

		mThreadPtr->Start();

		//mThreadPtr->Stop();

	}
	catch( std::exception& e ) // Here we handle standard library exceptions
	{
		std::cerr << e.what() << std::endl;
		// TODO. Do you really want to exit() here?
		exit(-1);
	}
}

void BufferedSDIFFileReader::StopLoadingFramesIntoBufferOnThread()
{
	try // Note the exception handling
	{
		if ( mThreadPtr != NULL)
		{
			//mThreadPtr->ReturnThreadToPool();
			mThreadPtr = NULL;
		}
	}
	catch( std::exception& e ) // Here we handle standard library exceptions
	{
		std::cerr << e.what() << std::endl;
		// TODO. Do you really want to exit() here?
		exit(-1);
	}
}

bool BufferedSDIFFileReader::IsThreaded()
{
	return mThreadPtr != NULL;
}

void BufferedSDIFFileReader::Run()
{
	int iterationsSinceLastExecution = 0;
	while ( mReaderHasMoreFrames )
	{
		int numberOfFramesAboveThreshhold = pendingFrameBuffer.size() - mThreshholdForPreloadingOnThread;
		//std::cout << "This <" << this << "> has " <<  pendingFrameBuffer.size() << " buffers." << std::endl;
		if ( (mReaderHasMoreFrames == true) && (numberOfFramesAboveThreshhold <= 0) )
		{
			//std::cout << "Thread <" << mThreadPtr << "> this <" << this << "> is loading buffers." << std::endl;
			mReaderHasMoreFrames = LoadFramesIntoBuffer(mFrameLoadChunkSize);
			iterationsSinceLastExecution = 0;
			mThreadPtr->Yield();
		}
		else
		{
			// sleep for 5 milliseconds for each frame that is yet to be processed
			// it would be better to sleep for the number of milliseconds used as the
			// synthesis algorigthms hop size but we don't know what this is.
			//int sleepTime = numberOfFramesAboveThreshhold * 256;
			//std::cout << "Thread <" << mThreadPtr << "> <" << this << "> is sleeping." << std::endl;
			int sleepTime = numberOfFramesAboveThreshhold * 5;
			mThreadPtr->Sleep( sleepTime );
			iterationsSinceLastExecution++;

			double timeSinceLastExecution = sleepTime * iterationsSinceLastExecution;
			if (timeSinceLastExecution > mMaximumThreadIdleTime)
			{
				//std::cout << "Thread <" << mThreadPtr << "> <" << this << "> is being killed." << std::endl;
				//StopLoadingFramesIntoBufferOnThread();
				mThreadPtr = NULL;
				return;
			}
		}
	}
}

bool BufferedSDIFFileReader::isLooping()
{
	return (startLoopPosition.GetFramePosition() != -1 && endLoopPosition.GetFramePosition() != -1);
}

void BufferedSDIFFileReader::SetLoopPoints(int startFrame, int endFrame)
{
	startLoopPosition.SetFramePosition( startFrame );
	endLoopPosition.SetFramePosition( endFrame );
}

} // END CLAM


