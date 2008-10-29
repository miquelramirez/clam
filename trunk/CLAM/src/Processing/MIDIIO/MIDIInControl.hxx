/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __MIDIINCONTROL__
#define __MIDIINCONTROL__

#include "MIDIEnums.hxx"
#include "Array.hxx"
#include "MIDIIn.hxx"

namespace CLAM {

class MIDIInControl:public MIDIIn
{
private:
	Array<FloatOutControl*> mMyOutControls;

	int mMessageSize;
	int mControllingBytes;
	
	unsigned char* mMsgByteIdToControlId;

public:
	MIDIInControl();
	MIDIInControl(const MIDIIOConfig &c);
	const char * GetClassName() const {return "MIDIInControl";}

	bool ConcreteConfigure(const ProcessingConfig& c)
		throw(ErrProcessingObj);

private:
	void Handle(unsigned char* msg,int size);
};

} // namespace CLAM

#endif // MIDIInControl.hxx

