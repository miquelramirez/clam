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
#include "MIDIIOConfig.hxx"
#include "MIDIOutControl.hxx"

#ifdef WIN32
	#include "windows.h"
#endif

using namespace CLAM;

void CrossPlatformSleep(int seconds)
{
	#ifdef WIN32
		Sleep(1000*seconds);
	#else 
		sleep(seconds);
	#endif
}

int main()
{
	MIDIManager manager;
	MIDIIOConfig outNoteCfg;

	// NOTE OUT EXAMPLE WITH FIXED CHANNEL`
	outNoteCfg.SetDevice("alsa:hw:1,0");
	outNoteCfg.SetChannel(1);

	outNoteCfg.SetMessage(MIDI::eNoteOn);

	MIDIOutControl outNote(outNoteCfg);
	
	manager.Start();
	
	SendFloatToInControl(outNote,0,60);
	SendFloatToInControl(outNote,1,120);

	CrossPlatformSleep(1);
	SendFloatToInControl(outNote,0,60);
	SendFloatToInControl(outNote,1,0);
	
	CrossPlatformSleep(1);

	// STOP HERE
	return 0;
	// NOTE OUT EXAMPLE WITH VARIABLE CHANNEL
	MIDIIOConfig outNote2Cfg;

	outNote2Cfg.SetDevice("default:default");
	outNote2Cfg.SetChannel(0); // means: create an in control!

	outNote2Cfg.SetMessage(MIDI::eNoteOn);

	MIDIOutControl outNote2(outNote2Cfg);
	
	SendFloatToInControl(outNote2,0,3);
	SendFloatToInControl(outNote2,1,60);
	SendFloatToInControl(outNote2,2,120);

	SendFloatToInControl(outNote2,0,3);
	SendFloatToInControl(outNote2,1,60);
	SendFloatToInControl(outNote2,2,120);

	// VOLUME CONTROL CHANGE EXAMPLE
	MIDIIOConfig volCtrlCfg;

	volCtrlCfg.SetDevice("default:default");
	volCtrlCfg.SetChannel(1);
	volCtrlCfg.SetFirstData(7); // 7 == volume ctrl change
	

	volCtrlCfg.SetMessage(MIDI::eControlChange);

	MIDIOutControl volCtrl(volCtrlCfg);

	// send value 110 to volume control change on channel 1
	SendFloatToInControl(volCtrl,0,110);
}

