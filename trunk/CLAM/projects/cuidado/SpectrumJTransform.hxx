#ifndef _SPECTRUM_J_TRANSFORM_H_
#define _SPECTRUM_J_TRANSFORM_H_

#include "ProcessingObject.hxx"
#include "DynamicType.hxx"
#include "Spectrum.hxx"
#include "Audio.hxx"

namespace MTG {


	class SpectrumJTransformConfig: public ProcessingObjectConfig
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (SpectrumJTransformConfig, 2,ProcessingObjectConfig);
		REGISTER (0, Name, std::string);

		void DefaultInit();

	};

  /**
   *   This is just a template to which can be used to start a new Processing Object.
   *   Just copy it to another fiel and replace the  SpectrumJTransform with your  own 
   *   class name.
   *
   */


	class SpectrumJTransform: public ProcessingObject {
		SpectrumJTransformConfig mConfig;

		const char *GetClassName() {return "SpectrumJTransform";}

		/** Config change method
		 * @throw
		 * bad_cast exception when the argument is not an SpecAdderConfig
		 * object.
		 */
		bool ConcreteConfigure(const ProcessingObjectConfig&) throw(std::bad_cast);

	public:
		SpectrumJTransform();

		SpectrumJTransform(const SpectrumJTransformConfig &c);

	        ~SpectrumJTransform();

		const SpectrumJTransformConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(Spectrum& input, Spectrum& out);

		// Port interfaces.

		/** Change the internal type state.  
		 * Apart from prototype configuration, the Size, Scale and
		 * SpectralRange attributes of each Spectrum are also
		 * checked.
		 */

		bool SetPrototypes(Spectrum& input,const Spectrum& out);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		void StoreOn(Storage &s) {};
	  bool Check(Spectrum& in,Spectrum& out);

	private:
	};

}

#endif // _SPECTRUM_J_TRANSFORM_H_
