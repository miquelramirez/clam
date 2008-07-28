#ifndef AudioSource_hxx
#define AudioSource_hxx

#include "Processing.hxx"
#include "AudioOutPort.hxx"

namespace CLAM
{
	class AudioSource : public Processing
	{
	private:
		AudioOutPort mOut;
		float* mFloatBuffer;
		double* mDoubleBuffer;
		unsigned mBufferSize;
	public:
		AudioSource(const ProcessingConfig & conf=Config())
			: mOut("AudioOut",this)
			, mFloatBuffer(0)
			, mDoubleBuffer(0)
			, mBufferSize(0)
		{
			//After being dropped it is ready to run as it does not need any configuration at all
			SetExecState(Ready);
		}

		void SetFrameAndHopSize(const int val)
		{
			mOut.SetSize(val);
			mOut.SetHop(val);
		}
		
		void SetExternalBuffer(float* buf, unsigned nframes );
		void SetExternalBuffer(double* buf, unsigned nframes );

		bool Do();
		
		const char* GetClassName() const { return "AudioSource";}

	};
} //namespace CLAM

#endif

