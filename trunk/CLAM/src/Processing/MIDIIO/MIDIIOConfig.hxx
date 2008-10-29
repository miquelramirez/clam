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

#ifndef __MIDIIOConfig__
#define __MIDIIOConfig__

#include "ProcessingConfig.hxx"

namespace CLAM
{

/** Configuration of the MIDIIn and MIDIOut classes. You can specify
 * a device (string), which will be used by the MIDIManager, when creating
 * the MIDIDevice objects. When you don't specify a concrete device, or
 * specify "default", the MIDIManager will choose the default device for your
 * setup.
 * @see MIDIIn, MIDIOut, MIDIDevice, MIDIManager, ProcessingConfig
 */
class MIDIIOConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (MIDIIOConfig, 4, ProcessingConfig);

	/** The Device that will be used by this MIDIIn/Out instantiation, if it 
	 *  is not indicated, or its value is "default:default", system will
	 *  choose the best device for this object
	 **/
	DYN_ATTRIBUTE (0, public, std::string, Device);

	/** The Message attribute allows you to create a MIDIOut that sends midi
	 *  messages of a certain type. */
	DYN_ATTRIBUTE (1, public, unsigned short, Message);

	/** The Channel attribute allows you to create a MIDIOut that sends midi
	 *  messages on a certain channel. If you leave it 0 (default), a
	 *  FloatInControl will be created to control the channel of each MIDI message.
	 **/
	DYN_ATTRIBUTE (2, public, unsigned short, Channel);

	/** The Message attribute allows you to create a MIDIOut that sends midi
	 *  messages with a certain first data byte (typically usefull for
	 *  control messages). If you leave it 128 (default), a FloatInControl will
	 *  be created to control the first data byte. */
	DYN_ATTRIBUTE (3, public, unsigned short, FirstData);

protected:
	void DefaultInit(void)
	{
		AddDevice();
		AddMessage();
		AddChannel();
		AddFirstData();

		UpdateData();

		SetDevice("default:default");
		SetMessage(0);
		
		/** For MIDIIn   0 means: listen to all channels, and add an OutControl.
		 *  For MIDIOut  0 means: channel is controlled by a FloatInControl
		 **/
		SetChannel(0);
		
		/** For MIDIIn   128 means: listen to all first data bytes, and add an 
		 *                          OutControl
		 *  For MIDIOut  128 means: first data byte is controlled by a FloatInControl
		 **/
		SetFirstData(128); 
	}
};

} // namespace CLAM

#endif // __MIDIIOConfig__

