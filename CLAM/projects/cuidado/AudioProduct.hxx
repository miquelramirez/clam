#ifndef _AUDIO_PRODUCT_H_
#define _AUDIO_PRODUCT_H_

#include "ProcessingObject.hxx"
#include "DynamicType.hxx"
#include "Spectrum.hxx"
#include "Audio.hxx"

namespace MTG {


	class AudioProductConfig: public ProcessingObjectConfig
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (AudioProductConfig, 2,ProcessingObjectConfig);
		REGISTER (0, Name, std::string);

		void DefaultInit();

	};


	class AudioProduct: public ProcessingObject {
		AudioProductConfig mConfig;

		const char *GetClassName() {return "AudioProduct";}

		/** Config change method
		 * @throw
		 * bad_cast exception when the argument is not an SpecAdderConfig
		 * object.
		 */
		bool ConcreteConfigure(const ProcessingObjectConfig&) throw(std::bad_cast);

	public:
		AudioProduct();

		AudioProduct(const AudioProductConfig &c);

		~AudioProduct();

		const AudioProductConfig &GetConfig() const { return mConfig;}

 		void Check(Audio& in1,Audio& in2, Audio& out);

		bool Do(void);

		bool Do(Audio& in1, Audio& in2, Audio& out);

		// Port interfaces.

		bool SetPrototypes(Audio& in1,Audio& in2, const Audio& out);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		void StoreOn(Storage &s) {};

	private:
	};

}

#endif //_AUDIO_PRODUCT_H_
