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

#include "MIDIManager.hxx"
#include "MIDIDevice.hxx"
#include "MIDIDeviceList.hxx"
#include "MIDIIn.hxx"
#include "MIDIOut.hxx"
#include "MIDIClocker.hxx"
#include <algorithm>
using std::find ;


namespace CLAM
{

MIDIManager::MIDIManager() throw(Err)
:mStarted(false)
{
	_Current(true,this);
}

MIDIManager::~MIDIManager()
{
	std::vector<MIDIDevice*>::iterator it;

	it = mDevices.begin();
	
	while (it!=mDevices.end())
	{
		MIDIDevice* d = *it;

		it = mDevices.erase(it);
		delete d;
	}

	_Current(true,0);
}

MIDIDevice* MIDIManager::FindDevice(const std::string& name)
{
	/* Find a created device */
	std::vector<MIDIDevice*>::iterator it;

	for (it = mDevices.begin(); it!=mDevices.end(); it++)
	{
		if ((*it)->mName == name ) return *it;
	}
	return 0;
}

void MIDIManager::Start(void) throw(Err)
{
	std::vector<MIDIDevice*>::iterator it;

	it = mDevices.begin();
	
	/** Remove created but unused devices
	*/
	while (it!=mDevices.end())
	{
		MIDIDevice* d = *it;

		if (d->mInputs.size()==0 && 
		    d->mOutputs.size()==0)
		{
			it = mDevices.erase(it);
			delete d;
		}else{
			it++;
		}
	}	
	
	/** Starting all the created devices
	*/
	for (it = mDevices.begin(); it!=mDevices.end(); it++)
	{
		(*it)->Start();
	}

	mStarted = true;
}

void MIDIManager::Stop(void) throw(Err)
{
	std::vector<MIDIDevice*>::iterator it;

	/** Stoping all the created devices
	*/
	for (it = mDevices.begin(); it!=mDevices.end(); it++)
	{
		(*it)->Stop();
	}
	
	mStarted = false;
}


void MIDIManager::Check(void)
{
	CLAM_DEBUG_ASSERT(mStarted,"MIDIManager not started before calling MIDIManager::Check()\n");
	
	std::vector<MIDIDevice*>::iterator it;

	/** Force all created devices to read data
	*/
	for (it = mDevices.begin(); it!=mDevices.end(); it++)
	{
		(*it)->Read();
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

	std::vector<std::string>::const_iterator it;

	for (
		it = list->AvailableDevices().begin(); 
		it != list->AvailableDevices().end();
		it++)
	{
		if (*it == device) break;
		break;
	}
	
/*	if (find(list->AvailableDevices().begin(),
			 list->AvailableDevices().end(),
			 device) == 
		list->AvailableDevices().end())
*/
	if (it == list->AvailableDevices().end())
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
	return device->Register(this,in);
}

bool MIDIManager::Register(MIDIClocker& cl)
{
	/** Getting the device for the MIDIIn object 
	 *  and registering the object as an input of the device
	 */
	MIDIDevice* device = FindOrCreateDevice(cl.mConfig.GetDevice());
	return device->Register(this,cl);
}

bool MIDIManager::Register(MIDIOut& out)
{
	MIDIDevice* device = FindOrCreateDevice(out.mConfig.GetDevice());
	return device->Register(this,out);
}

MIDIDeviceList* MIDIManager::FindList(const std::string& arch)
{
	unsigned int i;
	std::string tmp = arch;

	if (tmp == "default")
		tmp = DEFAULT_MIDI_ARCH; 

	/** Finding available devices for architecture arch
	*/
	for (i=0;i<DeviceLists().size();i++)
	{
		if (DeviceLists()[i]->ArchName() == tmp)
		{
			return DeviceLists()[i];
		}
	}

	return 0;
}


}

