#include "MIDIEnums.hxx"
#include "Array.hxx"
#include "MIDIIn.hxx"

namespace CLAM {

	class MIDIInControl:public MIDIIn
	{
	public:
		Array<OutControl*> mOutControls;

		struct OutControlInfo
		{
			OutControlInfo(MIDI::Message msg,int field)
			{
				mMessage = msg;
				mField = field;
			}

			MIDI::Message mMessage;
			int mField;
		};

		Array<OutControlInfo*> mOutControlInfos;

		MIDIInControl();
		MIDIInControl(const MIDIInConfig &c);

		bool ConcreteConfigure(const ProcessingConfig& cfg)
			throw(ErrProcessingObj);

		void Handle(unsigned char* msg,int size);
	};

}
