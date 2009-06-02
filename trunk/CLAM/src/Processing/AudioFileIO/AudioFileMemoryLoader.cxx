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

#include <cmath>

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
		: _output( "Samples Read", this )
		, _timeOutput( "Current Time Position", this )
		, _positionInput( "Current Time Position (%)", this )
		, _lastPosition( 0 )
		, _position( 0 )
	{
		Configure( cfg );
		_positionInput.SetBounds( 0.0, 1.0 );
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
		
		if ( !reader.IsConfigured() )
		{
			AddConfigErrorMessage(reader.GetConfigErrorMessage());
			return false;
		}

		reader.Start();
		_samples.SetSize(reader.GetHeader().GetSamples());
		reader.Do(_samples);
		
		_sampleRate = reader.GetHeader().GetSampleRate();
		_delta = 0.9 / reader.GetHeader().GetSamples();
		_position = 0;
		_timeOutput.SendControl(0);
		
		return true;
	}

	bool AudioFileMemoryLoader::ConcreteStart()
	{
		_position = 0;
		_timeOutput.SendControl(0);
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
		
		CLAM::TControlData currentPosition = _positionInput.GetLastValue();
		if (std::fabs (currentPosition - _lastPosition) > _delta)
		{
			_lastPosition = currentPosition;
			_position = long(currentPosition * TControlData(_samples.GetSize()));
		}
		
		long lastSample = _samples.GetSize() - 1;
		long length = outputSamples.GetSize();
		bool loop = _config.GetLoop();
		long i = 0;
		while (i < length)
		{
			long samplesLeft = lastSample - _position;
			if (samplesLeft > length - i)
				samplesLeft = length - i;
			
			for (; samplesLeft > 0; samplesLeft--)
			{
				outputArray[i] = samplesArray[_position];
				i++;
				_position++;
			}
			
			if (!loop)
				break;
			
			if (_position >= lastSample)
				_position = 0;
		}
		
		for (; i < length; i++)
		{
			outputArray[i] = 0.0;
		}
		
		_timeOutput.SendControl(float(_position) / _samples.GetSize());
		
		return true;
	}
	
	unsigned long AudioFileMemoryLoader::GetSamples()
	{
		return _samples.GetSize();
	}
	
}

