#ifndef _PEAKLIKELIHOOD_H__
#define _PEAKLIKELIHOOD_H__

#include "ProcessingObject.hxx"
#include "DynamicType.hxx"
#include "Spectrum.hxx"
#include "Audio.hxx"

namespace MTG {


	class PeakLikelihoodConfig: public ProcessingObjectConfig
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (PeakLikelihoodConfig, 1,ProcessingObjectConfig);
		REGISTER (0, Name, std::string);

		void DefaultInit();

	};


	class PeakLikelihood: public ProcessingObject {
		PeakLikelihoodConfig mConfig;

		const char *GetClassName() {return "PeakLikelihood";}

		/** Config change method
		 * @throw
		 * bad_cast exception when the argument is not an SpecAdderConfig
		 * object.
		 */
		bool ConcreteConfigure(const ProcessingObjectConfig&) throw(std::bad_cast);

	public:
		PeakLikelihood();

		PeakLikelihood(const PeakLikelihoodConfig &c);

		~PeakLikelihood();

		const PeakLikelihoodConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(Spectrum& input, Spectrum& out);

		// Port interfaces.

		/** Change the internal type state.  
		 * Apart from prototype configuration, the Size, Scale and
		 * SpectralRange attributes of each Spectrum are also
		 * checked.
		 */

		bool SetPrototypes(Spectrum& inputs,const Spectrum& out);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		void StoreOn(Storage &s) {};

	private:
	};

}

#endif // _PEAKLIKELIHOOD_H__
