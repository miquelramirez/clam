#include "MIDIManager.hxx"
#include "MIDIInControl.hxx"
#include "MIDIOutControl.hxx"
#include "MIDIClocker.hxx"
#include <vector>

using namespace CLAM;

main()
{
	MIDIManager manager;
	MIDIInConfig inNoteCfg;
	MIDIOutConfig outNoteCfg;

	MIDIDeviceList* list = manager.FindList("portmidi");
	const std::vector<std::string>& availableDevices = list->AvailableDevices();

	for (std::vector<std::string>::const_iterator it = availableDevices.begin();
			it != availableDevices.end();
			it++)
	{
		printf("%s\n",(*it).c_str());
	}
	
	inNoteCfg.SetDevice("portmidi:input:UART MIDI SB Live!");
	inNoteCfg.SetMessageMask(
			MIDI::MessageMask(MIDI::eNoteOn)|
			MIDI::MessageMask(MIDI::eNoteOff));
	inNoteCfg.SetChannelMask(MIDI::ChannelMask(-1));
	
	outNoteCfg.SetDevice("portmidi:output:Mapeador Microsoft MIDI");
	outNoteCfg.SetMessage(MIDI::eNoteOn);
	outNoteCfg.SetChannel(1);

	MIDIInControl inNote(inNoteCfg);
	MIDIOutControl outNote(outNoteCfg);
	
	inNote.GetOutControls().GetByNumber(0).AddLink(
			&outNote.GetInControls().GetByNumber(0));
	inNote.GetOutControls().GetByNumber(1).AddLink(
			&outNote.GetInControls().GetByNumber(1));
	inNote.GetOutControls().GetByNumber(2).AddLink(
			&outNote.GetInControls().GetByNumber(0));
	inNote.GetOutControls().GetByNumber(3).AddLink(
			&outNote.GetInControls().GetByNumber(1));
	
	manager.Start();

	while (1)
	{
		manager.Check();
	}
}
