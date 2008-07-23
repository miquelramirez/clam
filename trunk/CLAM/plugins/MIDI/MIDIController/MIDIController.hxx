#ifndef MIDIController_hxx
#define MIDIController_hxx

#include <CLAM/OutControl.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/Processing.hxx>
#include <bitset>

namespace CLAM {

	class MIDIControllerConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MIDIControllerConfig, 1, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, int, ControlNumber);

	protected:
		void DefaultInit(void)
		{
			AddAll();
			UpdateData();
			SetControlNumber(74);
		}

	};
	
	class MIDIController : public CLAM::Processing
	{ 
	protected:
		InControl mMIDIMessage;
		InControl mMIDIData1;
		InControl mMIDIData2;
		InControl mTrigger;

		OutControl mMIDIControllerValue;

		MIDIControllerConfig mConfig;
		
	public:
		const char* GetClassName() const { return "MIDIController"; }
		
		bool ConcreteConfigure(const ProcessingConfig& c);
		const ProcessingConfig &GetConfig() const { return mConfig;}
		
		MIDIController();
		~MIDIController();

		bool Do();
	};
	
} // End namespace
#endif // MIDIController_hxx
