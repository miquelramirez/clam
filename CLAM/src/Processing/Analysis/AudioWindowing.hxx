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

#ifndef __AudioWindowing__
#define __AudioWindowing__

#include "WindowGenerator.hxx"
#include "CircularShift.hxx"
#include "AudioWindowingConfig.hxx"
#include "Audio.hxx"
#include "AudioMultiplier.hxx"
#include "ProcessingComposite.hxx"
#include "AudioInPort.hxx"
#include "OutPort.hxx"

namespace CLAM {

class ProcessingConfig;

/** Processing class for performing a STFT. Its main functionality is to obtain
 *  a spectrum out of an incoming audio signal, applying windowing, circular shift
 *  and zero padding. Its different 
 */
class AudioWindowing : public ProcessingComposite
{

public:
	typedef AudioWindowingConfig Config;
// Processing Object compliance methods.

	const char *GetClassName() const {return "AudioWindowing";}

	AudioWindowing(const Config& config=Config())
		: mInput("Input",this)
		, mOutput("Output",this)
	{
		AttachChildren();
		Configure(config);
	}
	~AudioWindowing();
	
	const ProcessingConfig &GetConfig() const {return mConfig;}
	bool Do(void);
	bool Do(const Audio & in,Audio & out);

private:
	// Internal configuration data
	Config mConfig;

	// The internal Processing Objects
	WindowGenerator mWindowGenerator;
	AudioMultiplier mAudioProduct;
	CircularShift mCircularShift;

	// Internal Data objects

	/// An audio object containing the window that will
	/// be multiplied by each incomming frame.
	Audio mWindow; 

	// Internal convenience methods.
	void AttachChildren();
	bool ConfigureChildren();
	void ConfigureData();

	// Redefined methods
	bool ConcreteConfigure(const ProcessingConfig&);

	// Ports
	AudioInPort     mInput;
	OutPort<Audio> mOutput;
};

} //namespace

#endif

