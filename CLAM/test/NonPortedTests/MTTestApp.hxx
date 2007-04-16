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

#ifndef __MTTESTAPP__
#define __MTTESTAPP__

#include "GUIAudioApplication.hxx"
#include "AudioIO.hxx"
#include "Audio.hxx"
#include "Oscillator.hxx"
#include "OscillatorPresentation.hxx"
#include "OscillatorGView.hxx"
#include "AudioMultiplier.hxx"
#include "AudioAdder.hxx"

namespace CLAMTest
{
// Outer namespace symbol importing
using CLAM::GUIAudioApplication;
using CLAM::Audio;
using CLAM::Oscillator;
using CLAM::OscillatorConfig;
using CLAM::AudioIn;
using CLAM::AudioOut;
using CLAM::AudioIOConfig;
using CLAM::AudioManager;
using CLAM::Err;
using CLAM::AudioMultiplier;
using CLAM::AudioAdder;

class MTTestApp 
	: public GUIAudioApplication
{

public:

	MTTestApp() 
		: GUIAudioApplication(), finalize( false )
	{
	}

	void UserMain(); // overriden method (GUI things)

	void AudioMain(); // overriden method for inserting our desired Audio Processing

	void AppCleanup();

	static void SAudioThreadCleanUp( MTTestApp* pThis );

	virtual ~MTTestApp()
	{
	}

private:
	
	// Should class members be only shared objects between threads?

	bool finalize;

	// GUI thingies
	// These need to be declared here, since they are just in between
	CLAMGUI::AtomView< Oscillator >    mCarView;
	CLAMGUI::AtomView< Oscillator >    mModView;
	CLAMGUI::AtomView< Oscillator >    mAmpModView;


};

}

#endif // MTTestApp.hxx

