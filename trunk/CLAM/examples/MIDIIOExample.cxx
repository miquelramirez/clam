/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

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

int main()
{
	const char* indevice = "file:test.mid";
	const char* outdevice = "textfile:test.txt";
	
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

	inStop.GetOutControls().GetByNumber(0).AddLink(stopReceiver);

	CLAM::ConnectControls(inNote,0, outNote, 0);
	CLAM::ConnectControls(inNote,1, outNote, 1);
	CLAM::ConnectControls(inNote,2, outNote, 2);

	manager.Start();

	TTime curTime = 0;

	while (stopReceiver.GetLastValue()==0)
	{
		//we send a timing control to the MIDI clocker 
		SendFloatToInControl(inpClocker,0,curTime);
		SendFloatToInControl(outClocker,0,curTime);

		//we check for new events in the MIDI manager
		manager.Check();

		//we increment the time counter
		curTime ++;
	}
}

