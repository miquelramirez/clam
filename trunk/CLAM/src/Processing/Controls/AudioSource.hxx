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
		float* mFloatBuffer;
		double* mDoubleBuffer;
		unsigned mBufferSize;
	public:
		AudioSource();
		AudioSource(const ProcessingConfig & conf);

		void SetFrameAndHopSize(const int val)
		{
			mOut.SetSize(val);
			mOut.SetHop(val);
		}
		
		~AudioSource();
		void SetExternalBuffer(float* buf, unsigned nframes );
		void SetExternalBuffer(double* buf, unsigned nframes );

		bool Do();
		//TODO deprecate
		bool Do( float* buf, int nframes);
		//TODO deprecate
		bool Do( double* buf, int nframes);
		
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
