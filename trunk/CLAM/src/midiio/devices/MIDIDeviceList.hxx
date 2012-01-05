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

#ifndef __MIDIDeviceList__
#define __MIDIDeviceList__

#include "MIDIDevice.hxx"

namespace CLAM{

/** This abstract class allows you to retrieve a list of all available
 * devices (strings) for a certain architecture. An singleton instance 
 * of each concrete MIDIDevice class for a specific architecture must be
 * created, that fills the list mAvailableDevices on construction.
 * The MIDIManager verifies the existence of lists for the 
 * possible architectures
 * <p> 
 * To retrieve a list of all MIDIDeviceList's available, call 
 * AudioManager::FindList
 * @see MIDIDevice, MIDIManager
 */
class MIDIDeviceList
{
friend class MIDIManager;
private:
	std::string mArch;
protected:
	std::vector<std::string> mAvailableDevices;

	void AddMe(void);
public:
	typedef std::vector<MIDIDeviceList*>::const_iterator list_iterator;
protected:
	/** Constructor of the MIDIDeviceList class, with a string as parameter that contains the name of the concrete architecture by this list.
	 *  @param arch The name of architecture (i.e. ALSA)
	 */
	MIDIDeviceList(const std::string& arch);

	/** Destructor of the class*/
	virtual ~MIDIDeviceList();

	/** Getter to obtain the name of the architecture related to this MIDIDeviceList
	 */
	const std::string& ArchName() {return mArch;}
		
	/** Pure virtual function that the concrete MIDIDeviceList class implements to create a concrete MIDIDevice
	 *  @param name A string with the name of new device
	 *  @param device A string with the type of device
	 *  @return The MIDIDevice created
	 */
	virtual MIDIDevice* Create(
			const std::string& name,const std::string& device) = 0;

	/** Global iterator interface for device lists. It can be used
	 *	to obtain the device list for each existent architecture.
	 *  @return The beginning list iterator
	 */
//		static list_iterator lists_begin() {return sDeviceLists.begin();}

	/** Global iterator interface for device lists. It can be used
	 *	to obtain the device list for each existent architecture.
	 *  @return The ending list iterator
	 */
//		static list_iterator lists_end()   {return sDeviceLists.end();}
public:
	/** This method is useful to obtain a complete list of the devices available. This list is created with information of operating system, sound architecture, etc.
	 *  @return A list of strings containing the names of the Devices
	 */
	const std::vector<std::string>& AvailableDevices(void)
	{
		return mAvailableDevices;
	}

	/** Pure virtual function that the concrete MIDIDeviceList class implements to get the name of the default device
	 *  @return A string with the name of default device  
	 */
	virtual std::string DefaultDevice(void) = 0;
};

};//CLAM

#endif

