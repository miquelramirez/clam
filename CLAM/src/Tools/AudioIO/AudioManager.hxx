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
#ifndef __AudioManager___
#define __AudioManager___

#include <vector>
#include <string>
#include "Err.hxx"


namespace CLAM{

class AudioDevice;
class AudioDeviceList;

/** This class takes care of all the creation of the registration of 
 * AudioIn and AudioOut objects, and the creation of AudioDevice objects required. 
 * It's a singleton class (only must be an instantiation of this class) because and unique object manages all the Devices, AudioOut and AudioIn objects provided by the system
 * @see AudioIn, AudioOut, AudioDevice
 */
class AudioManager
{
	friend class AudioIn;
	friend class AudioOut;
private:
	int mSampleRate, mLatency;
	unsigned mInternalBuffersNumber;
	std::vector<AudioDevice*> mDevices;
	std::vector<AudioDeviceList*> mDeviceLists;
public:
	typedef std::vector<AudioDevice*>::const_iterator device_iterator;
	typedef std::vector<AudioDeviceList*>::const_iterator list_iterator;

	static AudioManager* pSingleton;

	/** Constructor of the class. It receives as parameter the sample rate and
	 * latency
	 *  @param sampleRate The sample rate that will be given to the
	 *  AudioManager, and passed to the AudioDevice(s). The default is 48000.
	 *  @param latency The latency that will be given to the
	 *  AudioManager, and passed to the AudioDevice(s). The default is 512.
	 */

	AudioManager(int sampleRate = 48000, int latency = 512) throw(Err);

	/** Destructor of the class*/
	~AudioManager();

	/** Find a created AudioDevice, or NULL when not found 
	 *  @param name The name of the AudioDevice we want to get
	 *  @return the AudioDevice if it exists, or NULL otherwise
	 */
	AudioDevice* FindDevice(const std::string& name);

	/** Find a created AudioDevice, or create it when not found
	 *  @param name The name of the AudioDevice we want to get
	 *  @return the AudioDevice if it exists; otherwise method will create and return a new AudioDevice
	 */
	AudioDevice* FindOrCreateDevice(const std::string& name);

	/** This method starts the AudioManager object*/
	void Start(void) throw(Err);

	/** Getter to obtain the sample rate of the AudioManager
	 *  @return the sample rate
	 */
	int SampleRate(void) { return mSampleRate; }

	/** Getter to obtain the latency of the AudioManager
	 *  @return the latency
	 */
	int Latency(void) { return mLatency; }

	/** Setter to set the number of internal buffers
	 *  used internally by DirectX. By default it is 8
	 */

	void SetInternalBuffersNumber( unsigned numbuffs)
	{
		mInternalBuffersNumber = numbuffs;
	}

	/** Getter to set the number of internal buffers
	 *  used internally by DirectX.
	 *  @return the number of internal buffers 
	 */

	unsigned GetInternalBuffersNumber() const
	{
		return mInternalBuffersNumber;
	}

	/** Method to control that only exists one AudioManager instantiation
	 *  @return the pointer to the AudioManager object
	 *  @throw Error if is not an AudioManager initialized yet
	 */		
	static AudioManager& Singleton(void) throw(Err)
	{ 
		if (pSingleton) return *pSingleton;
		else throw Err("No AudioManager initialized");
	}

 	/** Retrieve the list of devices available for a given architecture. You can then use the AvailableDevices() method to retrieve a list of the available devices for each AudioDeviceList.
	 *  @param arch The name of architecture wich will be returned the devices. By default is set to "default"
	 *  @return The list of AudioDevices
	 */
	AudioDeviceList* FindList(const std::string& arch = "default");

	/** Global iterator interface for device lists. It can be used
	 *	to obtain the device list for each existent architecture.
	 *  @return The beginning list iterator
	 */
	list_iterator lists_begin() const {return mDeviceLists.begin();}

	/** Global iterator interface for device lists. It can be used
	 *	to obtain the device list for each existent architecture.
	 *  @return The ending list iterator
	 */
	list_iterator lists_end() const {return mDeviceLists.end();}

	/** Iterator interface for used audio devices. It will iterate through the list of devices which have been registered.
	 *  @return The beginning list iterator
	 */
	device_iterator used_devices_begin() const {return mDevices.begin();}
	/** Iterator interface for used audio devices. It will iterate through the list of devices which have been registered.
	 *  @return The ending list iterator
	 */
	device_iterator used_devices_end() const {return mDevices.end();}

protected:
	/** Register an AudioIn object. This is done by the AudioIn object itself
	*/
	bool Register(AudioIn& in);
	/** Register an AudioOut object. This is done by the AudioOut object itself
	*/
	bool Register(AudioOut& out);
};

};//CLAM

#endif
