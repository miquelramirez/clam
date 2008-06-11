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

#include "AudioFileMemoryLoader.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "AudioFileMemoryLoader",
		"category", "Audio File I/O",
		"description", "AudioFileMemoryLoader",
		0
	};
	static FactoryRegistrator<ProcessingFactory, AudioFileMemoryLoader> reg = metadata;
}
	
	
	AudioFileMemoryLoader::AudioFileMemoryLoader( const ProcessingConfig& cfg )
		: _output( "Samples Read", this ),
		  _timeOutput( "Current Time Position", this),
		  _position ( 0 )
	{
		Configure( cfg );
	}

	AudioFileMemoryLoader::~AudioFileMemoryLoader()
	{
	}
	
	// TODO: move it to the header
	const char* AudioFileMemoryLoader::GetClassName() const
	{
		return "AudioFileMemoryLoader";
	}

	const ProcessingConfig& AudioFileMemoryLoader::GetConfig() const
	{
		return _config;
	}

	bool AudioFileMemoryLoader::ConcreteConfigure( const ProcessingConfig& cfgObject )
	{
		CopyAsConcreteConfig( _config, cfgObject );

		MonoAudioFileReader reader(_config);
		
		if ( !reader.Configure (_config) )
		{
			AddConfigErrorMessage(reader.GetConfigErrorMessage());
			return false;
		}

		reader.Start();
		_samples.SetSize(reader.GetHeader().GetSamples());
		reader.Do(_samples);
		
		return true;
	}

	bool AudioFileMemoryLoader::ConcreteStart()
	{
		return true;
	}

	bool AudioFileMemoryLoader::Do()
	{
		bool result = Do( _output.GetAudio() );
		_output.Produce();

		return result;
	}


	
	bool AudioFileMemoryLoader::Do( Audio & outputSamples )
	{
		CLAM::TData * samplesArray = &_samples.GetBuffer()[0];
		CLAM::TData * outputArray = &outputSamples.GetBuffer()[0];
		
		long samplesLeft = _samples.GetSize() - _position;
		long length = outputSamples.GetSize();
		if (length > samplesLeft)
			length = samplesLeft;
		
		long i;
		for (i = 0; i < length; i++)
		{
			outputArray[i] = samplesArray[_position];
			_position++;
		}
		length = outputSamples.GetSize();
		for (; i < length; i++)
		{
			outputArray[i] = 0.0;
		}
		
		return true;
	}
	
}

