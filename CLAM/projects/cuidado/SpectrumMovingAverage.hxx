#ifndef _SPECTRUM_MOVING_AVERAGE_H_
#define _SPECTRUM_MOVING_AVERAGE_H_

#include "ProcessingObject.hxx"
#include "DynamicType.hxx"
#include "Spectrum.hxx"
#include "Audio.hxx"
#include "WindowGenerator.hxx"

namespace MTG {


	class SpectrumMovingAverageConfig: public ProcessingObjectConfig
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (SpectrumMovingAverageConfig, 2,ProcessingObjectConfig);
		REGISTER (0, Name, std::string);

		void DefaultInit();

	};

  /**
   *   This is just a template to which can be used to start a new Processing Object.
   *   Just copy it to another fiel and replace the  SpectrumMovingAverage with your  own 
   *   class name.
   *
   */


	class SpectrumMovingAverage: public ProcessingObject {
		SpectrumMovingAverageConfig mConfig;
	  WindowGeneratorConfig* mWinGenConf;
	  WindowGenerator*       mWinGen;
	  DataArray             mArray;
		const char *GetClassName() {return "SpectrumMovingAverage";}

		/** Config change method
		 * @throw
		 * bad_cast exception when the argument is not an SpecAdderConfig
		 * object.
		 */
		bool ConcreteConfigure(const ProcessingObjectConfig&) throw(std::bad_cast);

	public:
		SpectrumMovingAverage();

		SpectrumMovingAverage(const SpectrumMovingAverageConfig &c);

	        ~SpectrumMovingAverage();

		const SpectrumMovingAverageConfig &GetConfig() const { return mConfig;}

		void Start(void);
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
	  TSize Bark(TSize num);
	private:
	};

}

#endif // _SPECTRUM_MOVING_AVERAGE_H_
