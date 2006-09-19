#ifndef _EXTERN_GENERATOR_HXX_
#define _EXTERN_GENERATOR_HXX_

#include "Processing.hxx"
#include "NullProcessingConfig.hxx"
#include "AudioOutPort.hxx"

namespace CLAM
{
	class ExternGenerator : public Processing
	{
	private:
		AudioOutPort mOut;
		NullProcessingConfig mConf;

	public:
		ExternGenerator();
		ExternGenerator(const ProcessingConfig & conf);

		void SetFrameAndHopSize(const int val)
		{
			mOut.SetSize(val);
			mOut.SetHop(val);
		}
		
		~ExternGenerator();

		bool Do();
		bool Do( CLAM::TData* buf, int nframes);
		
		const char* GetClassName() const { return "ExternGenerator";}

		bool ConcreteConfigure(const ProcessingConfig &c)
		{
			return true;
		}

		const ProcessingConfig& GetConfig() const
		{
			return mConf;
		}
		
		/** Wether the processing is a sync source such as audio i/o device,
		 * or an audio callback hook (i.e. Externalizer) */
		bool IsSyncSource() const { return true; }
	};
} //namespace CLAM

#endif
