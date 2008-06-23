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

#ifndef AudioFileMemoryLoader_hxx
#define AudioFileMemoryLoader_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/MonoAudioFileReaderConfig.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/OutControl.hxx>

namespace CLAM
{

	class AudioFileMemoryLoader : public Processing
	{
	public:
		
		typedef MonoAudioFileReaderConfig Config;
		
		AudioFileMemoryLoader( const ProcessingConfig& cfg = Config() );
   
		virtual const char* GetClassName() const;
		virtual const ProcessingConfig& GetConfig() const;
		bool Do( Audio & outputSamples );
		virtual bool Do();	
		virtual ~AudioFileMemoryLoader();

	protected: // methods

		virtual bool ConcreteConfigure( const ProcessingConfig& cfgObject );
		virtual bool ConcreteStart();

	protected: // attributes
		
		Config       _config;
		AudioOutPort _output;
		OutControl   _timeOutput;
		InControl    _positionInput;
		TControlData _lastPosition;
		TControlData _delta;
		long         _position;
		Audio        _samples;
		TData        _sampleRate;
	};
}

#endif // AudioFileMemoryLoader.hxx
