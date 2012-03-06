/*
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

#ifndef _ControlPiano_
#define _ControlPiano_

#include "Processing.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"

#include "MIDIMessage.hxx"

namespace CLAM
{

class ControlPiano : public Processing
{
public:
	const char *GetClassName() const { return "ControlPiano"; }

protected:
	/** Controls **/
	InControl< MIDI::Message > mInputMIDIMessage;
	OutControl< MIDI::Message > mOutputMIDIMessage;

public:

	ControlPiano(const Config & config=Config())
		:
		mInputMIDIMessage("MIDI Message In", this, &ControlPiano::DoCallback),
		mOutputMIDIMessage("MIDI Message Out", this)
	{
		Configure( config );

		_notes.resize(12); // one octave
		for(unsigned int i=0;i<_notes.size();i++) { _notes[i]=false; }

		_velocity = 90; // fixed velocity. TODO: add it as a processing configuration parameter.
		_octave = 4; // fixed octave (C4). TODO: add it as a processing configuration parameter.
	}

	~ControlPiano()
	{
	}

	bool Do() { return true; }

	void DoCallback(const MIDI::Message & inMessage)
	{
		MIDI::Message message = inMessage;
		
		mOutputMIDIMessage.SendControl(message); //delivers the same input message to the output

		MIDI::Message voidMessage;
		if (message==voidMessage) return;

		if (message[1]>=21 && message[1]<=108)
		{
			if (message[0]==144) //note on
			{
				_notes[(message[1]-21)%12]=true;
			}
			else if (message[0]==128) //note off
			{
				_notes[(message[1]-21)%12]=false;
			}
		}
	};

	void SendMIDIMessage(MIDI::Message &msg)
	{
		mOutputMIDIMessage.SendControl(msg);
	}

	void SetNoteStatus(TSize i, bool state)
	{
		_notes[i] = state;
	}

	bool GetNoteStatus(TSize i)
	{
		return _notes[i];
	}

	TSize GetVelocity()
	{
		return _velocity;
	}

	TSize GetOctave()
	{
		return _octave;
	}

protected:
	std::vector<bool> _notes; ///< Enabled/Disabled. Size: 12. One octave, from A to G#
	TSize _velocity;
	TSize _octave;
};

}

#endif
