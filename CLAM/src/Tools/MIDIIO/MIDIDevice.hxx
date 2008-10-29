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

#ifndef __MIDIDevice__
#define __MIDIDevice__


#include <string>
#include <vector>
#include "Err.hxx"
#include "MIDIEnums.hxx"
#include "InControl.hxx"

namespace CLAM{

class MIDIIn;
class MIDIOut;
class MIDIClocker;
class MIDIManager;

/** This is the abstract base class for an midi device. With an midi device
 * we refer to any kind of midi input/output.
 * MIDIDevices are usually created by the MIDIManager. The interface 
 * for the user however, are the MIDIIn and MIDIOut classes.
 * <p>
 * Each MIDIIn and MIDIOut created is registered with the MIDIManager. When the MIDIManager is started, it will create the 
 * appropiate MIDIDevice or MIDIDevices, based on the number of inputs, 
 * outputs, operating system and MIDIInConfig of the MIDIIn objects
 * @see MIDIIn, MIDIOut, MIDIDeviceList, MIDIManager
 */
class MIDIDevice
{
	friend class MIDIIn;
	friend class MIDIOut;
	friend class MIDIOutControl;
	friend class MIDIClocker;
	friend class MIDIManager;
protected:
	/** This class is used to respond to device information
	 *  request using the GetInfo method.
	 */
	struct TInfo
	{
		std::string mName;
		std::string mArch;
		/** Base constructor, calls to Reset method to clear all the values*/
		TInfo() { Reset();}

		/** Clear all the values of object, setting strings mName and mArch to ""*/
		void Reset()
		{
			mName=mArch="";
		}
	};

	MIDIIn* target[8][16][128];

	std::vector<MIDIIn*> mInputs;
	std::vector<MIDIOut*> mOutputs;

	std::string mName;

	unsigned char mLastStatus;
	int mIndex;
	int mLength;
	unsigned char mData[256];

	/** Constructor of the class that sets the name of object to the string passed by parameter
	 *  @param name String with the name of object
	 */
	MIDIDevice(const std::string& name)
	{
		mName = name;
		mLastStatus = 0;
		mIndex = 0;
		mLength = 0;
		mMIDIManager = 0;
	}
	
	/** Destructor of class*/
	virtual ~MIDIDevice() { };

	/** This method should be called to begin the use of this Device*/
	void Start(void);

	/** This method must be called to end the use of this Device*/
	void Stop(void);
	
	/** This function is executed to start the concrete use of the devices. Each implementation of MIDIDevice must define this method.*/
	virtual void ConcreteStart(void) throw(Err) = 0;

	/** This function is executed to stop the concrete use of the devices. Each implementation of MIDIDevice must define this method.*/
	virtual void ConcreteStop(void) throw(Err) = 0;

	/** Getter for the Info of Device Object attached to this MIDIDevice instantiation
	 *  @param info TInfo object that method will modify with the values of Tinfo internal object
	 */
	void GetInfo(TInfo&);

	virtual void SetClock(TControlData val) { }

	/** Reads the information given by this Device and passes data to an Audio chunk. Must be implemented by any Device derived from this class. Not implemented yet
	 */
	virtual void Read(void) throw(Err) = 0;

	virtual void Write(unsigned char* msg,int size) throw(Err) = 0;

protected:
	bool Register(MIDIManager* mm,MIDIIn& in);
	bool Register(MIDIManager* mm,MIDIOut& out);
	void Unregister(MIDIIn& in);
	void Unregister(MIDIOut& out);

	bool Register(MIDIManager* mm,MIDIClocker& in);
	void Unregister(MIDIClocker& in);

	int GetMessageLength(unsigned char byte);

	void HandleRawByte(unsigned char byte);

private:
	MIDIManager* mMIDIManager;
	MIDIManager& _MIDIManager(void);
	void _SetMIDIManager(MIDIManager* mm);

	void SetTarget(
		MIDI::Message msg,
		unsigned char chn,unsigned char firstData,
		MIDIIn* inp);
};

};//CLAM

#endif

