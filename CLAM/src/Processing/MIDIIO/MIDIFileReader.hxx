#ifndef __MIDIFILEREADER__
#define __MIDIFILEREADER__

#include "OutPort.hxx"
#include "MIDITypes.hxx"
#include "MIDIFileIOConfig.hxx"
#include "Processing.hxx"

namespace CLAM
{
	class MIDIFileReader : public Processing
	{
	public:
		MIDIFileReader();
		MIDIFileReader(const MIDIFileIOConfig& cfg);
		~MIDIFileReader();

		bool Do();
		bool Do(MIDISong& out);

		const char * GetClassName() const {return "MIDIFileReader";}

		inline const ProcessingConfig &GetConfig() const { return mConfig;}
		bool ConcreteConfigure(const ProcessingConfig& c);
	   
	private:
		MIDIFileIOConfig  mConfig;
		OutPort<MIDISong> mOutput;

		std::string GetEventName(TMIDIByte b);
	};
}

#endif

