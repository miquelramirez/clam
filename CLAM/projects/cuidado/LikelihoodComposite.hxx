#ifndef _LIKELIHOODCOMPOSITE_H_
#define _LIKELIHOODCOMPOSITE_H_

#include "ProcessingObject.hxx"
#include "ProcessingObjectComposite.hxx"
#include "DynamicType.hxx"
#include "Spectrum.hxx"
#include "Audio.hxx"
#include "WindowGenerator.hxx"
#include "CircularShift.hxx"
#include "SpectrumProduct.hxx"
#include "PeakLikelihood.hxx"

namespace MTG {


	class LikelihoodCompositeConfig: public ProcessingObjectConfig
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (LikelihoodCompositeConfig, 4,ProcessingObjectConfig);
		REGISTER (0, Name, std::string);
		REGISTER (1, Size, TSize);
		REGISTER (2, NumBands, TSize);
		REGISTER (3, BandDefinitions, TData*)
		void DefaultInit();

	};


	class LikelihoodComposite: public ProcessingObjectComposite {

		LikelihoodCompositeConfig mConfig;
		Spectrum         window;
		CircularShift    shift;

		WindowGenerator  windowGen;
		SpectrumProduct  prod;
		PeakLikelihood   likelihood;

		const char *GetClassName() {return "LikelihoodComposite";}
		
		bool ConcreteConfigure(const ProcessingObjectConfig&) throw(std::bad_cast);

		void AttachChildren();

		void ConfigureChildren();
	 
	public:
		LikelihoodComposite();

		LikelihoodComposite(LikelihoodCompositeConfig &c);

		~LikelihoodComposite();

		const LikelihoodCompositeConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(Spectrum& input, Spectrum* out[]);

		// Port interfaces.

		/** Change the internal type state.  
		 * Apart from prototype configuration, the Size, Scale and
		 * SpectralRange attributes of each Spectrum are also
		 * checked.
		 */

		bool SetPrototypes(Spectrum& inputs,const Spectrum* out[]);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		void StoreOn(Storage &s) {};

	private:
	};

}

#endif // _SPECTRUM_ADDER_
