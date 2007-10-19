#ifndef _BufferedSDIFFileReader_
#define _BufferedSDIFFileReader_

#include "SDIFInConfig.hxx"
#include "SDIFFileReader.hxx"
#include "Frame.hxx"
#include "Thread.hxx"
#include "Mutex.hxx"
#include <deque>

#define DEFAULT_INITIAL_NUMBER_OF_FRAMES_TO_BUFFER 10
#define DEFAULT_FRAME_LOAD_CHUNK_SIZE 5
#define DEFAULT_THRESHHOLD_FOR_PRELOADING 5
#define DEFAULT_THRESHHOLD_FOR_PRELOADING_ON_THREAD 20
#define DEFAULT_MAXIMUM_THREAD_IDLE_TIME 2500

namespace CLAM
{

/**
* This is a helper class used internally by the BufferedSDIFFileReader to
* store frame positions together with the read position of that frame in an
* SDIF file. This class was not intended to be used by clients for anything.
*
*  \author greg kellum [gkellum@iua.upf.edu] 7/6/07
*  \since CLAM v1.1
*/
class LoopPosition
{
public:
	LoopPosition() : framePosition(-1), readPosition(-1) {}
	LoopPosition(int argFramePosition) : framePosition(argFramePosition), readPosition(-1) {}
	// FramePosition property - the frame position of a loop point. This is dependent of course
	// upon the FFT size and hop size used to analyze a sample.
	int GetFramePosition() { return framePosition; }
	void SetFramePosition(int argFramePosition) { framePosition = argFramePosition; }
	// SDIFReadPosition - the read position of a frame in a particular SDIF file
	int GetSDIFReadPosition() { return readPosition; }
	void SetSDIFReadPosition(int argReadPosition) { readPosition = argReadPosition; }
private:
	int framePosition;
	int readPosition;
};

/**
*  BufferedSDIFFileReader is an SDIFFileReader that preloads some SDIF frames
*  into an internal buffer so that real-time audio applications won't stall
*  while waiting for disk reading operations.  Instances can be given a thread
*  (presumably from a thread pool) so that the SDIF frames will be loaded in the
*  background.
*
*  \author greg kellum [gkellum@iua.upf.edu] 7/6/07
*  \since CLAM v1.1
*/
class BufferedSDIFFileReader
{
public:
	/**
	* Default constructor. If you use this constructor, you MUST later call
	* Configure(SDIFInConfig) to give the instance the filename of the SDIF
	* file to read.
	*/
	BufferedSDIFFileReader();

	/**
	* You MUST to give the instance the filename of the SDIF file to read.
	* in the SDIFInConfig object.
	*/
	BufferedSDIFFileReader(const SDIFInConfig& argSDIFInConfig);

	~BufferedSDIFFileReader();

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
	Frame* ReadFrame();

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

	bool IsThreaded();

	bool isLooping();
	void SetLoopPoints(int startFrame, int endFrame);

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
	std::deque<Frame*> pendingFrameBuffer;
	/// The linked list for frames that have been read already
	std::deque<Frame*> processedFrameBuffer;
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
	/// This keeps track of which frame number we're reading reading
	int framePosition;
	LoopPosition startLoopPosition;
	LoopPosition endLoopPosition;
};

}

#endif

