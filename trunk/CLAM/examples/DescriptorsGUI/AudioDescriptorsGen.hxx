#ifndef _AUDIODESCRIPTORSGEN_H_
#define _AUDIODESCRIPTORSGEN_H_

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "AudioDescriptors.hxx"
#include "Audio.hxx"
#include "BasicStatistics.hxx"

namespace CLAM {


	class AudioDescriptorsGenConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (AudioDescriptorsGenConfig, 1,ProcessingConfig);
		DYN_ATTRIBUTE (0,public, std::string, Name);
		
		void DefaultInit();

	};




	class AudioDescriptorsGen: public Processing {
		AudioDescriptorsGenConfig mConfig;

		const char *GetClassName() const {return "AudioDescriptorsGen";}

		/** Config change method
		 * @throw
		 * bad_cast exception when the argument is not an SpecAdderConfig
		 * object.
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

	public:
		AudioDescriptorsGen();

		AudioDescriptorsGen(const AudioDescriptorsGenConfig &c);

	        ~AudioDescriptorsGen();

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(Audio& input,AudioDescriptors& output);

		// Port interfaces.

		/** Change the internal type state.  
		 * Apart from prototype configuration, the Size, Scale and
		 * SpectralRange attributes of each Spectrum are also
		 * checked.
		 */

		bool SetPrototypes(Audio& input,const AudioDescriptors& out);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		void StoreOn(Storage &s) {};

	private:
	};

  /* Summation Kernels for usage in Calculations (experimental) */

  SUM_KERNEL(zerocross,out+=(X1[i]*X1[i+1] < 0.));
  SUM_KERNEL(offsettime,if (X1[i] > 0.02*X2[0]) if (out == 0) out = i;);

}

#endif // _AUDIODESCRIPTORGEN_H_
