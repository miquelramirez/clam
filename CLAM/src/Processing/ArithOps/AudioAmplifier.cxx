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
		SetInitGain(1.);
	}

	void AudioAmplifier::ResizePorts(unsigned newSize)
	{
		unsigned oldSize = _inputs.size();
		CLAM_ASSERT(_inputs.size()==_outputs.size(),
			"AudioAmplifier had different number of inputs and outputs");
		for (unsigned i = newSize; i<oldSize; i++)
		{
			delete _inputs[i];
			delete _outputs[i];
		}
		_inputs.resize(newSize);
		_outputs.resize(newSize);
		for (unsigned i = oldSize; i<newSize; i++)
		{
			std::ostringstream number;
			if (i>0) number << " " << i;
			_inputs[i] = new AudioInPort("Input Audio" + number.str(), this );
			_outputs[i] = new AudioOutPort ( "Audio Output" + number.str(), this);
		}
	}
}
