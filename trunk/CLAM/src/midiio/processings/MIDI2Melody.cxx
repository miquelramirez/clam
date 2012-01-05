/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "MIDI2Melody.hxx"

namespace CLAM {

MIDI2Melody::MIDI2Melody()
	: mOutput("Output",this),
	  mTime("time",this)
{
	ConcreteConfigure(Control2DataConfig());
}
	
bool MIDI2Melody::GenerateOutputData(int id, TControlData val)
{
	switch(id)
	{
		case 0://key for note off
		{
			//find the note with that key in the member buffers, remove it, add it to melody after computing duration
			int index,velocity,beginTime,endTime;
			index=FindNote(int(val));
			if(index==-1) break; //I don't know why this happens but it does
			velocity=(int) mVelocities[index];
			beginTime=(int) mBeginTimes[index];
			endTime=(int) mTime.GetLastValue();
			DeleteNoteFromIndex(index);

			MIDINote newNote;
			newNote.SetKey(int(val));//should be MidiNote!
			newNote.SetVelocity(velocity);//should be MidiVelocity
			MediaTime time;
			time.AddBegin();
			time.AddEnd();
			time.RemoveDuration();
			time.UpdateData();
			time.SetBegin(beginTime);
			time.SetEnd(endTime);
			newNote.SetTime(time);
			mOutput.GetData().GetNoteArray().AddElem(newNote);

			break;
		}
		case 1://velocity for note off
		{
			//for the time being this will not be acknowledged
			break;
		}
		case 2://key for note on
		{
			//add note to member array with velocity 0 and modify mLastKey
			AddNote(int(val),0,int(mTime.GetLastValue()));
			mLastKey=int(val);
			break;
		}
		case 3://velocity for note on
		{
			//Modify velocity for mLastKey
			ModifyVelocity(mLastKey,int(val));
			break;
		}
		default:
		{
			CLAM_ASSERT(false,"Not a valid MIDI control");
		}
	}
	return true;
}


bool MIDI2Melody::ConcreteConfigure(const ProcessingConfig& c)
{
	//Ugly, ugly. Don't try this at home.
	Control2DataConfig &tmp=const_cast<Control2DataConfig&>(dynamic_cast<const Control2DataConfig&>(c));
	//We harcode this value as the concrete MIDI2Melody now will only work on 4 controls
	tmp.SetNumControls(4);
	Control2Data::ConcreteConfigure(tmp);
	return true;
}

int MIDI2Melody::FindNote(int key)
{
	int index=-1;
	int i;
	for(i=0;i<mKeys.Size();i++)
	{
		if (mKeys[i]==key){
		index=i;
		break;
		}
	}
	return index;
}

void MIDI2Melody::AddNote(int key, int velocity,int time)
{
	mKeys.AddElem(key);
	mVelocities.AddElem(velocity);
	mBeginTimes.AddElem(time);
}

void MIDI2Melody::DeleteNote(int key)
{
	DeleteNoteFromIndex(FindNote(key));
}

void MIDI2Melody::DeleteNoteFromIndex(int index)
{
	mKeys.DeleteElem(index);
	mVelocities.DeleteElem(index);
	mBeginTimes.DeleteElem(index);
}

void MIDI2Melody::ModifyVelocity(int key,int newVelocity)
{	
	int index=FindNote(key);
	mVelocities[index]=newVelocity;
}

} // namespace CLAM

