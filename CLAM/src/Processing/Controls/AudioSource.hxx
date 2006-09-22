#ifndef _EXTERN_GENERATOR_HXX_
#define _EXTERN_GENERATOR_HXX_

#include "Processing.hxx"
#include "NullProcessingConfig.hxx"
#include "AudioOutPort.hxx"

namespace CLAM
{
	class AudioSource : public Processing
	{
	private:
		AudioOutPort mOut;
		NullProcessingConfig mConf;

	public:
		AudioSource();
		AudioSource(const ProcessingConfig & conf);

		void SetFrameAndHopSize(const int val)
		{
			mOut.SetSize(val);
			mOut.SetHop(val);
		}
		
		~AudioSource();

		bool Do();
		bool Do( CLAM::TData* buf, int nframes);
		
		const char* GetClassName() const { return "AudioSource";}

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
