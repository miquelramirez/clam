#ifndef _OverlapAdd_
#define _OverlapAdd_

#include "Array.hxx"
#include "Audio.hxx"
#include "AudioCircularBuffer.hxx"
#include "DataTypes.hxx"
#include "Defines.hxx"
#include "DynamicType.hxx"
#include "ProcessingObject.hxx"
#include "Storage.hxx"

#include "config.h"



namespace MTG {

  class OverlapAdd;

  class OverlapAddConfig: public MTG::ProcessingObjectConfig
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (OverlapAddConfig, 4, MTG::ProcessingObjectConfig);
		REGISTER (0, Name, std::string);
		REGISTER (1, HopSize, TInt32);
		REGISTER (2, FrameSize, TInt32);
		REGISTER (3, BufferSize, TInt32);
		void Init();
	};

 
  class OverlapAdd: public MTG::ProcessingObject 
  {
	protected:	
    OverlapAddConfig   mConfig;
		TInt32             mHopSize;

		
	private:
		const char *GetClassName() {return "OverlapAdd";}

		/** Configuration change method
		 * @throw
		 * bad_cast exception when the argument is not an FFTConfig
		 * object.  
		 */

    bool ConcreteConfigure(const ProcessingObjectConfig&) throw(std::bad_cast);

		/* protected methods */
	protected:


	public:

		/* configuration methods */
		OverlapAdd(void);

		OverlapAdd(const OverlapAddConfig &c);

	  ~OverlapAdd();

		const ProcessingObjectConfig &GetConfig() const { return mConfig;}


		/* do methods */

		bool Do(void);

		/** Do method
			*
			*/

		bool Do(DataArray &in, AudioCircularBuffer<TData> &overlapAddBuffer, DataArray &out);

		bool Do(Audio &in, AudioCircularBuffer<TData> &overlapAddBuffer, Audio &out);

		//bool Do(Audio &in, Audio &out, DataArray &window);

		/* prototype methods */

    bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		/* storage */

    //void StoreOn(Storage &s) {};

	};
} //end of namespace MTG


#endif // _OverlapAdd_