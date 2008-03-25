#ifndef AudioSink_hxx
#define AudioSink_hxx

#include "Processing.hxx"
#include "NullProcessingConfig.hxx"
#include "AudioInPort.hxx"

namespace CLAM
{
	class AudioSink : public Processing
	{
	private:
		AudioInPort mIn;
		NullProcessingConfig mConf;
		float* mFloatBuffer;
		double* mDoubleBuffer;
		unsigned mBufferSize;

	public:
		AudioSink(const ProcessingConfig & conf=Config())
			: mIn("AudioIn",this)
			, mFloatBuffer(0)
			, mDoubleBuffer(0)
			, mBufferSize(0)
		{
			//After being dropped it is ready to run as it does not need any configuration at all
			SetExecState(Ready);
		}

		/// @deprecated Delegated to SetExternalBuffer
		void SetFrameAndHopSize(const int val)
		{
			mIn.SetSize(val);
			mIn.SetHop(val);
		}

		~AudioSink() {}

		void SetExternalBuffer(float* buf, unsigned nframes );
		void SetExternalBuffer(double* buf, unsigned nframes );

		bool Do();
		
		const char* GetClassName() const { return "AudioSink";}

		bool ConcreteConfigure(const ProcessingConfig &c)
		{
			return true;
		}

		const ProcessingConfig& GetConfig() const
		{
			return mConf;
		}
		
	};
} //namespace CLAM

#endif

