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

#ifndef __MIDIManager__
#define __MIDIManager__

namespace CLAM{

/** This class takes care of all the creation of the registration of 
 *  MIDIIn and MIDIOut objects, and the creation of MIDIDevice objects required. 
 * It's a singleton class (only must be an instantiation of this class) because and unique object manages all the Devices, MIDIOut and MIDIIn objects provided by the system
 * @see MIDIIn, MIDIOut, MIDIDevice
 */
class MIDIManager
{
	friend class MIDIIn;
//		friend class MIDIOut;
private:
	std::vector<MIDIDevice*> mDevices;
	std::vector<MIDIDeviceList*> mDeviceLists;
public:
	typedef std::vector<MIDIDevice*>::const_iterator device_iterator;

	static MIDIManager* pSingleton;

	/** Constructor of the class. */
	MIDIManager() throw(Err);

	/** Destructor of the class*/
	~MIDIManager();

	/** Find a created MIDIDevice, or NULL when not found
	 *  @param name The name of the MIDIDevice we want to get
	 *  @return the MIDIDevice if it exists, or NULL otherwise
	 */
	MIDIDevice* FindDevice(const std::string& name);

	/** Find a created MIDIDevice, or create it when not found
	 *  @param name The name of the AudioDevice we want to get
	 *  @return the AudioDevice if it exists; otherwise method will create and return a new MIDIDevice
	 */
	MIDIDevice* FindOrCreateDevice(const std::string& name);

	/** This method starts the AudioManager object*/
	void Start(void) throw(Err);

	/** This method stops the AudioManager object*/
	void MIDIManager::Stop(void) throw(Err);

	/** Checks all devices searching data to read*/
	void Check(void);

	/** Method to control that only exists one MIDIManager instantiation
	 *  @return the pointer to the MIDIManager object
	 *  @throw Error if is not an MIDIManager initialized yet
	 */		
	static MIDIManager& Singleton(void) throw(Err)
	{ 
		if (pSingleton) return *pSingleton;
		else throw Err("No MIDIManager initialized");
	}

 	/** Retrieve the list of devices available for a given architecture. You can then use the AvailableDevices() method to retrieve a list of the available devices for each AudioDeviceList.
	 *  @param arch The name of architecture wich will be returned the devices. By default is set to "default"
	 *  @return The list of AudioDevices
	 */
	MIDIDeviceList* FindList(const std::string& arch = "default");

	/** Iterator interface for used midi devices. It will iterate
	 *  through the list of devices which have been registered.
	 *  @return The beginning list iterator
	 */
	device_iterator used_devices_begin() const {return mDevices.begin();}
	/** Iterator interface for used midi devices. It will iterate
	 *  through the list of devices which have been registered.
	 *  @return The ending list iterator
	 */
	device_iterator used_devices_end() const {return mDevices.end();}

protected:
	/** Register an MIDIIn object. This is done by the MIDIIn object itself
	*/
	bool Register(MIDIIn& in);
	/** Register an MIDIOut object. This is done by the MIDIOut object itself
	*/
//		bool Register(MIDIOut& out);
};




};//CLAM

#endif