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
		: _Output( "Samples Read", this ),
		  _TimeOutput( "Current Time Position", this)
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
		return _Config;
	}

	bool AudioFileMemoryLoader::ConcreteConfigure( const ProcessingConfig& cfgObject )
	{
		CopyAsConcreteConfig( _Config, cfgObject );

		MonoAudioFileReader reader(_Config);
		Audio audio;
		
		if ( !reader.Configure (_Config) )
		{
			AddConfigErrorMessage(reader.GetConfigErrorMessage());
			return false;
		}

		reader.Start();
		audio.SetSize(reader.GetHeader().GetSamples());
		reader.Do(audio);
		
		return true;
	}

	bool AudioFileMemoryLoader::ConcreteStart()
	{
		return true;
	}

	bool AudioFileMemoryLoader::Do()
	{
		bool result = Do( _Output.GetAudio() );
		_Output.Produce();

		return result;
	}


	
	bool AudioFileMemoryLoader::Do( Audio & outputSamples )
	{
		return true;
	}
	
}

