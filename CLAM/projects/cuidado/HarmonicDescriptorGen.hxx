#ifndef _HARMONICDESCRIPTORGEN_H_
#define _HARMONICDESCRIPTORGEN_H_

#include "ProcessingObject.hxx"
#include "DynamicType.hxx"
#include "SpectralPeakArray.hxx"
#include "HarmonicDescriptor.hxx"

namespace MTG {


	class HarmonicDescriptorGenConfig: public ProcessingObjectConfig
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (HarmonicDescriptorGenConfig, 2,ProcessingObjectConfig);
		REGISTER (0, Name, std::string);

		void DefaultInit();

	};



	class HarmonicDescriptorGen: public ProcessingObject {
		HarmonicDescriptorGenConfig mConfig;

		const char *GetClassName() {return "HarmonicDescriptorGen";}

		/** Config change method
		 * @throw
		 * bad_cast exception when the argument is not an SpecAdderConfig
		 * object.
		 */
		bool ConcreteConfigure(const ProcessingObjectConfig&) throw(std::bad_cast);

	public:
		HarmonicDescriptorGen();

		HarmonicDescriptorGen(const HarmonicDescriptorGenConfig &c);

		~HarmonicDescriptorGen();

		const HarmonicDescriptorGenConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(SpectralPeakArray& input, HarmonicDescriptor& out);

		// Port interfaces.

		/** Change the internal type state.  
		 * Apart from prototype configuration, the Size, Scale and
		 * SpectralRange attributes of each Spectrum are also
		 * checked.
		 */

		bool SetPrototypes(SpectralPeakArray& input,const HarmonicDescriptor& out);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		void StoreOn(Storage &s) {};

	private:
	};

}

#endif // _SPECTRUM_ADDER_
