#ifndef _ZeroPadding_
#define _ZeroPadding_

#include "Array.hxx"
#include "Audio.hxx"
#include "DataTypes.hxx"
#include "Defines.hxx"
#include "DynamicType.hxx"
#include "ProcessingObject.hxx"
#include "Storage.hxx"

#include "config.h"



namespace MTG {

  class ZeroPadding;

  class ZeroPaddingConfig: public MTG::ProcessingObjectConfig
	{
	public:
    REG_NUM_ATTR_USING_INTERFACE (ZeroPaddingConfig, 5, MTG::ProcessingObjectConfig);
		REGISTER (0, Name, std::string);
		REGISTER (1, InSize, TInt32);
		REGISTER (2, OutSize, TInt32);
    REGISTER (3, ForcePowerOfTwo, bool);
		REGISTER (4, ZeroPhaseUsed, bool);
    
		void Init();
	};

 
  class ZeroPadding: public ProcessingObject 
  {
	protected:	
		TInt32 CalculatePowerOfTwo(TInt32 size);
		
    ZeroPaddingConfig mConfig;

		bool Do(TData *pinputBuffer, TInt32 inSize, TData *poutputBuffer, TInt32 outSize);

  private:
		const char *GetClassName() const {return "ZeroPadding";}

		/** Configuration change method
		 * @throw
		 * bad_cast exception when the argument is not an FFTConfig
		 * object.  
		 */
    bool ConcreteConfigure(const ProcessingObjectConfig&) throw(std::bad_cast);

	public:

		/* configuration methods */

		ZeroPadding(void);

		ZeroPadding(const ZeroPaddingConfig &c);

	  ~ZeroPadding();

		const ProcessingObjectConfig &GetConfig() const { return mConfig;}

		/* do methods */

		bool Do(void);

		/** Do method
			* Applies the window function buffer to an input signal buffer and stores the
			* the result in an output signal buffer. Processing in two different modes is 
			* possible, normal and zero-phase.
			*
		  *		in:     signal input buffer
			*		out:    signal output buffer
			*   window: window function buffer
			*/
		bool Do(Audio &in, Audio &out);

		bool Do(Audio &in);

		bool Do(Array<TData> &buffer);

		bool Do(Array<TData> &inputBuffer, Array<TData> &outputBuffer);



		/* prototype methods */

    bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		/* storage */

    //void StoreOn(Storage &s) {};

	private:
	};
} //end of namespace MTG


#endif // _ZeroPadding_
