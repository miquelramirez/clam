#include "MIDIManager.hxx"
#include "MIDIInControl.hxx"
#include "MIDIOutControl.hxx"
#include "MIDIClocker.hxx"
#include "OutControl.hxx"
#include <vector>

using namespace CLAM;

void ConfigureAndCheck(Processing& p,ProcessingConfig& cfg)
{
	CLAM_ASSERT( p.Configure(cfg), p.GetConfigErrorMessage().c_str() );
}

main()
{
	char* indevice = "file:test.mid";
	char* outdevice = "textfile:test.txt";
	
	MIDIManager manager;

	MIDIClockerConfig inpClockerCfg;
	inpClockerCfg.SetDevice(indevice);
	MIDIClocker inpClocker(inpClockerCfg);

	MIDIClockerConfig outClockerCfg;
	outClockerCfg.SetDevice(outdevice);
	MIDIClocker outClocker(outClockerCfg);

	MIDIIOConfig inNoteCfg;
	inNoteCfg.SetDevice(indevice);
	inNoteCfg.SetMessage(MIDI::eNoteOnOff);
	MIDIInControl inNote;
	ConfigureAndCheck(inNote,inNoteCfg);

	MIDIIOConfig outNoteCfg;
	outNoteCfg.SetDevice(outdevice);
	outNoteCfg.SetMessage(MIDI::eNoteOnOff);
	MIDIOutControl outNote;
	ConfigureAndCheck(outNote,outNoteCfg);


	//control for stoping at eof 

	MIDIIOConfig inStopCfg;
	inStopCfg.SetDevice(indevice);
	inStopCfg.SetChannel(CLAM::MIDI::eStop); //it is a sys message that uses channel byte for actual data
	inStopCfg.SetMessage(CLAM::MIDI::eSystem);
	MIDIInControl inStop;
	ConfigureAndCheck(inStop,inStopCfg);

	InControl stopReceiver("stop-receiver");

	inStop.GetOutControls().GetByNumber(0).AddLink(
			&stopReceiver);

	CLAM::ConnectControls(inNote,0, outNote, 0);
	CLAM::ConnectControls(inNote,1, outNote, 1);
	CLAM::ConnectControls(inNote,2, outNote, 2);

	manager.Start();

	TTime curTime = 0;

	while (stopReceiver.GetLastValue()==0)
	{
		//we send a timing control to the MIDI clocker 
		inpClocker.GetInControls().GetByNumber(0).DoControl(curTime);
		outClocker.GetInControls().GetByNumber(0).DoControl(curTime);

		//we check for new events in the MIDI manager
		manager.Check();

		//we increment the time counter
		curTime ++;
	}
}
