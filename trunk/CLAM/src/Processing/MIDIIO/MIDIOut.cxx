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

#include "MIDIOut.hxx"
#include "MIDIManager.hxx"

namespace CLAM {

bool MIDIOut::ConcreteConfigure(const ProcessingConfig& c)
	throw(ErrProcessingObj)
{
	CopyAsConcreteConfig(mConfig, c);
  
	/** Unregistering a possible device already associated with this
	 *  MIDIOut object
	 */
	if (mpDevice)
		mpDevice->Unregister(*this);

	MIDIManager *m;
  
	/** Getting the MIDIManager object
	*/
	try
	{
		m = &MIDIManager::Current();
	}
	catch (Err &e)
	{
		ErrProcessingObj ne("MIDIOut::ConcreteConfigure(): No MIDIManager found.",this);
		ne.Embed(e);
		throw(ne);
	} 
 
	/** Registering the MIDIOut object through the MIDIManager object
	*/
	bool res;
	try {
		res = m->Register(*this);
	}
	catch (Err &e) {
		ErrProcessingObj ne("MIDIOut::ConcreteConfigure(): Failed to register in MIDIManager.",this);
		ne.Embed(e);
		throw(ne);
	} 
 
	if (res == false)
		throw ErrProcessingObj("MIDIOut::ConcreteConfigure(): "
			                   "Failed to register in MIDIManager.",this);                   
	return true;
  
}

bool MIDIOut::ConcreteStart(void) 
{
	if (!mpDevice)
		throw Err("MIDIOut::ConcreteStart(): No Device found!");
	/** Starting the MIDIDevice object assigned to the MIDIOut */		
	mpDevice->Start();
	return true;
}

void MIDIOut::GetDeviceInfo(MIDIDevice::TInfo &info) const
{
	if (mpDevice)
		mpDevice->GetInfo(info);
	else
		info.Reset();
}

} // namespace CLAM

