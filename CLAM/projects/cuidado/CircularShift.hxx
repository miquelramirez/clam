#ifndef _CIRCULAR_SHIFT_H_
#define _CIRCULAR_SHIFT_H_

#include "ProcessingObject.hxx"
#include "DynamicType.hxx"
#include "Spectrum.hxx"
#include "Audio.hxx"




namespace MTG {

	class CircularShiftConfig: public ProcessingObjectConfig
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (CircularShiftConfig, 2,ProcessingObjectConfig);
		REGISTER (0, Name, std::string);
		REGISTER (1, Amount, TData);
		void DefaultInit();

	};

	class CircularShift: public ProcessingObject {
		CircularShiftConfig mConfig;

		const char *GetClassName() {return "CircularShift";}

		/** Config change method
		 * @throw
		 * bad_cast exception when the argument is not an SpecAdderConfig
		 * object.
		 */
		bool ConcreteConfigure(const ProcessingObjectConfig&) throw(std::bad_cast);

	public:
		CircularShift();

		CircularShift(const CircularShiftConfig &c);

		~CircularShift();

		const CircularShiftConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(DataArray& input, DataArray& out);
		bool Do(Spectrum& input, Spectrum& out);
		bool Do(Audio& input, Audio& out);

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
		void SetAmount(TSize amount) { mAmount.DoControl(amount); }

	private:
		InControl    mAmount;
	};

}

#endif // _CIRCULAR_SHIFT_H_
