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

#include "MIDIManager.hxx"
#include "MIDIDevice.hxx"
#include "MIDIDeviceList.hxx"
#include "MIDIIn.hxx"
#include <algorithm>
using std::find ;

using namespace CLAM;

MIDIManager* MIDIManager::pSingleton = 0;

MIDIDeviceList* sAlsaMIDIDeviceList = 0;
MIDIDeviceList* sPortMidiMIDIDeviceList = 0;

MIDIManager::MIDIManager() throw(Err)
{
	if (pSingleton)
		throw Err("can only have one midimanager at a time");
	pSingleton = this;

	if (sAlsaMIDIDeviceList)
	{
		Singleton().mDeviceLists.push_back(sAlsaMIDIDeviceList);
	}
	if (sPortMidiMIDIDeviceList)
	{
		Singleton().mDeviceLists.push_back(sPortMidiMIDIDeviceList);
	}
}

MIDIManager::~MIDIManager()
{
	unsigned int i;
	for (i=0;i<mDevices.size(); i++)
	{
		delete mDevices[i];
	}
	pSingleton = 0;
}

MIDIDevice* MIDIManager::FindDevice(const std::string& name)
{
	unsigned int i;

	/** Finding a created device
	*/
	for (i=0;i<mDevices.size();i++)
	{
		if (mDevices[i]->mName == name)
		{
			return mDevices[i];
		}
	}

	return 0;
}

void MIDIManager::Start(void) throw(Err)
{
	unsigned int i;

	/** Starting all the created devices
	*/
	for (i=0;i<mDevices.size();i++)
	{
		mDevices[i]->Start();
	}
}

void MIDIManager::Stop(void) throw(Err)
{
	unsigned int i;

	/** Stoping all the created devices
	*/
	for (i=0;i<mDevices.size();i++)
	{
		mDevices[i]->Stop();
	}
}


void MIDIManager::Check(void)
{
	unsigned int i;

	/** Force all created devices to read data
	*/
	for (i=0;i<mDevices.size();i++)
	{
		mDevices[i]->Read();
	}
}


MIDIDevice* MIDIManager::FindOrCreateDevice(const std::string& name)
{
	std::string arch =   name.substr(0,name.find(":",0));
	std::string device = name.substr(name.find(":",0)+1,name.size());

	if (arch == "" || device == "") 
	{
		std::string msg = "MIDIManager::FindOrCreateDevice(...): Invalid device name: ";
		msg       += name;
		throw Err(msg.c_str());
	}
	
	if (arch == "default")
	{
		arch = DEFAULT_MIDI_ARCH; 
	}

	/** Finding available devices for the architecture 
	*/
	MIDIDeviceList* list = FindList(arch);

	if (list==0)
	{
		std::string errstr;
		errstr = "MIDIManager::FindOrCreateDevice(): "
			"Don't have a list of \""+arch+"\" devices";
		throw Err((char*) errstr.c_str());
	}

	if (list->AvailableDevices().size()==0)
	{
		std::string errstr;
		errstr = "MIDIManager::FindOrCreateDevice(): "
			"Don't have any \""+arch+"\" devices available";
		throw Err((char*) errstr.c_str());
	}

	if (device == "default")
	{
		device = list->DefaultDevice();
	}

	/** Checking if the device is an avalaible device for the architecture
	*/
	if (find(list->AvailableDevices().begin(),
			 list->AvailableDevices().end(),
			 device) == 
		list->AvailableDevices().end())
	{
		std::string errstr;
		errstr = "MIDIManager::FindOrCreateDevice(): "
			"No device \""+device+"\" available in architecture \""+arch+"\".\n";
		throw Err((char*) errstr.c_str());
	}

	std::string real_name = arch+":"+device;


	/** Verifing if the device is just created
	*/
	MIDIDevice* mididevice = FindDevice(real_name);

	if (mididevice==0) {
		
		/** Creating a new device
		*/
		mididevice = list->Create(real_name,device);

		if (mididevice==0)
		{
			std::string errstr;
			errstr = "MIDIManager::FindOrCreateDevice(): Don't know how to make device "+real_name;
			throw Err((char*) errstr.c_str());
		}
		else
		{
			mDevices.push_back(mididevice);
		}
	}
	return mididevice;
}


bool MIDIManager::Register(MIDIIn& in)
{
	/** Getting the device for the MIDIIn object 
	 *  and registering the object as an input of the device
	 */
	MIDIDevice* device = FindOrCreateDevice(in.mConfig.GetDevice());
	return device->Register(in);
}
/*
bool MIDIManager::Register(MIDIOut& out)
{
	MIDIDevice* device = FindOrCreateDevice(out.mConfig.GetDevice());
	return device->Register(out);
}
*/

