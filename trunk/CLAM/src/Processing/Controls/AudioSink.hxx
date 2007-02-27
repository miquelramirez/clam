#ifndef _EXTERN_SINK_HXX_
#define _EXTERN_SINK_HXX_

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
		bool Do( float* buf, int nframes);
		bool Do( double* buf, int nframes);
		
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
