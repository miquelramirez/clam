#include "SDIFFileReaderWithBuffer.hxx"
#include "DataUtil.hxx"
#include <limits.h>

namespace CLAM
{

SDIFFileReaderWithBuffer::SDIFFileReaderWithBuffer(const SDIFInConfig& argSDIFInConfig) :
	mSDIFFileReader(argSDIFInConfig),
	frameBufferPosition(1),
	mFrameLoadChunkSize( DEFAULT_FRAME_LOAD_CHUNK_SIZE ),
	mThreshholdForPreloading( DEFAULT_THRESHHOLD_FOR_PRELOADING ),
	mThreshholdForPreloadingOnThread( DEFAULT_THRESHHOLD_FOR_PRELOADING_ON_THREAD ),
	mReaderHasMoreFrames( true ),
	dequeMutex(),
	readSDIFMutex()
{
	mFunctor = makeMemberFunctor0( *this, SDIFFileReaderWithBuffer, Run );

	Configure(argSDIFInConfig);
}

SDIFFileReaderWithBuffer::~SDIFFileReaderWithBuffer()
{
	for (unsigned int counter = 0; counter < frameBuffer.size(); counter++)
	{
		delete frameBuffer[counter];
	}
}

bool SDIFFileReaderWithBuffer::Configure(const SDIFInConfig& config)
{
	bool response;
	if ( response = mSDIFFileReader.Configure(config) )
	{
		// if the metadata has defined the number of milliseconds to preload
		// preload the corresponding number of frames.
		if ( config.HasNumberOfFramesToPreload() )
		{
			std::cout << "Preloading " << config.GetNumberOfFramesToPreload()+1 << " frames" << std::endl;
			LoadFramesIntoBuffer( config.GetNumberOfFramesToPreload()+1 );
		}
		else
		{
			std::cout << "Preloading DEFAULT_INITIAL_NUMBER_OF_FRAMES_TO_BUFFER == " << DEFAULT_INITIAL_NUMBER_OF_FRAMES_TO_BUFFER << std::endl;
			LoadFramesIntoBuffer( DEFAULT_INITIAL_NUMBER_OF_FRAMES_TO_BUFFER );
		}
		
		if ( config.HasNumberOfFramesToLoad() )
		{
			totalNumberOfFramesToLoad = config.GetNumberOfFramesToLoad();
		}
		else
		{
			if (config.HasNumberOfFramesToPreload())
				totalNumberOfFramesToLoad = config.GetNumberOfFramesToPreload()+1;
			else
				totalNumberOfFramesToLoad = INT_MAX;
		}
	}

	return response;
}

const SDIFInConfig SDIFFileReaderWithBuffer::GetConfig()
{
	return dynamic_cast<const SDIFInConfig&>(mSDIFFileReader.GetConfig());
}

int SDIFFileReaderWithBuffer::GetFrameBufferPosition()
{
	return frameBufferPosition;
}

void SDIFFileReaderWithBuffer::SetFrameBufferPosition(int argFrameBufferPosition)
{
	frameBufferPosition = argFrameBufferPosition;
}

Frame* SDIFFileReaderWithBuffer::GetFrame( int frameBufferPosition )
{

	std::cout << "[SDIFFileReaderWithBuffer::GetFrame] frameBufferPosition==" << frameBufferPosition << " frameBuffer.size()="<< frameBuffer.size()<< std::endl;
	
	Mutex::ScopedLock lock( dequeMutex );

	std::cout << "[SDIFFileReaderWithBuffer::GetFrame] Mutex::ScopedLock lock( dequeMutex );" << std::endl;

	Frame* requestedFrame = frameBuffer.at( frameBufferPosition );

	std::cout << "[SDIFFileReaderWithBuffer::GetFrame] frameBuffer.at( frameBufferPosition ). frameBufferPosition==" << frameBufferPosition <<" frameBuffer.size()="<< frameBuffer.size() << std::endl;
	
	return requestedFrame;
}

Frame* SDIFFileReaderWithBuffer::ReadFrame()
{
	int framesTillTheEnd = frameBuffer.size() - frameBufferPosition;
	if ((mReaderHasMoreFrames == true) && (framesTillTheEnd < mThreshholdForPreloading))
	{
		mReaderHasMoreFrames = LoadFramesIntoBuffer(mFrameLoadChunkSize);
	}

	if ( frameBuffer.size() == 0 )
	{
	    std:: cout << "Frame* SDIFFileReaderWithBuffer::ReadFrame() return NULL because frameBuffer.size() == 0" << std::endl;
		return NULL;
	}
	else
	{
		Mutex::ScopedLock lock( dequeMutex );
		
		std::cout << "Retrieving buffer pos: " << frameBufferPosition << ", size: " << frameBuffer.size() << std::endl;
		Frame* nextFrame = frameBuffer.at(frameBufferPosition);
		frameBufferPosition++;

		if (nextFrame == NULL)
		{
			std::cout << "next frame is null" << std::endl;
		}

		return nextFrame;
	}
}

bool SDIFFileReaderWithBuffer::LoadFramesIntoBuffer(int argNumberOfBuffers)
{
	// when we read in the SDIFFrames, let's first put them in a temporary
	// list object so we can avoid locking the frameBuffer every iteration
	// of the loop
	std::deque<Frame*> tempFrameBuffer;

	// this object locks the mSDIFFileReader until the scopedlock goes
	// out of scope and is garbage collected. its destructor frees the lock
	{
		Mutex::ScopedLock lock( readSDIFMutex );
		
		// here's the loop where we read in the desired number of buffers
		for (int counter = 0; counter < argNumberOfBuffers; counter++)
		{
			Frame* aFrame = new Frame();
			aFrame->AddSpectralPeakArray();
			aFrame->AddResidualSpec();
			aFrame->AddFundamental();
			aFrame->AddSynthAudioFrame();
			aFrame->UpdateData();
	
			TTime frameCenterTime;
			
			// we read in the frame
			mReaderHasMoreFrames = mSDIFFileReader.ReadFrame( aFrame->GetFundamental(), aFrame->GetSpectralPeakArray(), aFrame->GetResidualSpec(), frameCenterTime );

			// we'll update the frame position variable when we actually add these to 
			// the frameVector
		
			// as long as there was something to read, add the frame to the temporary buffer
			if (mReaderHasMoreFrames)
			{
				aFrame->SetCenterTime(frameCenterTime);
		
				tempFrameBuffer.push_back( aFrame );
			}
			else
			{
				std::cout << "BufferedSDIFReader: could only load " << counter << " of ";
				std::cout << argNumberOfBuffers << " frames." << std::endl;
				delete aFrame;
				break;
			}
		}
	}

	// now let's copy the frames from the temporary buffer to the frameBuffer
	Mutex::ScopedLock lock( dequeMutex );
	for (unsigned int counter = 0; counter < tempFrameBuffer.size(); counter++)
	{
		Frame* aFrame = tempFrameBuffer.at(counter);
		
		std::cout << "[before push_back] frameBuffer.size()==" << frameBuffer.size() << " counter==" << counter << std::endl;
		frameBuffer.push_back( aFrame );
		std::cout << "[after push_back] frameBuffer.size()==" << frameBuffer.size() << std::endl;

	}
	
	return mReaderHasMoreFrames;
}

int SDIFFileReaderWithBuffer::GetNumberOfFramesLoaded()
{
	Mutex::ScopedLock lock( dequeMutex );
	return frameBuffer.size();
}

void SDIFFileReaderWithBuffer::LoadFramesIntoBufferOnThread(Thread* argThread)
{
	CLAM_ASSERT(argThread != NULL, "Thread* given to SDIFFileReaderWithBuffer may not be null!");

	mThreadPtr = argThread;
	try // Note the exception handling
	{
		mThreadPtr->SetThreadCode( mFunctor );

		mThreadPtr->Start();

		//mThreadPtr->Stop();

	}
	catch( std::exception& e ) // Here we handle standard library exceptions
	{
		std::cerr << e.what() << std::endl;
		std::string msg("SDIFFileReaderWithBuffer: exception when starting thread. SDIFFile will be loaded on main thread.");
		std::cerr << msg << std::endl;
	}
}

void SDIFFileReaderWithBuffer::StopLoadingFramesIntoBufferOnThread()
{
	try // Note the exception handling
	{
		if ( mThreadPtr != NULL)
		{
			mThreadPtr = NULL;
		}
	}
	catch( std::exception& e ) // Here we handle standard library exceptions
	{
		std::cerr << e.what() << std::endl;
		std::string msg("SDIFFileReaderWithBuffer: exception when stopping thread.");
		std::cerr << msg << std::endl;
	}
}

bool SDIFFileReaderWithBuffer::IsThreaded()
{
	return mThreadPtr != NULL;
}

void SDIFFileReaderWithBuffer::Run()
{
	int iterationsSinceLastExecution = 0;
	while ( mReaderHasMoreFrames && totalNumberOfFramesToLoad <= frameBuffer.size() )
	{
		//std::cout << "Thread <" << mThreadPtr << "> this <" << this << "> is loading buffers." << std::endl;
		mReaderHasMoreFrames = LoadFramesIntoBuffer(mFrameLoadChunkSize);
		iterationsSinceLastExecution = 0;
		mThreadPtr->Yield();
	}
}



} // END CLAM


