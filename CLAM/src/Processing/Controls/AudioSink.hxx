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

	public:
		AudioSink();
		AudioSink(const ProcessingConfig & conf);

		void SetFrameAndHopSize(const int val)
		{
			mIn.SetSize(val);
			mIn.SetHop(val);
		}

		~AudioSink();

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
