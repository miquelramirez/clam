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

#ifndef __MIDIOUTCONTROL__
#define __MIDIOUTCONTROL__

#include "MIDIEnums.hxx"
#include "Array.hxx"
#include "MIDIOut.hxx"
#include "MIDIIOConfig.hxx"

namespace CLAM {

class MIDIOutControl:public MIDIOut
{
	private:
		Array<InControl*> mMyInControls;

		/* keep a unique id for each message we are constructing,
		 * to make sure we have each controlled byte only once
		 */
		unsigned char mUniqId;
		
		/* buffer to store the message */
		unsigned char* mMessage;
		unsigned char  mStatusByte;

		/* buffer to store the uniq id for each controlled byte */
		unsigned char* mReceivedUniqId;
		
		/* table to map InControl ID to byte-in-message index */
		unsigned char* mControlIdToMsgByteId;

		/* size of the midi message in bytes */
		unsigned char mMessageSize;

		/* number of bytes in the message that are controlled, and the
		 * number of of those that have been received for the current 
		 * message construction (current unique ID)
		 * (when these are equal the message has been constructed fully)
		 */
		unsigned char mControlledBytes;
		unsigned char mControlsReceived;
		
		void InitMembers(void);

		void Handle(unsigned char* msg,int size);
		
		/* callback function for each received control value */
		int DoControl(int id,TControlData val);
	public:
		MIDIOutControl();
		MIDIOutControl(const MIDIIOConfig &c);

		const char * GetClassName() const {return "MIDIOutControl";}

		bool ConcreteConfigure(const ProcessingConfig& c)
			throw(ErrProcessingObj);
};

} // namespace CLAM

#endif // MIDIOutControl.hxx

