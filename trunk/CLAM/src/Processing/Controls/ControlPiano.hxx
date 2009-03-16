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
#include "TypedInControl.hxx"
#include "TypedOutControl.hxx"

#include "MIDIMessage.hxx"

namespace CLAM
{

class ControlPiano : public Processing
{
public:
	const char *GetClassName() const { return "ControlPiano"; }

protected:
	/** Controls **/
	TypedInControl< MIDI::Message > mInputMIDIMessage;
	TypedOutControl< MIDI::Message > mOutputMIDIMessage;

public:

	ControlPiano(const Config & config=Config())
		:
		mInputMIDIMessage("MIDI Message In", this, &ControlPiano::DoCallback),
		mOutputMIDIMessage("MIDI Message Out", this)
	{
		Configure( config );

		_notes.resize(12); // one octave
		for(unsigned int i=0;i<_notes.size();i++) { _notes[i]=false; }
	}

	~ControlPiano()
	{
	}

	bool Do() { return true; }

	void DoCallback(MIDI::Message inMessage)
	{
		inMessage = mInputMIDIMessage.GetLastValue();

		mOutputMIDIMessage.SendControl(inMessage); //delivers the same input message to the output

		MIDI::Message voidMessage;
		if (inMessage==voidMessage) return;

		if (inMessage[1]>=21 && inMessage[1]<=108)
		{
			if (inMessage[0]==144) //note on
			{
				_notes[(inMessage[1]-21)%12]=true;
			}
			else if (inMessage[0]==128) //note off
			{
				_notes[(inMessage[1]-21)%12]=false;
			}
		}
	};

	void SendMIDIMessage(MIDI::Message &msg)
	{
		mOutputMIDIMessage.SendControl(msg);
	}

	bool SetNoteStatus(unsigned int i)
	{
		return _notes[i];
	}

	bool GetNoteStatus(unsigned int i)
	{
		return _notes[i];
	}

protected:
	std::vector<bool> _notes; ///< Enabled/Disabled. Size: 12. One octave, from A to G#
};

}

#endif
