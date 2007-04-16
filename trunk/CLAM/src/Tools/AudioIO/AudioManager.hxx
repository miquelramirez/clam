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
#ifndef __AudioManager___
#define __AudioManager___

#include <vector>
#include <string>
#include "Err.hxx"


namespace CLAM{

class AudioDevice;
class AudioDeviceList;
class AudioIn;
class AudioOut;

/** This class takes care of all the creation of the registration of 
 * AudioIn and AudioOut objects, and the creation of AudioDevice objects required. 
 * It's a singleton class (only must be an instantiation of this class) because and unique object manages all the Devices, AudioOut and AudioIn objects provided by the system
 * @see AudioIn, AudioOut, AudioDevice
 */
class AudioManager
{
	friend class AudioIn;
	friend class AudioOut;
	friend class AudioDeviceList;
private:
	int mSampleRate, mLatency;
	unsigned mInternalBuffersNumber;

	std::vector<AudioDevice*> mDevices;

	/** A Meyers-Singleton-style list of all DeviceList */
	static std::vector<AudioDeviceList*>& DeviceLists(void);
	
	static AudioManager* _Current(bool set = 0,AudioManager* m = 0)
	{
		static AudioManager* sCurrent = 0;
		if (set)
		{
			if (m)
			{
				if (sCurrent) throw Err("Can only have one AudioManager at a time");
			}
			sCurrent = m;
		}
		return sCurrent;
	}
public:
	typedef std::vector<AudioDevice*>::const_iterator device_iterator;
	typedef std::vector<AudioDeviceList*>::const_iterator list_iterator;

	/** Constructor of the class. It receives as parameter the sample rate and
	 * latency
	 *  @param sampleRate The sample rate that will be given to the
	 *  AudioManager, and passed to the AudioDevice(s). The default is 48000.
	 *  @param latency The latency that will be given to the
	 *  AudioManager, and passed to the AudioDevice(s). The default is 512.
	 */
	AudioManager(int sampleRate,int latency);

	/** Destructor of the class*/
	~AudioManager();

	static AudioManager& Current()
	{
		AudioManager* p = _Current();
		
		if (p==0) throw Err("No AudioManager current");
		
		return *p;
	}

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

	/** Setter to change the latency of the AudioManager
	 */
	void SetLatency(int latency) { mLatency = latency; }


 	/** Retrieve the list of devices available for a given architecture. You can then use the AvailableDevices() method to retrieve a list of the available devices for each AudioDeviceList.
	 *  @param arch The name of architecture wich will be returned the devices. By default is set to "default"
	 *  @return The list of AudioDevices
	 */
	AudioDeviceList* FindList(const std::string& arch = "default");

	/** Global iterator interface for device lists. It can be used
	 *	to obtain the device list for each existent architecture.
	 *  @return The beginning list iterator
	 */
	list_iterator lists_begin() const {return DeviceLists().begin();}

	/** Global iterator interface for device lists. It can be used
	 *	to obtain the device list for each existent architecture.
	 *  @return The ending list iterator
	 */
	list_iterator lists_end() const {return DeviceLists().end();}

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

