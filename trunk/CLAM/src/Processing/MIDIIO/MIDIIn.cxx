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


#ifdef WIN32
#define DEFAULT_MIDI_ARCH "portmidi"
#else
#define DEFAULT_MIDI_ARCH "alsa"
#endif

#include "MIDIIn.hxx"

using namespace CLAM;

bool MIDIIn::ConcreteConfigure(const ProcessingConfig& cfg)
	throw(ErrProcessingObj)
{
	try {
		mConfig = dynamic_cast<const MIDIInConfig&>(cfg);
	}
	catch (std::bad_cast) {
		throw(ErrProcessingObj("MIDIIn::ConcreteConfigure(): Wrong argument type.",this));
	}

	/** Unregistering a possible device already assigned to the MIDIIn object
	*/
	if (mpDevice)
		mpDevice->Unregister(*this);

	MIDIManager *m;

	/** Getting the MIDIManager object
	*/
	try {
		m = &MIDIManager::Singleton();
	}
	catch (Err &e) {
		ErrProcessingObj ne("MIDIIn::ConcreteConfigure(): No MIDIManager found.",this);
		ne.Embed(e);
		throw(ne);
	}

	/** Registering the MIDIIn object through the MIDIManager object
	*/
	bool res;
	try {
		res = m->Register(*this);
	}
	catch (Err &e) {
		ErrProcessingObj ne("MIDIIn::ConcreteConfigure(): Failed to register in MIDIManager.",this);
		ne.Embed(e);
		throw(ne);
	}

	if (res == false)
		throw ErrProcessingObj("MIDIIn::ConcreteConfigure(): "
			                   "Failed to register in MIDIManager.",this);

	return true;
}

bool MIDIIn::ConcreteStart(void) 
{
	if (!mpDevice)
		throw(Err("MIDIIn::ConcreteStart(): No Device found!"));
	/** Starting the MIDIDevice object assigned to the MIDIIn */		
	mpDevice->Start();
	return true;
}

void MIDIIn::GetDeviceInfo(MIDIDevice::TInfo &info) const
{
	if (mpDevice)
		mpDevice->GetInfo(info);
	else
		info.Reset();
}
