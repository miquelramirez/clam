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

#ifndef _AudioMixer_hxx__
#define _AudioMixer_hxx__

#include "Processing.hxx"
#include "AudioOutPort.hxx"
#include "AudioInPort.hxx"
#include "Audio.hxx"
#include "InControl.hxx"

namespace CLAM
{

class AudioMixerConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (AudioMixerConfig, 2, ProcessingConfig);
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

class AudioMixer: public Processing
{
private:
	AudioMixerConfig  mConfig;

	std::vector< AudioInPort* > mInputPorts;
	AudioOutPort     mOutputPort;		
	std::vector< FloatInControl* > mInputControls;
	
	void RemovePortsAndControls();
	void CreatePortsAndControls();

public:
	bool ConcreteConfigure(const ProcessingConfig& c);
	AudioMixer();
	virtual ~AudioMixer()
	{
		RemovePortsAndControls();
	}
	bool ModifiesPortsAndControlsAtConfiguration()
	{
		return true;
	}

	const char * GetClassName() const {return "AudioMixer";}
	const ProcessingConfig &GetConfig() const { return mConfig;}
	bool Do();
};

} // namespace CLAM

#endif // __AudioMixer_hxx__

