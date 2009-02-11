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

#include "AudioAmplifier.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "AudioAmplifier",
		"category", "Arithmetic Operations",
		"description", "AudioAmplifier",
		0
	};
	static FactoryRegistrator<ProcessingFactory, AudioAmplifier> reg = metadata;
}

	void AudioAmplifierConfig::DefaultInit( )
	{
		AddAll();
		UpdateData();
		SetMaxGain(1.);
		SetPortsNumber(1);
	}

	void AudioAmplifier::CreatePorts()
	{
		unsigned portsNumber=1;
		if (mConfig.HasPortsNumber())
			portsNumber=mConfig.GetPortsNumber();
		for( unsigned i=0; i<portsNumber; i++ )
		{
			std::ostringstream number("");
			if (i>0)
				number << " "<< i;
			AudioInPort * inPort = new AudioInPort( "Input Audio" + number.str(), this );
			mInputPorts.push_back( inPort );
			AudioOutPort * outPort = new AudioOutPort ( "Audio Output" + number.str(), this);
			mOutputPorts.push_back(outPort);
		}
	}

	void AudioAmplifier::RemovePorts()
	{
		std::vector< AudioInPort* >::iterator itInPort;
		for(itInPort=mInputPorts.begin(); itInPort!=mInputPorts.end(); itInPort++)
			delete *itInPort;
		mInputPorts.clear();
		
		std::vector<AudioOutPort*>::iterator itOutPort;
		for(itOutPort=mOutputPorts.begin(); itOutPort!=mOutputPorts.end(); itOutPort++)
			delete *itOutPort;
		mOutputPorts.clear();

		GetInPorts().Clear();
		GetOutPorts().Clear();
	}





}
