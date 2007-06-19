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

#include "AudioOut.hxx"
#include "AudioManager.hxx"
#include "ProcessingFactory.hxx"



namespace CLAM
{

namespace detail
{
	static FactoryRegistrator<ProcessingFactory, AudioOut> regAudioOut("AudioOut");
}
	
AudioOut::AudioOut() 
	: mInput( "Audio Input", this )
{ 
	mpDevice = 0; 
	Configure(AudioIOConfig()); 
}

AudioOut::AudioOut(const AudioIOConfig &c)
	: mInput( "Audio Input", this )
{ 
	mpDevice = 0; 
	Configure(c); 
}

AudioOut::~AudioOut()
{ 
	try {
		if (mpDevice) 
			mpDevice->Unregister(*this); 
	}
	catch (Err &e) {
		std::cerr << "AudioOut::~AudioOut: unregistering processing: " << e.what() << std::endl; 
	}
}

bool AudioOut::ConcreteConfigure(const ProcessingConfig& c)
	throw(ErrProcessingObj)
{
	CopyAsConcreteConfig(mConfig, c);

	if (mpDevice)
		mpDevice->Unregister(*this);

	AudioManager *m;

	try {
		m = &(AudioManager::Current());
	}
	catch (Err &e) {
		ErrProcessingObj ne("AudioOut::ConcreteConfigure(): No AudioManager found.",this);
		ne.Embed(e);
		throw(ne);
	}

	bool res;
	try {
		res = m->Register(*this);
	}
	catch (Err &e) {
		AddConfigErrorMessage( e.what() );
		res = false;
	}

	if (res == false)
		AddConfigErrorMessage( "AudioOut::ConcreteConfigure(): "
		       "Failed to register in AudioManager.") ;

	mInput.SetSize(mConfig.GetFrameSize());
	return res;
}

bool AudioOut::ConcreteStart(void) 
{
	if (!mpDevice)
		throw Err("AudioOut::ConcreteStart(): No Device found!");
	try
	{
		mpDevice->Start();
	}
	catch (Err &e) {
		std::cerr << " AudioOut::ConcreteStart(): could not start audio device: maybe another program is blocking it" << std::endl;
		std::cerr << " Received error message: <"<<e.what() << std::endl;
	}
	return true;
}

bool AudioOut::ConcreteStop()
{
	mpDevice->Stop();
	return true;
}

void AudioOut::GetDeviceInfo(AudioDevice::TInfo &info) const
{
	if (mpDevice)
		mpDevice->GetInfo(info);
	else
		info.Reset();
}

bool AudioOut::Do()
{
	bool res = Do(mInput.GetAudio());
	mInput.Consume();;
	return res;
}

}

