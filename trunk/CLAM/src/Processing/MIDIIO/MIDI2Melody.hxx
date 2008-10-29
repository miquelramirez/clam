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

#ifndef __MIDI2Melody__
#define __MIDI2Melody__


#include "Control2Data.hxx"
#include "Array.hxx"
#include "MIDIMelody.hxx"
#include "OutPort.hxx"

namespace CLAM {

/** This processing is a particular control-to-data converter that takes in MIDI control events
 *	and converts them into a MIDIMelody processing data
 *	@see: Control2Data
 */
class MIDI2Melody:public Control2Data
{
public:
	MIDI2Melody();
	virtual  ~MIDI2Melody(){};
    
	/** Concrete output port of type MIDIMelody */
	OutPort<MIDIMelody> mOutput;
	/** Special input control for keeping trace of the time*/
	FloatInControl mTime;
    
protected:

	/** Particular implementation of this operation to generate data from enqueued controls*/
	bool GenerateOutputData(int id, TControlData val);
	/** Concrete configuration, we hardcode the number of controls to use and pass it to the
	 *	configuration of the base Control2Data configuration method.
	 *	@see Control2Data::ConcreteConfigure
	 */
	bool ConcreteConfigure(const ProcessingConfig& c);
	/** Finds the given note (according to key number) in internal container.
	 *	@return the index of the given note in internal container
	 */
	int FindNote(int key);
	/** Adds a note to internal container.
	 *	@param key note (MIDI key)
	 *	@param velocity is MIDI velocity
	 *	@param time is time in milliseconds
	 */
	void AddNote(int key, int velocity,int time);
	/** Deletes note from internal container using MIDI key */
	void DeleteNote(int key);
	/** Deletes note in given position in internal container */
	void DeleteNoteFromIndex(int index);
	/** Modifies midi velocity for given midi note*/
	void ModifyVelocity(int key,int newVelocity);
protected:
//protected member attributes and containers
	/** Contains an array of midi keys active at a given moment*/
	Array<int> mKeys;
	/** Array of Midi velocities corresponding to the active midi notes*/
	Array<int> mVelocities;
	/** Array of begin times in milliseconds corresponding to the active midi notes*/
	Array<int> mBeginTimes;
	/** Last key added to internal container*/
	int mLastKey;
    
  };

} // namespace CLAM

#endif

