#ifndef _SDIFFileReaderWithBuffer_
#define _SDIFFileReaderWithBuffer_

#include "SDIFInConfig.hxx"
#include "SDIFFileReader.hxx"
#include "Frame.hxx"
#include "Thread.hxx"
#include "Mutex.hxx"
#include <deque>

/// this defines how many frames will be preloaded when this object is first
/// created. if you're creating a lot of objects for a lot of SDIF files,
/// you shouldn't make this too large
#define DEFAULT_INITIAL_NUMBER_OF_FRAMES_TO_BUFFER 50 // 150
/// this defines how many frames should be loaded before the thread yields.
/// the current value is arbitrary, and needs to be refined.
#define DEFAULT_FRAME_LOAD_CHUNK_SIZE 5
/// this class starts loading more frames when it only has this many frames
/// left in the buffer
#define DEFAULT_THRESHHOLD_FOR_PRELOADING 5
/// this class starts loading more frames when it only has this many frames
/// left in the buffer if it is loading the frames on a thread
#define DEFAULT_THRESHHOLD_FOR_PRELOADING_ON_THREAD 20

namespace CLAM
{

/**
*  SDIFFileReaderWithBuffer is an SDIFFileReader that preloads some SDIF frames
*  into an internal buffer so that real-time audio applications won't stall
*  while waiting for disk reading operations.  Instances can be given a thread
*  (presumably from a thread pool) so that the SDIF frames will be loaded in the
*  background.
*
*  \author greg kellum [gkellum@iua.upf.edu] 7/6/07
*  \since CLAM v1.1
*/
class SDIFFileReaderWithBuffer
{
public:
	/**
	* You MUST give the instance the filename of the SDIF file to read.
	* in the SDIFInConfig object.
	*/
	SDIFFileReaderWithBuffer(const SDIFInConfig& argSDIFInConfig);
	
	virtual ~SDIFFileReaderWithBuffer();

	/**
	* You MUST to give the instance the filename of the SDIF file to read.
	* in the SDIFInConfig object. You can further specify which kinds of
	* objects to read from the SDIFFile. See the doc for SDIFInConfig.
	*/
	bool Configure(const SDIFInConfig& c);

	const SDIFInConfig GetConfig();

	/**
	* This method returns the next frame from the internal buffer.
	*/
	virtual Frame* ReadFrame();

	/**
	* Use this method to give the instance a thread which it can use to load
	* the SDIF file in the background.
	*/
	void LoadFramesIntoBufferOnThread(Thread* argThread);

	/**
	* This method stops the thread given to this object. This should never (?)
	* be called however by a client because this class knows when it no longer
	* needs its thread uses this method to stop the thread itself.
	*
	* TODO should this be protected?
	*/
	void StopLoadingFramesIntoBufferOnThread();

	/**
	* True if and only if this has a thread.
	*/
	bool IsThreaded();

	/**
	* The number of frames that have been read into the buffer.
	*/
	int GetNumberOfFramesLoaded();
	
protected:
	int GetFrameBufferPosition();
	void SetFrameBufferPosition(int frameBufferPosition);
	Frame* GetFrame( int frameBufferPosition );
	
private:
	/**
	* Internal method for loading a certain sized chunk of SDIF frames.
	*/
	bool LoadFramesIntoBuffer(int argNumberOfBuffers);

	/**
	* Internal method to be called by a thread given by LoadFramesIntoBufferOnThread
	*/
	void Run();

	/// The SDIFFileReader for reading the SDIF file
	SDIFFileReader mSDIFFileReader;
	/// The linked list for frames that haven't been read yet
	std::deque<Frame*> frameBuffer;
	/// The position in the frameBuffer where we are currently reading from
	int frameBufferPosition;
	
	/// How many chunks should be read at one time
	int mFrameLoadChunkSize;
	/// When the instance only has this many chunks left, it will start reading more
	int mThreshholdForPreloading;
	/// When the instance only has this many chunks left, the thread will start
	/// reading more. This number should be higher than the mThreshholdForPreloading
	/// Otherwise, the thread will never be used.
	int mThreshholdForPreloadingOnThread;
	// The maximum amount of time that a thread can remain idle. After this
	// threshhold is crossed, the thread is stopped thereby returning it to the
	// pool where it can be reused.
	int mMaximumThreadIdleTime;
	/// Whether there are more SDIF frames in the buffer.
	bool mReaderHasMoreFrames;
	/// The thread to be used to read frames.
	Thread* mThreadPtr;
	/// The wrapper object that the thread will use to call the Run method.
	CBL::Functor0 mFunctor;
	// This is used to prevent the main thread and the reader thread from trying
	// to access the deque of frames simultaneously
	Mutex dequeMutex;
	/// This is used to prevent the main thread and the reader thread from trying to
	/// read the SDIF file at the same time.
	// if mThreshholdForPreloadingOnThread > mThreshholdForPreloading.
	Mutex readSDIFMutex;
	/// How many frames of the file should be loaded. If the file has multiple loop
	/// points. This should be the last loop point.
	int totalNumberOfFramesToLoad;
};

}

#endif

