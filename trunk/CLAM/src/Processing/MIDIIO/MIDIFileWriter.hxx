#ifndef __MIDIFILEWRITER__
#define __MIDIFILEWRITER__

#include <list>
#include "InPort.hxx"
#include "MIDIMelody.hxx"
#include "MIDITypes.hxx"
#include "MIDIFileIOConfig.hxx"
#include "Processing.hxx"

namespace CLAM
{
	class MIDIFileWriter : public Processing
	{
	public:
		MIDIFileWriter();
		MIDIFileWriter(const MIDIFileIOConfig& cfg);
		~MIDIFileWriter();

		bool Do();
		bool Do(const MIDISong& in);

		const char * GetClassName() const {return "MIDIFileWriter";}

		inline const ProcessingConfig &GetConfig() const { return mConfig;}
		bool ConcreteConfigure(const ProcessingConfig& c);
	   
	private:
		MIDIFileIOConfig mConfig;
		InPort<MIDISong> mInput;
		std::list<MIDIEvent> mEventList;
	  
		void BuildEventList(const MIDITrack& trk);
	};
}

#endif

