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
		AudioSink();
		AudioSink(const ProcessingConfig & conf);

		/// @deprecated Delegated to SetExternalBuffer
		void SetFrameAndHopSize(const int val)
		{
			mIn.SetSize(val);
			mIn.SetHop(val);
		}

		~AudioSink();

		void SetExternalBuffer(float* buf, unsigned nframes );
		void SetExternalBuffer(double* buf, unsigned nframes );

		bool Do();
		
		const char* GetClassName() const { return "AudioSink";}

		bool ConcreteConfigure(const ProcessingConfig &c)
		{
std::cout << "AudioSink::ConcreteConfigure(..) - buffer size: "
<< mBufferSize << std::endl
<< "BackendBufferSize() " << BackendBufferSize() << std::endl;
			return true;
		}

		const ProcessingConfig& GetConfig() const
		{
			return mConf;
		}
		
	};
} //namespace CLAM

#endif

