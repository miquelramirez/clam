#ifndef _EXTERN_SINK_HXX_
#define _EXTERN_SINK_HXX_

#include "Processing.hxx"
#include "NullProcessingConfig.hxx"
#include "AudioInPort.hxx"

namespace CLAM
{
	class ExternSink : public Processing
	{
	private:
		AudioInPort mIn;
		NullProcessingConfig mConf;

	public:
		ExternSink();
		ExternSink(const ProcessingConfig & conf);

		void SetFrameAndHopSize(const int val)
		{
			mIn.SetSize(val);
			mIn.SetHop(val);
		}

		~ExternSink();

		bool Do();
		bool Do( CLAM::TData* buf, int nframes);
		
		const char* GetClassName() const { return "ExternSink";}

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
