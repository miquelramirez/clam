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

#include "MIDIDeviceList.hxx"
#include "MIDIManager.hxx"

namespace CLAM
{
	MIDIDeviceList *sAlsaMIDIDeviceList = 0;
	MIDIDeviceList *sPortMidiMIDIDeviceList = 0;
	
	MIDIDeviceList::MIDIDeviceList(const std::string& arch)
		:mArch(arch)
	{
	}
	
	MIDIDeviceList::~MIDIDeviceList()
	{
	}
	
	MIDIDeviceList* MIDIManager::FindList(const std::string& arch)
	{
		unsigned int i;
		std::string tmp = arch;
		
		if (tmp == "default")
			tmp = DEFAULT_MIDI_ARCH; 
		
		/** Finding available devices for architecture arch
		 */
		for (i=0;i<Singleton().mDeviceLists.size();i++)
		{
			if (Singleton().mDeviceLists[i]->ArchName() == tmp)
			{
				return Singleton().mDeviceLists[i];
			}
		}
		
		return 0;
	}
	
}
