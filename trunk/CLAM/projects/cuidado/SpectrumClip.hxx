#ifndef _SPECTRUMCLIP_H_
#define _SPECTRUMCLIP_H_

#include "ProcessingObject.hxx"
#include "DynamicType.hxx"
#include "Spectrum.hxx"
#include "Audio.hxx"

namespace MTG {


	class SpectrumClipConfig: public ProcessingObjectConfig
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (SpectrumClipConfig, 3,ProcessingObjectConfig);
		REGISTER (0, Name, std::string);
		REGISTER (1, Lower, TData);
		REGISTER (2, Upper, TData);

		void DefaultInit();

	};

  /**
   *   This is just a template to which can be used to start a new Processing Object.
   *   Just copy it to another fiel and replace the  SpectrumClip with your  own 
   *   class name.
   *
   */


	class SpectrumClip: public ProcessingObject {
		SpectrumClipConfig mConfig;

		const char *GetClassName() {return "SpectrumClip";}

		/** Config change method
		 * @throw
		 * bad_cast exception when the argument is not an SpecAdderConfig
		 * object.
		 */
		bool ConcreteConfigure(const ProcessingObjectConfig&) throw(std::bad_cast);

	public:
		SpectrumClip();

		SpectrumClip(const SpectrumClipConfig &c);

	        ~SpectrumClip();

		const SpectrumClipConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Check(Spectrum& in1, Spectrum& in2,const Spectrum& out);

		bool Do(Spectrum& in1, Spectrum& in2, Spectrum& out);

		// Port interfaces.

		/** Change the internal type state.  
		 * Apart from prototype configuration, the Size, Scale and
		 * SpectralRange attributes of each Spectrum are also
		 * checked.
		 */

		bool SetPrototypes(Spectrum& inp, Spectrum& in2,const Spectrum& out);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		void StoreOn(Storage &s) {};

	private:
	};

}

#endif // _SPECTRUMCLIP_H_
