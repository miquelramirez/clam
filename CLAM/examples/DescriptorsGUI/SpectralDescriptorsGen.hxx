#ifndef _SPECTRALDESCRIPTORSGEN_H_
#define _SPECTRALDESCRIPTORSGEN_H_

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "SpectralDescriptors.hxx"
#include "Spectrum.hxx"
#include "BasicStatistics.hxx"
#include "MFCCGen.hxx"

namespace CLAM {


	class SpectralDescriptorsGenConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE(SpectralDescriptorsGenConfig, 1,ProcessingConfig);
		DYN_ATTRIBUTE (0,public, std::string, Name);
		void DefaultInit();

	};




	class SpectralDescriptorsGen: public Processing {
		SpectralDescriptorsGenConfig mConfig;

		const char *GetClassName() const {return "SpectralDescriptorsGen";}

		/** Config change method
		 * @throw
		 * bad_cast exception when the argument is not an SpecAdderConfig
		 * object.
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

	public:
		SpectralDescriptorsGen();

		SpectralDescriptorsGen(const SpectralDescriptorsGenConfig &c);

		~SpectralDescriptorsGen();

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(Spectrum& input,SpectralDescriptors& output);


		// Port interfaces.

		/** Change the internal type state.  
		 * Apart from prototype configuration, the Size, Scale and
		 * SpectralRange attributes of each Spectrum are also
		 * checked.
		 */

		bool SetPrototypes(Spectrum& input,const SpectralDescriptors& out);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		void StoreOn(Storage &s) {};


	private:
		MFCCGen mMFCCGen;
	};

}

#endif // _AUDIODESCRIPTORGEN_H_
