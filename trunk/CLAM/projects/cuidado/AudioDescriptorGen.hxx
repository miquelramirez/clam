#ifndef _AUDIODESCRIPTORGEN_H_
#define _AUDIODESCRIPTORGEN_H_

#include "ProcessingObject.hxx"
#include "DynamicType.hxx"
#include "AudioDescriptor.hxx"
#include "Audio.hxx"
#include "BasicStatistics.hxx"

namespace MTG {


	class AudioDescriptorGenConfig: public ProcessingObjectConfig
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (AudioDescriptorGenConfig, 2,ProcessingObjectConfig);
		REGISTER (0, Name, std::string);

		void DefaultInit();

	};




	class AudioDescriptorGen: public ProcessingObject {
		AudioDescriptorGenConfig mConfig;

		const char *GetClassName() {return "AudioDescriptorGen";}

		/** Config change method
		 * @throw
		 * bad_cast exception when the argument is not an SpecAdderConfig
		 * object.
		 */
		bool ConcreteConfigure(const ProcessingObjectConfig&) throw(std::bad_cast);

	public:
		AudioDescriptorGen();

		AudioDescriptorGen(const AudioDescriptorGenConfig &c);

		~AudioDescriptorGen();

		const AudioDescriptorGenConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(Audio& input,AudioDescriptor& output);

		// Port interfaces.

		/** Change the internal type state.	
		 * Apart from prototype configuration, the Size, Scale and
		 * SpectralRange attributes of each Spectrum are also
		 * checked.
		 */

		bool SetPrototypes(Audio& input,const AudioDescriptor& out);

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
