/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "MIDIInControl.hxx"
#include "MIDIManager.hxx"
#include "PrintControl.hxx"
#include "ControlMapper.hxx"
#include "Oscillator.hxx"
#include "ControlMultiplier.hxx"
#include <iostream>

using namespace CLAM;
using namespace std;

int main(void)
{
	try {

		PrintControlConfig printCtrlCfg;
		printCtrlCfg.SetMessage("Control:");

		PrintControlConfig printKeyCfg;
		printKeyCfg.SetMessage("Note key");
		
		PrintControlConfig printVelCfg;
		printVelCfg.SetMessage("Note vel");

		PrintControlConfig printPichBendCfg;
		printPichBendCfg.SetMessage("Pitch Bend");
		
		PrintControl printCtrl(printCtrlCfg);
		PrintControl printKey(printKeyCfg);
		PrintControl printVel(printVelCfg);
		PrintControl printPitchBend(printPichBendCfg);


		ControlMapperConfig mapperCfg;
		
		mapperCfg.SetMapping("linear");
		TData ptr[]={0.0, 127.0, 0.0, 1.0};
		mapperCfg.SetArguments(DataArray(ptr,4));

		ControlMapper mapper(mapperCfg);



		ControlMapperConfig mapperNoteCfg ;

		mapperNoteCfg.SetMapping( "NoteToFreq" ) ;

		ControlMapper mapperNote(mapperNoteCfg);



		ControlMapperConfig mapperVelCfg ;

		mapperVelCfg.SetMapping("linear");
		TData ptr3[] = {0 ,127 ,0 ,1} ;
		mapperVelCfg.SetArguments(DataArray(ptr3,4));

		ControlMapper mapperVel(mapperVelCfg);



		ControlMapperConfig mapperPBendCfg ;

		mapperPBendCfg.SetMapping("ValueToRatio");
		TData ptr4[] = { 12 } ;							// 1 octave up or down
		mapperPBendCfg.SetArguments(DataArray(ptr4,1));

		ControlMapper mapperPitchBend(mapperPBendCfg);

		BinaryControlOpConfig multiplyCfg ;

		ControlMultiplier FreqMultiplier( multiplyCfg); 


		MIDIManager midiManager;

		MIDIInConfig inNoteCfg;

		
		inNoteCfg.SetDevice("default:default");
		inNoteCfg.SetChannelMask(
			MIDI::ChannelMask(1)|
			MIDI::ChannelMask(2)
		);

		inNoteCfg.SetMessageMask(
			MIDI::MessageMask(MIDI::eNoteOn)|
			MIDI::MessageMask(MIDI::eNoteOff)
		);
		
		MIDIInControl inNote(inNoteCfg);

		MIDIInConfig inCtrlCfg;
		
		inCtrlCfg.SetDevice("default:default");
		inCtrlCfg.SetChannelMask(MIDI::ChannelMask(1));
		inCtrlCfg.SetMessageMask(MIDI::MessageMask(MIDI::eControlChange));
		inCtrlCfg.SetFilter(0x02); // breath control
		
		MIDIInControl inCtrl(inCtrlCfg);

		MIDIInConfig inPitchBendCfg;
		
		inPitchBendCfg.SetDevice("default:default");
		inPitchBendCfg.SetChannelMask(MIDI::ChannelMask(1));
		inPitchBendCfg.SetMessageMask(MIDI::MessageMask(MIDI::ePitchbend));
		
		MIDIInControl inPitchBend(inPitchBendCfg);

		inNote.GetOutControls().GetByNumber(2).AddLink(&mapperNote.GetInControls().GetByNumber(0));
		inNote.GetOutControls().GetByNumber(3).AddLink(&mapperVel.GetInControls().GetByNumber(0));
		inNote.GetOutControls().GetByNumber(1).AddLink(&mapperVel.GetInControls().GetByNumber(0));
		inCtrl.GetOutControls().GetByNumber(0).AddLink(&mapper.GetInControls().GetByNumber(0));

		inPitchBend.GetOutControls().GetByNumber(0).AddLink(&mapperPitchBend.GetInControls().GetByNumber(0));


		mapperNote.GetOutControls().GetByNumber(0).AddLink(&printKey.GetInControls().GetByNumber(0));
		mapperNote.GetOutControls().GetByNumber(0).AddLink(&FreqMultiplier.GetInControls().GetByNumber(0));

		mapperVel.GetOutControls().GetByNumber(0).AddLink(&printVel.GetInControls().GetByNumber(0));
		
		mapper.GetOutControls().GetByNumber(0).AddLink(&printCtrl.GetInControls().GetByNumber(0));
		
		mapperPitchBend.GetOutControls().GetByNumber(0).AddLink(&FreqMultiplier.GetInControls().GetByNumber(1));
		
		FreqMultiplier.GetOutControls().GetByNumber(0).AddLink(&printPitchBend.GetInControls().GetByNumber(0));

		midiManager.Start();

		while (1)
		{
			midiManager.Check();
		}
	}
	catch(Err error)
	{
		error.Print();
		std::cerr << "Abnormal Program Termination" << std::endl;
	}

	std::cerr << "Successfully finished" << std::endl;
	return 0;
}
