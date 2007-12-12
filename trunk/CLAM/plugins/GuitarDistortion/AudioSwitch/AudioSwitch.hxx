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

#ifndef _AudioSwitch_hxx__
#define _AudioSwitch_hxx__

#include <CLAM/Processing.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InControl.hxx>

namespace CLAM
{

class AudioSwitchConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (AudioSwitchConfig, 2, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, int, NumberOfInPorts);
	DYN_ATTRIBUTE (1, public, int, FrameSize);

protected:
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetNumberOfInPorts(2);
		SetFrameSize(512);
	}

};

/**	\brief Audio switcher
*
*	Switchs between a configurable amount of inputs (like a multiplexer)
*/
class AudioSwitch: public Processing
{
private:
	AudioSwitchConfig  mConfig;

	/** Ports **/
	std::vector< AudioInPort* > mInputPorts;
	AudioOutPort     mOutputPort;		

	/** Controls **/
	InControl mSwitch;

	void RemovePortsAndControls();
	void CreatePortsAndControls();

public:
	bool ConcreteConfigure(const ProcessingConfig& c);
	AudioSwitch();
	virtual ~AudioSwitch()
	{
		RemovePortsAndControls();
	}
	bool ModifiesPortsAndControlsAtConfiguration()
	{
		return true;
	}

	const char * GetClassName() const {return "AudioSwitch";}
	const ProcessingConfig &GetConfig() const { return mConfig;}
	bool Do();
};

} // namespace CLAM

#endif // __AudioSwitch_hxx__

